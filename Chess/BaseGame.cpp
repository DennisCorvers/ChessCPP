#include "pch.h"
#include "BaseGame.h"
#include "BoardManager.h"
#include "StateManager.h"
#include "EventManager.h"

BaseGame::BaseGame(StateManager & stateManager, States state) :
	BaseState(stateManager),
	m_gameState(state)
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

	EventManager* eventManager = stateManager.getContext().eventManager;
	eventManager->addCallback(States::Pause, "onResetBoard", &BaseGame::onResetBoard, this);
	eventManager->addCallback(States::Pause, "onSwitchBoard", &BaseGame::onSwitchBoard, this);
	eventManager->addCallback(States::Pause, "onQuitGame", &BaseGame::onQuitGame, this);
}

BaseGame::~BaseGame()
{
	m_stateManager->getContext().eventManager->removeCallbacks(States::Pause);
}

void BaseGame::loadAssets()
{
	auto* textureManager = m_stateManager->getContext().textureManager;
	textureManager->requireResource(m_gameState, AssetNames::t_board);
	textureManager->requireResource(m_gameState, AssetNames::t_pieces);
}

void BaseGame::render() {
	m_boardManager->render(*m_window);
}

bool BaseGame::update(float deltaTime)
{
	sf::Vector2f position = EventManager::GetPixelPosition(*m_window, m_view);
	m_boardManager->updateMousePosition(position);

	m_boardManager->update(deltaTime);
	return true;
}

bool BaseGame::handleEvent(const sf::Event & event)
{
	if (event.type == sf::Event::Resized)
		Graphics::applyResize(m_view, event);

	return false;
}




