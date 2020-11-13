#include "pch.h"
#include "SGameClient.h"
#include "NetClient.hpp"
#include "BoardManager.h"
#include "StateManager.h"

SGameClient::SGameClient(StateManager & stateManager) :
	BaseGame(stateManager, States::MultiplayerClient),
	m_client(*stateManager.getContext().netClient)
{
	m_gameState = GameState::None;
}

SGameClient::~SGameClient()
{ }

void SGameClient::onCreate()
{
	//Start listening for server messages and find if we are black/white...
}

bool SGameClient::update(float deltaTime)
{
	return false;
}

bool SGameClient::onEvent(const sf::Event & event)
{
	return false;
}

void SGameClient::onResetBoard()
{
}

void SGameClient::onSwitchBoard()
{
}

void SGameClient::onQuitGame()
{
	m_client.disconnect();
}
