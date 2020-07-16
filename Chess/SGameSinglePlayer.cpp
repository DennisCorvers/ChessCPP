#include "pch.h"
#include "SGameSinglePlayer.h"
#include "URIConnector.hpp"
#include "BoardManager.h"
#include "StateManager.h"

SGameSinglePlayer::SGameSinglePlayer(StateManager & stateManager) :
	BaseGame(stateManager, States::SinglePlayer)
{
	auto engineInfo = EngineInformation();
	m_stockfish = std::make_unique<URIConnector>("stockfish.exe", engineInfo);
}

SGameSinglePlayer::~SGameSinglePlayer()
{}

void SGameSinglePlayer::onCreate() {
	m_boardManager->resetGame();
	//m_stockfish->resetGame();
	m_boardManager->getFENFormat();
}

void SGameSinglePlayer::onDestroy() {
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

	m_stockfish->update(deltaTime);
	return false;
}

bool SGameSinglePlayer::handleEvent(const sf::Event & event)
{
	BaseGame::handleEvent(event);

	if (event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window), m_view);

		if (event.type == sf::Event::MouseButtonPressed)
			m_boardManager->startSelection(mousePos, PieceColour::White);

		if (event.type == sf::Event::MouseButtonReleased) {
			ChessMove newMove;
			if (m_boardManager->endSelection(mousePos, newMove))
				m_boardManager->inputMove(newMove, false, false);
		}
	}

	if (event.type == EType::KeyReleased && event.key.code == sf::Keyboard::R) {
		m_boardManager->resetGame();
	}

	if (event.type == EType::KeyReleased && event.key.code == sf::Keyboard::F) {
		m_boardManager->flipBoard();
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
	m_boardManager->resetGame();
	m_stockfish->resetGame();
}


