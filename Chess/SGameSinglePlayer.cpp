#include "pch.h"
#include "SGameSinglePlayer.h"

#include "UCIConnector.hpp"
#include "BoardManager.h"
#include "StateManager.h"
#include "GuiBotInput.hpp"


SGameSinglePlayer::SGameSinglePlayer(StateManager & stateManager) :
	BaseGame(stateManager, States::SinglePlayer),
	m_myColour(PieceColour::White)
{
	auto engineInfo = UCI::EngineInformation();
	m_chessEngine = std::make_unique<UCI::UCIConnector>("stockfish.exe", engineInfo);
	m_botLevelWindow = GuiBotInput::create(stateManager.getContext());
	m_botLevelWindow->setText("10");
	m_botLevelWindow->OnConfirm.connect(&SGameSinglePlayer::onBotLevelEntered, this);
	m_gui->addWindow(m_botLevelWindow);
}

SGameSinglePlayer::~SGameSinglePlayer()
{ }

void SGameSinglePlayer::onBotLevelEntered(int level)
{
	m_gameState = GameState::Playing;

	m_chessEngine->setSkillLevel(level);

	if (m_myColour == PieceColour::Black)
		m_chessEngine->requestMove(m_boardManager->getFENFormat());
}

void SGameSinglePlayer::onCreate() {
	m_boardManager->resetGame(m_myColour);
	m_chessEngine->resetGame();

	m_botLevelWindow->showDialog();
}

bool SGameSinglePlayer::update(float deltaTime)
{
	BaseGame::update(deltaTime);

	m_chessEngine->runEngine(deltaTime);
	if (m_chessEngine->pollEngine()) {
		auto message = m_chessEngine->getMessage();
		switch (message.messageType)
		{
		case UCI::EngineMessageType::BestMove:
			inputMove(ChessMove::fromChessNotation(message.message), false, true);
			break;
		default:
			break;
		}
	}
	return false;
}

bool SGameSinglePlayer::onEvent(const sf::Event & event)
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
					m_chessEngine->requestMove(m_boardManager->getFENFormat());
				}
			}
		}
	}

	return false;
}

void SGameSinglePlayer::onResetBoard() {
	onCreate();
}

void SGameSinglePlayer::onSwitchBoard() {
	m_myColour = m_myColour == PieceColour::Black ? PieceColour::White : PieceColour::Black;
	onCreate();
}

void SGameSinglePlayer::onQuitGame() {
	m_stateManager->switchState(States::MainMenu);
	m_stateManager->removeState(States::SinglePlayer);
}


