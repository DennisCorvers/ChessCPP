#include "pch.h"

#include "SGameHost.h"
#include "NetServer.hpp"
#include "BoardManager.h"
#include "StateManager.h"
#include "PacketType.hpp"

SGameHost::SGameHost(StateManager & stateManager) :
	BaseGame(stateManager, States::MultiplayerHost),
	m_myColour(PieceColour::White),
	m_clientID(-1),
	m_poller(1)
{
	//m_server = std::make_unique<NetServer>(stateManager.getContext().netSettings.Port);
	m_server = std::make_unique<NetServer>(1001);
	m_gameState = GameState::None;
}

SGameHost::~SGameHost()
{ }

void SGameHost::onCreate()
{
	//Only allow one player...
	m_server->startListening(1);

	m_boardManager->resetGame(m_myColour);
}

bool SGameHost::update(float deltaTime)
{
	if (m_poller.poll(deltaTime)) {

		int id = m_server->tryAccept();
		if (id > -1)
		{
			if (m_clientID == -1) {
				m_clientID = id;
				GameState::Playing;
				//New player...
			}
			//New observer...
		}
		std::cout << "Listening..." << std::endl;
	}

	m_server->receive();

	return false;
}

bool SGameHost::onEvent(const sf::Event& event)
{
	if (m_gameState != GameState::Playing)
		return false;

	if (event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mousePos = getWindow().mapPixelToCoords(sf::Mouse::getPosition(getWindow()), m_view);

		if (event.type == sf::Event::MouseButtonPressed)
			m_boardManager->startSelection(mousePos, m_myColour);

		if (event.type == sf::Event::MouseButtonReleased) {
			ChessMove newMove;
			if (m_boardManager->endSelection(mousePos, newMove)) {
				if (inputMove(newMove, false, false)) {
					sf::Packet packet;
					packet << PacketType::NewMove;
					newMove.netSerialize(packet, true);

					m_server->broadCast(packet);
				}
			}
		}
	}

	return false;
}

void SGameHost::onResetBoard()
{
	sf::Packet packet;
	packet << PacketType::ResetBoard;

	m_server->broadCast(packet);

	onCreate();
}

void SGameHost::onSwitchBoard()
{
	sf::Packet packet;
	packet << PacketType::SwapColour;

	m_server->broadCast(packet);

	m_myColour.swap();
	onCreate();
}

void SGameHost::onQuitGame()
{
	m_server->closeServer();
	BaseGame::onQuitGame();
}


void SGameHost::onNetPacket(sf::Packet& packet)
{
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

		clientID = -1;
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
