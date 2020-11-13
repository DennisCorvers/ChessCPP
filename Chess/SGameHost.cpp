#include "pch.h"

#include "SGameHost.h"
#include "NetServer.hpp"
#include "BoardManager.h"
#include "StateManager.h"

SGameHost::SGameHost(StateManager & stateManager)
	:BaseGame(stateManager, States::MultiplayerHost),
	m_clientID(-1)
{
	m_server = std::make_unique<NetServer>(stateManager.getContext().netSettings.Port);
	m_gameState = GameState::None;
}

SGameHost::~SGameHost()
{ }

void SGameHost::onCreate()
{
	//Only allow one player...
	m_server->startListening(1);
	m_boardManager->resetGame(PieceColour::White);
}

bool SGameHost::update(float deltaTime)
{
	

	return false;
}

bool SGameHost::onEvent(const sf::Event & event)
{
	return false;
}

void SGameHost::onResetBoard()
{
}

void SGameHost::onSwitchBoard()
{
}

void SGameHost::onQuitGame()
{
	m_server->closeServer();
}
