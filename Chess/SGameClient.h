#pragma once
#include "BaseGame.h"
#include "PacketHandler.hpp"

class sf::Packet;
class NetClient;

class SGameClient : public BaseGame
{
private:
	enum struct ConnectionState{ Connected, None, Failed, Disconnected };
	static const int CONNECT_TIMEOUT = 15;

	NetClient& m_client;
	ChessColour m_myColour;
	PacketHandler m_packetHandler;
	bool m_clientIsPlayer;

	float m_connectionTimeout = CONNECT_TIMEOUT;
	ConnectionState m_connectionState = ConnectionState::None;

public:
	SGameClient(StateManager& stateManager);
	virtual ~SGameClient();

	virtual void onCreate() override;
	virtual void onDestroy() override {};

	virtual void activate() override {};
	virtual void deactivate() override {};

	virtual bool update(float deltaTime) override;
	virtual bool onEvent(const sf::Event & event) override;

	// Inherited via BaseGame
	virtual void onQuitGame() override;

private:
	void onNetPacket(sf::Packet& packet);
	void onDisconnect();

	void onReset(sf::Packet& packet);
	void onSnapshot(sf::Packet& packet);
	void onRemoteInput(sf::Packet& packet);
	void onConnectResponse(sf::Packet& packet);

	void onConnectFailed();
};