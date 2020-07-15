#include "pch.h"
#include "SGame.h"
#include "EventManager.h"
#include "BoardManager.h"
#include "BaseState.hpp"
#include "StateManager.h"
#include "TextureManager.hpp"

void SGame::loadAssets()
{
	auto* textureManager = m_stateManager->getContext().textureManager;
	textureManager->requireResource(States::Sandbox, AssetNames::t_board);
	textureManager->requireResource(States::Sandbox, AssetNames::t_pieces);
}

SGame::SGame(StateManager& stateManager)
	:BaseState(stateManager)
{
	m_window = stateManager.getContext().window;
	loadAssets();

	m_boardManager = std::unique_ptr<BoardManager>(
		new BoardManager(
			*stateManager.getContext().textureManager,
			*stateManager.getContext().soundManager,
			static_cast<int>(m_window->getSize().y * .85f)
		));

	m_view.setSize(sf::Vector2f(m_window->getSize().x, m_window->getSize().y));
	m_view.setCenter(m_boardManager->getBoardCenter());
}

SGame::~SGame()
{}

void SGame::onCreate() {
	m_boardManager->resetGame();
}

void SGame::onDestroy() {
}

void SGame::activate() {
}

void SGame::deactivate() {
}

void SGame::render() {

	m_boardManager->render(*m_window);
}

bool SGame::update(float deltaTime)
{
	sf::Vector2f position = EventManager::GetPixelPosition(*m_window, m_view);
	m_boardManager->updateMousePosition(position);

	if (!m_moveBuffer.empty()) {
		//Animate only enemy moves.
		m_boardManager->inputMove(m_moveBuffer.front(), false);
		m_moveBuffer.pop();
	}

	m_boardManager->update(deltaTime);
	return true;
}

bool SGame::handleEvent(const sf::Event & event)
{
	EType eType = event.type;
	if (eType == sf::Event::Resized)
		Graphics::applyResize(m_view, m_window->getSize().x, m_window->getSize().y);


	if (event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window), m_view);

		if (eType == sf::Event::MouseButtonPressed)
			m_boardManager->startSelection(mousePos, false);

		if (eType == sf::Event::MouseButtonReleased) {
			ChessMove newMove;
			if (m_boardManager->endSelection(mousePos, newMove))
				m_moveBuffer.push(newMove);
		}
	}

	if (eType == EType::KeyReleased && event.key.code == sf::Keyboard::R) {
		m_boardManager->resetGame();
	}

	if (eType == EType::KeyReleased && event.key.code == sf::Keyboard::F) {
		m_boardManager->flipBoard();
	}

	if (eType == EType::KeyReleased && event.key.code == sf::Keyboard::Escape) {
		m_stateManager->switchState(States::Pause);
	}

	return true;
}

