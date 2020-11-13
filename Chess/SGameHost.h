#pragma once
#include "BaseGame.h"
#include "Poller.hpp"

class sf::Packet;
class NetServer;
class SGameHost : public BaseGame
{
private:
	std::unique_ptr<NetServer> m_server;
	int m_clientID;

	ChessColour m_myColour;
	Poller m_poller;

public:
	SGameHost(StateManager& stateManager);
	virtual ~SGameHost();

	virtual void onCreate() override;
	virtual void onDestroy() override {};

	virtual void activate() override {};
	virtual void deactivate() override {};

	virtual bool update(float deltaTime) override;
	virtual bool onEvent(const sf::Event & event) override;

	// Inherited via BaseGame
	virtual void onResetBoard() override;
	virtual void onSwitchBoard() override;
	virtual void onQuitGame() override;

private:
	void onNetPacket(sf::Packet& packet);
	void onDisconnect(int clientID);

	void onRemoteInput(sf::Packet& packet);
};