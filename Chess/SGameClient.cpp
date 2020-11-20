#include "pch.h"
#include "SGameClient.h"
#include "NetClient.hpp"
#include "BoardManager.h"
#include "StateManager.h"
#include "PacketType.h"
#include "GuiGameMessage.h"

SGameClient::SGameClient(StateManager & stateManager) :
	BaseGame(stateManager, States::MultiplayerClient),
	m_client(*stateManager.getContext().netClient),
	m_myColour(PieceColour::Black),
	m_clientIsPlayer(false),
	m_connectionTimeout(CONNECT_TIMEOUT),
	m_connectionState(ConnectionState::None)
{
	m_gameState = GameState::None;

	m_packetHandler.registerHandle(PacketType::NewMove, &SGameClient::onRemoteInput, this);
	m_packetHandler.registerHandle(PacketType::ResetBoard, &SGameClient::onReset, this);
	m_packetHandler.registerHandle(PacketType::Snapshot, &SGameClient::onSnapshot, this);
	m_packetHandler.registerHandle(PacketType::Connect, &SGameClient::onConnectResponse, this);
}

SGameClient::~SGameClient()
{ }

void SGameClient::onCreate()
{
	m_client.setup(&SGameClient::onNetPacket, &SGameClient::onDisconnect, this);
	auto& netSettings = m_stateManager->getContext().netSettings;
	m_client.connect(sf::IpAddress(netSettings.IpAddress), netSettings.getPort());

	guiLoadShow("Connecting to server");
}

bool SGameClient::update(float deltaTime)
{
	BaseGame::update(deltaTime);

	m_client.receive();

	if (m_connectionState == ConnectionState::None) {
		m_connectionTimeout -= deltaTime;
		if (m_connectionTimeout <= 0)
			onConnectFailed();
	}

	return false;
}

bool SGameClient::onEvent(const sf::Event & event)
{
	if (m_gameState != GameState::Playing) {
		return false;
	}

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
					m_client.send(packet);
				}
			}
		}
	}

	return false;
}

void SGameClient::onQuitGame()
{
	m_client.disconnect();
	m_client.clearHandlers();
	m_packetHandler.clearHandles();

	BaseGame::onQuitGame();
}

void SGameClient::onNetPacket(sf::Packet & packet)
{
	PacketType pType;
	if (!(packet >> pType))
		return;

	m_packetHandler.handlePacket(pType, packet);
}

void SGameClient::onDisconnect()
{
	m_gameState = GameState::None;

	if (m_connectionState == ConnectionState::Connected)
		BaseGame::endGame("Connection to server was lost.");
	else
	{
		//Connection rejected
		guiLoadHide();
		displayMessage("Error", "Unable to connect to server.", "Ok");
	}

	m_connectionState = ConnectionState::Disconnected;
}

void SGameClient::onReset(sf::Packet& packet)
{
	PieceColour newColour;
	if (!(packet >> newColour))
		return;

	if (m_clientIsPlayer) {
		m_gameState = GameState::Playing;
		m_myColour = newColour;
	}
	else {
		m_myColour = PieceColour::White;
		m_gameState = GameState::None;
	}

	m_boardManager->resetGame(m_myColour);
}

void SGameClient::onSnapshot(sf::Packet& packet)
{
	m_boardManager->serializeBoard(packet, false);
}

void SGameClient::onRemoteInput(sf::Packet& packet)
{
	ChessMove newMove;
	newMove.netSerialize(packet, false);

	if (packet) {
		if (!inputMove(newMove, false, true)) {
			//Server is lying!
		}
	}
}

void SGameClient::onConnectResponse(sf::Packet& packet)
{
	m_connectionState = ConnectionState::Connected;
	guiLoadHide();

	sf::Uint8 myRole;
	packet >> myRole;

	if (myRole == 1) {
		m_clientIsPlayer = true;
		PieceColour myColour;
		packet >> myColour;
		m_myColour = myColour;

		m_boardManager->resetGame(m_myColour);
		m_gameState = GameState::Playing;

	}
	else
	{
		displayMessage("Joined Server", "You joined as a spectator.", "Continue");
		m_myColour = PieceColour::White;
		onSnapshot(packet);

		m_gameState = GameState::None;
	}
}

void SGameClient::onConnectFailed()
{
	guiLoadHide();
	displayMessage("Error", "Server connection timeout", "Ok");
	m_client.disconnect();
	m_connectionState = ConnectionState::Failed;
	m_connectionTimeout = CONNECT_TIMEOUT;
}
