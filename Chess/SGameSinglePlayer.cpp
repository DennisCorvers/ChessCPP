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
	m_chessEngine = std::make_unique<URI::URIConnector>("stockfish.exe", engineInfo);
}

SGameSinglePlayer::~SGameSinglePlayer()
{

}

void SGameSinglePlayer::onCreate() {
	m_boardManager->resetGame();
	m_chessEngine->resetGame();
	m_chessEngine->setSkillLevel(1);
}

bool SGameSinglePlayer::update(float deltaTime)
{
	BaseGame::update(deltaTime);

	m_chessEngine->runEngine(deltaTime);
	if (m_chessEngine->pollEngine()) {
		auto message = m_chessEngine->getMessage();
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
					m_chessEngine->requestMove(m_boardManager->getFENFormat());
				}
			}
		}
	}

	if (event.type == EType::KeyReleased && event.key.code == sf::Keyboard::Escape) {
		m_stateManager->switchState(States::Pause);
	}

	return false;
}

void SGameSinglePlayer::onQuitGame(const EventArgs & eventInfo) {
	m_stateManager->switchState(States::MainMenu);
	m_stateManager->removeState(States::SinglePlayer);
}

void SGameSinglePlayer::onResetBoard(const EventArgs& eventInfo) {
	onCreate();
}

void SGameSinglePlayer::onSwitchBoard(const EventArgs & eventInfo) {
	m_myColour = m_myColour == PieceColour::Black ? PieceColour::White : PieceColour::Black;
	m_boardManager->resetGame(m_myColour);

	if (m_myColour == PieceColour::Black)
		m_chessEngine->requestMove(m_boardManager->getFENFormat());
}


