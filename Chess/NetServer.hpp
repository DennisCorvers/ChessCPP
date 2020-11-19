#pragma once
#include "SFML/Network.hpp"
#include "ClientInfo.hpp"
#include <functional>
#include <unordered_map>

struct ClientConnection final {
private:
	friend class NetServer;
	std::unique_ptr<sf::TcpSocket> m_socket;
	ClientInfo m_clientInfo;

public:
	ClientConnection(std::unique_ptr<sf::TcpSocket> socket, ClientInfo& clientInfo) :
		m_socket(std::move(socket)),
		m_clientInfo(clientInfo)
	{ }

	ClientConnection(const ClientConnection&) = delete;
	ClientConnection& operator=(const ClientConnection&) = delete;

	ClientConnection(ClientConnection&&) = default;
	ClientConnection& operator = (ClientConnection&&) = default;

	const ClientInfo& getClientInfo() const {
		return m_clientInfo;
	}
};

class NetServer {
private:
	using SocketStatus = sf::Socket::Status;
	using ClientID = int;
	using PacketHandler = std::function<void(ClientID clientID, sf::Packet& packet)>;
	using DisconnectHandler = std::function<void(ClientID clientID)>;

	ClientID m_lastID;
	unsigned short m_port;
	unsigned short m_maxClients;
	bool m_pauseListening;

	PacketHandler m_packetHandler;
	DisconnectHandler m_disconnectHandler;

	std::unique_ptr<sf::TcpListener> m_listener;
	std::unordered_map<ClientID, ClientConnection> m_clients;

	std::vector<int> m_removedClients;
	std::unique_ptr<sf::TcpSocket> m_pendingSock;

public:
	NetServer(unsigned short port) :
		m_port(port),
		m_maxClients(0),
		m_lastID(0)
	{
		m_clients.reserve(1);
		m_listener = std::make_unique<sf::TcpListener>();
		m_pendingSock = std::make_unique<sf::TcpSocket>();

		m_listener->setBlocking(false);
	}

	virtual ~NetServer() {
		closeServer();
	}

	NetServer(const NetServer&) = delete;
	NetServer& operator=(const NetServer&) = delete;

	NetServer(NetServer&&) = default;
	NetServer& operator = (NetServer&&) = default;


	template<typename T>
	void bindPacketHandler(void(T::*func)(ClientID, sf::Packet&), T* instance) {
		m_packetHandler = std::bind(func, instance, std::placeholders::_1, std::placeholders::_2);
	}

	template<typename T>
	void bindDisconnectHandler(void(T::*func)(ClientID), T* instance) {
		m_disconnectHandler = std::bind(func, instance, std::placeholders::_1);
	}

	template<typename T>
	void setup(void(T::*onPacket)(ClientID, sf::Packet&), void(T::*onDisconnect)(ClientID), T* instance) {
		bindPacketHandler(onPacket, instance);
		bindDisconnectHandler(onDisconnect, instance);
	}


	int getConnections() const {
		return static_cast<int>(m_clients.size());
	}

	unsigned short getPort() const {
		return m_listener->getLocalPort();
	}

	bool getClientInfo(ClientID clientID, ClientInfo& clientInfo) const {
		auto itr = m_clients.find(clientID);
		if (itr == m_clients.end())
			return false;

		clientInfo = itr->second.m_clientInfo;
		return true;
	}

	int getMaxConnections() const {
		return m_maxClients;
	}


	bool startListening(unsigned short maxConnections = 1)
	{
		m_pauseListening = false;
		m_maxClients = maxConnections;
		return m_listener->listen(m_port) == sf::Socket::Done;
	}

	void pauseListening(bool pause) {
		m_pauseListening = pause;
	}

	ClientID tryAccept() {

		if (m_listener->accept(*m_pendingSock) == SocketStatus::Done) {

			//Reject any more connections above the maximum allowed amount.
			if (getConnections() >= m_maxClients || m_pauseListening) {
				m_pendingSock->disconnect();
				return -1;
			}

			m_pendingSock->setBlocking(false);

			ClientInfo cInfo = ClientInfo(m_lastID);
			m_clients.emplace(m_lastID, ClientConnection(std::move(m_pendingSock), cInfo));

			//Create new PendingSocket for the next connection.
			m_pendingSock = std::make_unique<sf::TcpSocket>();
			return m_lastID++;
		}

		return -1;
	}

	void receive() {
		sf::Packet nextPacket;
		nextPacket.clear();

		for (auto itr = m_clients.begin(); itr != m_clients.end();) {
			SocketStatus clientStatus = itr->second.m_socket->receive(nextPacket);

			switch (clientStatus) {
			case SocketStatus::Done: {
				m_packetHandler(itr->second.m_clientInfo.m_playerID, nextPacket);
				break;
			}
			case SocketStatus::Error: {
				//Do something?
			}
			case SocketStatus::Disconnected: {
				m_removedClients.push_back(itr->second.m_clientInfo.m_playerID);
				itr = m_clients.erase(itr);
				continue;
			}
			}

			++itr;
		}


		//Handle disconnected clients...
		for (auto clientID : m_removedClients) {
			if (m_disconnectHandler)
				m_disconnectHandler(clientID);
		}

		m_removedClients.clear();
	}


	void broadCast(sf::Packet& packet) {
		for (auto& client : m_clients)
			sendToClient(packet, client.second);
	}

	bool sendToClient(sf::Packet& packet, ClientID clientID) {
		auto itr = m_clients.find(clientID);
		if (itr == m_clients.end())
			return false;

		sendToClient(packet, itr->second);
		return true;
	}

	void sendToClients(sf::Packet& packet, const std::vector<ClientID>& clients) {
		for (const auto& clientID : clients) {
			auto itr = m_clients.find(clientID);
			if (itr == m_clients.end())
				continue;

			sendToClient(packet, itr->second);
		}
	}


	bool disconnectClient(ClientID clientID) {
		auto itr = m_clients.find(clientID);
		if (itr == m_clients.end())
			return false;

		disconnectClient(itr->second);
		m_clients.erase(itr);

		return true;
	}

	void disconnectClients(const std::vector<ClientID>& clients) {
		for (const auto& clientID : clients) {
			auto itr = m_clients.find(clientID);
			if (itr == m_clients.end())
				continue;

			m_clients.erase(itr);
			disconnectClient(itr->second);
		}
	}

	void disconnectAll() {
		for (auto& client : m_clients)
			disconnectClient(client.second);

		m_clients.clear();
	}


	void closeServer() {
		m_listener->close();

		disconnectAll();
	}

	void clearHandlers() {
		m_packetHandler = nullptr;
		m_disconnectHandler = nullptr;
	}

private:
	inline void sendToClient(sf::Packet& packet, ClientConnection& client) {
		while (true) {
			SocketStatus sendStatus = client.m_socket->send(packet);

			if (sendStatus == SocketStatus::Partial)
				continue;

			break;
		}
	}

	inline void disconnectClient(ClientConnection& client) {
		client.m_socket->disconnect();
	}
};