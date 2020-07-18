#include "pch.h"
#include "SGameSinglePlayer.h"

#include "URIConnector.hpp"
#include "BoardManager.h"
#include "StateManager.h"

SGameSinglePlayer::SGameSinglePlayer(StateManager & stateManager) :
	BaseGame(stateManager, States::SinglePlayer),
	m_myColour(PieceColour::White)
{
	auto engineInfo = URI::EngineInformation();
	m_stockfish = std::make_unique<URI::URIConnector>("stockfish.exe", engineInfo);
}

SGameSinglePlayer::~SGameSinglePlayer()
{

}

void SGameSinglePlayer::switchBoard() {
	m_myColour = m_myColour == PieceColour::Black ? PieceColour::White : PieceColour::Black;
	m_boardManager->resetGame(m_myColour);
}

void SGameSinglePlayer::onCreate() {
	m_boardManager->resetGame(m_myColour);
	m_stockfish->resetGame();
}

void SGameSinglePlayer::onDestroy()
{
}

void SGameSinglePlayer::activate()
{
}

void SGameSinglePlayer::deactivate()
{

}

bool SGameSinglePlayer::update(float deltaTime)
{
	BaseGame::update(deltaTime);

	m_stockfish->runEngine(deltaTime);
	if (m_stockfish->pollEngine()) {
		auto message = m_stockfish->getMessage();
		std::cout << message.message << std::endl;
		switch (message.messageType)
		{
		case URI::EngineMessageType::BestMove:
			m_boardManager->inputMove(ChessMove::fromChessNotation(message.message), true, true);
			break;
		default:
			break;
		}
	}
	return false;
}

bool SGameSinglePlayer::handleEvent(const sf::Event & event)
{
	BaseGame::handleEvent(event);

	if (event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window), m_view);

		if (event.type == sf::Event::MouseButtonPressed)
			m_boardManager->startSelection(mousePos, m_myColour);

		if (event.type == sf::Event::MouseButtonReleased) {
			ChessMove newMove;
			if (m_boardManager->endSelection(mousePos, newMove)) {
				if (m_boardManager->inputMove(newMove, false, false)) {
					m_stockfish->requestMove(m_boardManager->getFENFormat());
				}
			}
		}
	}

	if (event.type == EType::KeyReleased && event.key.code == sf::Keyboard::Escape) {
		m_stateManager->switchState(States::Pause);
	}

	if (event.type == EType::KeyReleased && event.key.code == sf::Keyboard::Q) {
		m_stockfish->requestMove(m_boardManager->getFENFormat());
	}

	return false;
}

void SGameSinglePlayer::onQuitGame(const EventArgs & eventInfo) {
	m_stateManager->switchState(States::MainMenu);
	m_stateManager->removeState(States::SinglePlayer);
}

void SGameSinglePlayer::onResetBoard(const EventArgs& eventInfo) {
	m_boardManager->resetGame();
	m_stockfish->resetGame();
}


