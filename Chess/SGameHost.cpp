#include "pch.h"

#include "SGameHost.h"
#include "NetServer.hpp"
#include "BoardManager.h"
#include "StateManager.h"
#include "PacketType.hpp"

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

bool SGameHost::onEvent(const sf::Event& event)
{
	return false;
}

void SGameHost::onResetBoard()
{
	sf::Packet packet;
	packet << PacketType::ResetBoard;

	m_server->broadCast(packet);
}

void SGameHost::onSwitchBoard()
{
	sf::Packet packet;
	packet << PacketType::SwapColour;

	m_server->broadCast(packet);
}

void SGameHost::onQuitGame()
{
	m_server->closeServer();
}

void SGameHost::onNetPacket(sf::Packet& packet)
{
	//Process packet...
	PacketType pType;

	if (!(packet >> pType))
		return;

	switch (pType)
	{
	case PacketType::NewMove:
	{
		onRemoteInput(packet);
		break;
	}
	default: //Unknown packet...
		return;
	}
}

void SGameHost::onDisconnect(int clientID)
{
	if (clientID == m_clientID) {
		//Display gameover screen and reset game for next client...
	}
}

void SGameHost::onRemoteInput(sf::Packet& packet)
{
	ChessMove newMove;
	newMove.netSerialize(packet, false);

	if (packet && inputMove(newMove, true, true)) {
		//Boardcast valid moves to observers...
	}
	else 
	{
		sf::Packet snapshot;
		snapshot << PacketType::Snapshot;

		m_boardManager->serializeBoard(snapshot, true);
		m_server->sendToClient(snapshot, m_clientID);
	}
}
