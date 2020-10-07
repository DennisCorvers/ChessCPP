#include "pch.h"
#include "BaseGame.h"
#include "BoardManager.h"
#include "GuiPauseMenu.h"
#include "GuiContainer.hpp"
#include "EventManager.h"
#include "ResourceManager.hpp"

BaseGame::BaseGame(StateManager& stateManager, States state) :
	BaseState(stateManager),
	m_gameState(state)
{
	loadAssets();

	m_boardManager = std::unique_ptr<BoardManager>(
		new BoardManager(
			*stateManager.getContext().textureManager,
			*stateManager.getContext().soundManager,
			static_cast<int>(m_view.getSize().y * .85f)
		));

	m_view.setCenter(m_boardManager->getBoardCenter());

	m_gui = std::make_unique<GuiContainer>(*stateManager.getContext().window);
	m_pauseMenu = std::make_shared<GuiPauseMenu>(stateManager.getContext());
	m_gui->addWindow(m_pauseMenu);

	//Bind events
	m_conNewGame = m_pauseMenu->OnNewGameEvent.connect(&BaseGame::onResetBoard, this);
	m_conQuitGame = m_pauseMenu->OnExitGameEvent.connect(&BaseGame::onQuitGame, this);
	m_conSwitchColour = m_pauseMenu->OnSwapColourEvent.connect(&BaseGame::onSwitchBoard, this);
}

BaseGame::~BaseGame()
{ }

void BaseGame::loadAssets()
{
	auto* textureManager = m_stateManager->getContext().textureManager;
	textureManager->requireResource(m_gameState, AssetNames::t_board);
	textureManager->requireResource(m_gameState, AssetNames::t_pieces);
}

void BaseGame::render() {
	m_boardManager->render(getWindow());
	m_gui->render();
}

bool BaseGame::update(float deltaTime)
{
	sf::Vector2f position = EventManager::GetPixelPosition(getWindow(), m_view);
	m_boardManager->updateMousePosition(position);

	m_boardManager->update(deltaTime);
	return true;
}

bool BaseGame::handleEvent(const sf::Event & event)
{
	if (event.type == sf::Event::Resized)
		Graphics::applyResize(m_view, event);

	if (event.type == EType::KeyReleased && event.key.code == sf::Keyboard::Escape) {
		if (m_pauseMenu->isVisible())
			m_pauseMenu->hide();
		else
			m_pauseMenu->show();
	}

	if (!m_gui->handleEvent(event))
		return onEvent(event);

	return false;
}




