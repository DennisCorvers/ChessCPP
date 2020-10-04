#include "pch.h"
#include "SGameSandbox.h"
#include "EventManager.h"
#include "BoardManager.h"
#include "BaseState.hpp"
#include "StateManager.h"
#include "TextureManager.hpp"

SGameSandbox::SGameSandbox(StateManager& stateManager)
	:BaseGame(stateManager, States::Sandbox)
{}

SGameSandbox::~SGameSandbox()
{}

void SGameSandbox::onCreate() {
	m_boardManager->resetGame();
}

void SGameSandbox::onDestroy() {
}

void SGameSandbox::activate() {
}

void SGameSandbox::deactivate() {
}

bool SGameSandbox::onEvent(const sf::Event & event)
{
	if (event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window), m_view);

		if (event.type == sf::Event::MouseButtonPressed)
			m_boardManager->startSelection(mousePos, false);

		if (event.type == sf::Event::MouseButtonReleased) {
			ChessMove newMove;
			if (m_boardManager->endSelection(mousePos, newMove)) {
				m_boardManager->inputMove(newMove, false, false);
				m_boardManager->getFENFormat();
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

void SGameSandbox::onQuitGame() {
	m_stateManager->switchState(States::MainMenu);
	m_stateManager->removeState(States::Sandbox);
}

