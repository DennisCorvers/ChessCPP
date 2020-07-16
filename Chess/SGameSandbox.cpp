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

bool SGameSandbox::handleEvent(const sf::Event & event)
{
	BaseGame::handleEvent(event);

	if (event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window), m_view);

		if (event.type == sf::Event::MouseButtonPressed)
			m_boardManager->startSelection(mousePos, false);

		if (event.type == sf::Event::MouseButtonReleased) {
			ChessMove newMove;
			if (m_boardManager->endSelection(mousePos, newMove))
				m_boardManager->inputMove(newMove, false, false);
		}
	}

	if (event.type == EType::KeyReleased && event.key.code == sf::Keyboard::Escape) {
		m_stateManager->switchState(States::Pause);
	}

	return false;
}

void SGameSandbox::onQuitGame(const EventArgs & eventInfo) {
	m_stateManager->switchState(States::MainMenu);
	m_stateManager->removeState(States::Sandbox);
}

void SGameSandbox::onResetBoard(const EventArgs& eventInfo) {
	m_boardManager->resetGame();
}

void SGameSandbox::onSwitchBoard(const EventArgs& eventInfo) {
	m_boardManager->flipBoard();
}


