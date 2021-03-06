#include "pch.h"

#include "SGameHost.h"
#include "NetServer.hpp"
#include "BoardManager.h"
#include "StateManager.h"
#include "PacketType.h"
#include "GuiInfoBox.h"

SGameHost::SGameHost(StateManager & stateManager) :
	BaseGame(stateManager, States::MultiplayerHost),
	m_myColour(PieceColour::White),
	m_playerID(-1), m_poller(1)
{
	m_server = std::make_unique<NetServer>(stateManager.getContext().netSettings.getPort());
	m_server->setup(&SGameHost::onNetPacket, &SGameHost::onDisconnect, this);

	m_hasBoundPort = m_server->startListening(10);
	if (!m_hasBoundPort)
		displayMessage("Error", "Unable to bind TCP Port: " + std::to_string(m_server->getPort()), "Ok");

	m_gameState = GameState::None;
}

SGameHost::~SGameHost()
{ }

void SGameHost::onCreate()
{
	m_boardManager->resetGame(m_myColour);

	if (!m_hasBoundPort)
		return;

	if (m_playerID == -1) {
		m_server->pauseListening(false);
		guiLoadShow("Waiting for player...");
	}
	else {
		m_gameState = GameState::Playing;
		guiLoadHide();
	}
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
	if (clientID != m_playerID)
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
	if (m_playerID == clientID) {
		endGame("Player disconnected.");

		m_server->pauseListening(true);
		m_playerID = -1;
		return;
	}

	auto itr = m_observers.begin();
	while (itr != m_observers.end()) {
		if ((*itr) == clientID) {
			m_observers.erase(itr);
			return;
		}
		++itr;
	}
}

void SGameHost::onConnect(int clientID)
{
	sf::Packet connectPacket;
	connectPacket << PacketType::Connect;

	if (m_playerID == -1) {
		m_playerID = clientID;

		m_gameState = GameState::Playing;
		guiLoadHide();

		connectPacket << sf::Uint8(1);
	}
	else
	{
		m_observers.push_back(clientID);

		connectPacket << sf::Uint8(0);
		m_boardManager->serializeBoard(connectPacket, true);
	}

	connectPacket << m_myColour.opposite();
	m_server->sendToClient(connectPacket, clientID);
}

void SGameHost::onRemoteInput(sf::Packet& packet)
{
	ChessMove newMove;
	newMove.netSerialize(packet, false);

	if (packet && inputMove(newMove, true, true)) {
		m_server->sendToClients(packet, m_observers);
	}
	else
	{
		sf::Packet snapshot;
		snapshot << PacketType::Snapshot;

		m_boardManager->serializeBoard(snapshot, true);
		m_server->sendToClient(snapshot, m_playerID);
	}
}
