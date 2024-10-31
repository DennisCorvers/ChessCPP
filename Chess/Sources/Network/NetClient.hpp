#pragma once
#include "SFML/Network.hpp"
#include <functional>

class NetClient {

private:
	using SocketStatus = sf::Socket::Status;
	using PacketHandler = std::function<void(sf::Packet& packet)>;
	using DisconnectHandler = std::function<void()>;

	std::unique_ptr<sf::TcpSocket> m_clientSocket;

	PacketHandler m_packetHandler;
	DisconnectHandler m_disconnectHandler;

public:
	NetClient()
	{
		m_clientSocket = std::make_unique<sf::TcpSocket>();
		m_clientSocket->setBlocking(false);
	}

	virtual ~NetClient() { }

	NetClient(const NetClient&) = delete;
	NetClient& operator=(const NetClient&) = delete;

	NetClient(NetClient&&) = default;
	NetClient& operator = (NetClient&&) = default;

	template<typename T>
	void bindPacketHandler(void(T::*func)(sf::Packet&), T* instance) {
		m_packetHandler = std::bind(func, instance, std::placeholders::_1);
	}

	template<typename T>
	void bindDisconnectHandler(void(T::*func)(), T* instance) {
		m_disconnectHandler = std::bind(func, instance);
	}

	template<typename T>
	void setup(void(T::*onPacket)(sf::Packet&), void(T::*onDisconnect)(), T* instance) {
		bindPacketHandler(onPacket, instance);
		bindDisconnectHandler(onDisconnect, instance);
	}

	bool isConnected() const {
		return m_clientSocket->getRemoteAddress() != sf::IpAddress::None;
	}

	const sf::IpAddress getRemoteIP() const {
		return m_clientSocket->getRemoteAddress();
	}

	unsigned short getRemotePort() const {
		return m_clientSocket->getRemotePort();
	}

	bool connect(const sf::IpAddress& ip, unsigned short port) {
		return m_clientSocket->connect(ip, port) == SocketStatus::Done;
	}

	bool receive() {
		if (!isConnected())
			return false;

		sf::Packet nextPacket;
		SocketStatus socketStatus = m_clientSocket->receive(nextPacket);

		switch (socketStatus) {
		case SocketStatus::Done: {

			if (m_packetHandler)
				m_packetHandler(nextPacket);

			return true;
		}
		case SocketStatus::Error: {
			//Do something...?
		}
		case SocketStatus::Disconnected: {
			m_clientSocket->disconnect();

			if (m_disconnectHandler)
				m_disconnectHandler();

			return true;
		}
		}

		return false;
	}

	bool send(sf::Packet& packet) {
		SocketStatus sendStatus;

		while (true) {
			sendStatus = m_clientSocket->send(packet);

			if (sendStatus == SocketStatus::Partial)
				continue;

			break;
		}

		return sendStatus == SocketStatus::Done;
	}

	void disconnect() {
		m_clientSocket->disconnect();
	}

	void clearHandlers() {
		m_packetHandler = nullptr;
		m_disconnectHandler = nullptr;
	}
};
