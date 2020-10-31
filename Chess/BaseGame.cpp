#include "pch.h"
#include "BaseGame.h"
#include "BoardManager.h"
#include "GuiPauseMenu.h"
#include "GuiContainer.hpp"
#include "EventManager.h"
#include "ResourceManager.hpp"
#include "GuiGameOver.h"


BaseGame::BaseGame(StateManager& stateManager, States state) :
	BaseState(stateManager),
	m_myState(state),
	m_gameState(GameState::None)
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
	m_pauseMenu->OnNewGameEvent.connect(&BaseGame::onResetBoard, this);
	m_pauseMenu->OnExitGameEvent.connect(&BaseGame::onQuitGame, this);
	m_pauseMenu->OnSwapColourEvent.connect(&BaseGame::onSwitchBoard, this);
	m_gui->addWindow(m_pauseMenu);

	m_gameOverScreen = std::make_shared<GuiGameOver>(stateManager.getContext());
	m_gui->addWindow(m_gameOverScreen);
}

BaseGame::~BaseGame()
{ }

void BaseGame::loadAssets()
{
	auto* textureManager = m_stateManager->getContext().textureManager;
	textureManager->requireResource(m_myState, AssetNames::t_board);
	textureManager->requireResource(m_myState, AssetNames::t_pieces);
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

	if (!m_gui->handleEvent(event)) {
		if (event.type == EType::KeyReleased && event.key.code == sf::Keyboard::Escape) {
			m_pauseMenu->showDialog();
		}

		if (!m_pauseMenu->isVisible())
			return onEvent(event);
	}

	return false;
}

bool BaseGame::inputMove(const ChessMove& move, bool validateMove, bool animate)
{
	bool succes = false;
	succes = m_boardManager->inputMove(move, validateMove, animate);

	ActionType lastAction = m_boardManager->getLastAction();

	if (lastAction & ActionType::Checkmate) {
		endGame(ActionType::Checkmate);
	}

	if (lastAction & ActionType::Stalemate) {
		endGame(ActionType::Stalemate);
	}

	if (lastAction & ActionType::Draw) {
		endGame(ActionType::Draw);
	}

	return succes;
}

void BaseGame::endGame(ActionType gameResult)
{
	m_gameState = GameState::GameOver;


	switch (gameResult) {
	case ActionType::Checkmate: {
		std::string winningColour = m_boardManager->getPlayingColour() == PieceColour::Black ? "White" : "Black";
		m_gameOverScreen->setText(winningColour + " won by Checkmate.");
		break;
	}
	case ActionType::Stalemate: {
		m_gameOverScreen->setText("Game ended in a Stalemate");
		break;
	}
	case ActionType::Draw: {
		m_gameOverScreen->setText("Game ended in a Draw.");
		break;
	}
	default: return;
	}

	m_gameOverScreen->showDialog();
}

