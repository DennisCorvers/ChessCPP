#pragma once
#include "BaseState.hpp"
#include "StateManager.h"
#include "NetServer.hpp"
#include "NetClient.hpp"

class STest : public BaseState
{
private:
	NetServer m_server;
	NetClient m_client;
	NetClient m_newClient;

public:
	STest(StateManager& stateManager) :
		BaseState(stateManager), m_server(1001)
	{

	}

	virtual ~STest() {};

	virtual void onCreate() override
	{
		m_server.startListening(2);
		m_client.connect(sf::IpAddress(127, 0, 0, 1), 1001);
		m_newClient.connect(sf::IpAddress(127, 0, 0, 1), 1001);

		m_client.bindPacketHandler(&STest::onClientPacket, this);
		m_newClient.bindPacketHandler(&STest::onClientPacket, this);
	};

	void onClientPacket(sf::Packet& packet) {
		int num = 0;
		packet >> num;
		std::cout << "Client received packet: " << num << std::endl;
	}

	virtual void onDestroy() override
	{
	};

	virtual void activate() override
	{
	};

	virtual void deactivate() override
	{
	};

	virtual void render() override
	{

	};

	virtual bool update(float deltaTime) override
	{
		auto client = m_server.tryAccept();

		std::cout << "Server sending... " << std::endl;
		//m_server.sendToAll(sf::Packet() << deltaTime);
		//m_client.receive();
		m_newClient.receive();

		return false;
	};

	virtual bool handleEvent(const sf::Event & event) override
	{
		return false;
	};

};

