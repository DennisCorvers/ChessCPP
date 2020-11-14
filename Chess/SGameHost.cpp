#include "pch.h"

#include "SGameHost.h"
#include "NetServer.hpp"
#include "BoardManager.h"
#include "StateManager.h"
#include "PacketType.h"

SGameHost::SGameHost(StateManager & stateManager) :
	BaseGame(stateManager, States::MultiplayerHost),
	m_myColour(PieceColour::White),
	m_clientID(-1),
	m_poller(1)
{
	//m_server = std::make_unique<NetServer>(stateManager.getContext().netSettings.Port);
	m_server = std::make_unique<NetServer>(1001); //HACK Temp for testing...
	m_server->setup(&SGameHost::onNetPacket, &SGameHost::onDisconnect, this);
	m_gameState = GameState::None;
}

SGameHost::~SGameHost()
{ }

void SGameHost::onCreate()
{
	//Only allow one player...
	m_server->startListening(1);

	m_boardManager->resetGame(m_myColour);

	std::cout << "Waiting for player" << std::endl;
}

bool SGameHost::update(float deltaTime)
{
	BaseGame::update(deltaTime);

	if (m_poller.poll(deltaTime))
	{
		int id = m_server->tryAccept();
		if (id > -1)
			onConnect(id);
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
	packet << m_myColour.opposite();
	m_server->broadCast(packet);

	onCreate();
}

void SGameHost::onSwitchBoard()
{
	m_myColour.swap();
	onResetBoard();
}

void SGameHost::onQuitGame()
{
	m_server->closeServer();
	BaseGame::onQuitGame();
}


void SGameHost::onNetPacket(int clientID, sf::Packet& packet)
{
	//Only accept messages from the remote player
	if (clientID != m_clientID)
		return;


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
	if (m_clientID == clientID) {
		//TODO: Display gameover screen and reset game for next client...
		m_boardManager->resetGame(m_myColour);
		std::cout << "Player disconnected" << std::endl;
		m_clientID = -1;
	}
}

void SGameHost::onConnect(int clientID)
{
	sf::Packet connectPacket;
	connectPacket << PacketType::Connect;

	if (m_clientID == -1) {
		m_clientID = clientID;
		
		m_gameState = GameState::Playing;
		std::cout << "Player connected" << std::endl;

		connectPacket << sf::Uint8(1);
	}
	else
	{
		connectPacket << sf::Uint8(0);
		//Send observers a snapshot of the game in progress...
	}

	connectPacket << m_myColour.opposite();
	m_server->sendToClient(connectPacket, m_clientID);

	std::cout << "Client connected" << std::endl;
}

void SGameHost::onRemoteInput(sf::Packet& packet)
{
	ChessMove newMove;
	newMove.netSerialize(packet, false);

	if (packet && inputMove(newMove, true, true)) {
		//Boardcast valid moves to observers...
		std::cout << "Valid input" << std::endl;
	}
	else
	{
		sf::Packet snapshot;
		snapshot << PacketType::Snapshot;

		m_boardManager->serializeBoard(snapshot, true);
		m_server->sendToClient(snapshot, m_clientID);
		std::cout << "Invalid input" << std::endl;
	}
}
