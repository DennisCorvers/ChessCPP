#include "pch.h"
#include "SGameSandbox.h"
#include "EventManager.h"
#include "BoardManager.h"
#include "BaseState.hpp"
#include "StateManager.h"
#include "TextureManager.hpp"

SGameSandbox::SGameSandbox(StateManager& stateManager)
	:BaseGame(stateManager, States::Sandbox)
{
	m_gameState = GameState::Playing;
}

SGameSandbox::~SGameSandbox()
{}

void SGameSandbox::onCreate() {
	m_boardManager->resetGame();
}

bool SGameSandbox::onEvent(const sf::Event & event)
{
	if (event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mousePos = getWindow().mapPixelToCoords(sf::Mouse::getPosition(getWindow()), m_view);

		if (event.type == sf::Event::MouseButtonPressed)
			m_boardManager->startSelection(mousePos, false);

		if (event.type == sf::Event::MouseButtonReleased) {
			ChessMove newMove;
			if (m_boardManager->endSelection(mousePos, newMove)) {
				m_boardManager->inputMove(newMove, false, false);
			}
		}
	}

	return false;
}

void SGameSandbox::onResetBoard() {
	m_boardManager->resetGame();
}

void SGameSandbox::onSwitchBoard() {
	m_boardManager->flipBoard();
}

