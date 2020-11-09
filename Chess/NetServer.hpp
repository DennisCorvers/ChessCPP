#pragma once
#include "SFML/Network.hpp"
#include <functional>
#include <unordered_map>

struct ClientInfo {
private:
	friend class NetServer;
	int m_clientID;

public:
	ClientInfo(int clientID) :
		m_clientID(clientID)
	{ }

	int getClientID() const {
		return m_clientID;
	}
};

struct ClientConnection final {
private:
	friend class NetServer;
	std::unique_ptr<sf::TcpSocket> m_socket;
	ClientInfo m_clientInfo;

	ClientConnection(const ClientConnection&) = delete;
	ClientConnection& operator=(const ClientConnection&) = delete;

public:
	ClientConnection(std::unique_ptr<sf::TcpSocket> socket, ClientInfo& clientInfo) :
		m_socket(std::move(socket)),
		m_clientInfo(clientInfo)
	{ }

	const ClientInfo& getClientInfo() const {
		return m_clientInfo;
	}
};

class NetServer {
public:
	using PacketHandler = std::function<void>(int clientID, sf::Packet& packet);
	using DisconnectHandler = std::function<void>(int clientID);

private:
	using SocketStatus = sf::Socket::Status;
	using Clients = std::unordered_map<int, ClientConnection>;

	std::unique_ptr<sf::TcpSocket> m_pendingSock;
	std::unique_ptr<sf::TcpListener> m_listener;
	Clients m_clients;

	int m_lastID;
	unsigned short m_port;
	unsigned short m_maxClients;

	PacketHandler m_packetHandler;
	DisconnectHandler m_disconnectHandler;

public:

	NetServer(unsigned short port, unsigned short maxClients = 1) :
		m_port(port),
		m_maxClients(maxClients),
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

	bool startListening() {
		return m_listener->listen(m_port) == sf::Socket::Done;
	}

	int tryAccept() {
		if (getConnections() < m_maxClients)
			return -1;

		if (m_listener->accept(*m_pendingSock)) {
			m_pendingSock->setBlocking(false);

			//First connection is never a spectator.
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

		for (auto itr = m_clients.begin(); itr != m_clients.end();) {
			SocketStatus clientStatus = itr->second.m_socket->receive(nextPacket);

			switch (clientStatus) {
			case SocketStatus::Done: {
				if (m_packetHandler)
					m_packetHandler(itr->second.m_clientInfo.m_clientID, nextPacket);
			}
			case SocketStatus::Error: {
				//Do something?
			}
			case SocketStatus::Disconnected: {
				if (m_disconnectHandler)
					m_disconnectHandler(itr->second.m_clientInfo.m_clientID);

				itr = m_clients.erase(itr);
				continue;
			}
			}

			++itr;
		}
	}

	bool getClientInfo(int clientID, ClientInfo& clientInfo) const {
		auto itr = m_clients.find(clientID);
		if (itr == m_clients.end())
			return false;

		clientInfo = itr->second.m_clientInfo;
		return true;
	}

	bool disconnectClient(int clientID) {
		auto itr = m_clients.find(clientID);
		if (itr == m_clients.end())
			return false;

		(*itr).second.m_socket->disconnect();
		m_clients.erase(itr);

		return true;
	}

	void disconnectAll() {
		for (auto& client : m_clients)
			client.second.m_socket->disconnect();

		m_clients.clear();
	}

	int getConnections() const {
		return m_clients.size();
	}

	unsigned short getPort() const {
		return m_listener->getLocalPort();
	}

	void closeServer() {
		m_listener->close();

		disconnectAll();
	}
};