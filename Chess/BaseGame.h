#pragma once
#include "BaseState.hpp"

enum struct GameState {
	GameOver,
	Playing,
	None
};

struct ChessMove;
class BoardManager;
class GuiContainer;
class GuiPauseMenu;
class GuiGameOver;
class BaseGame : public BaseState
{
private:
	States m_myState;
	std::shared_ptr<GuiPauseMenu> m_pauseMenu;
	std::shared_ptr<GuiGameOver> m_gameOverScreen;

protected:
	using EType = sf::Event::EventType;

	std::unique_ptr<GuiContainer> m_gui;
	std::unique_ptr<BoardManager> m_boardManager;
	GameState m_gameState;

	void loadAssets();

	virtual void onResetBoard() = 0;
	virtual void onSwitchBoard() = 0;
	virtual void onQuitGame() = 0;
	virtual bool onEvent(const sf::Event& event) = 0;

	bool inputMove(const ChessMove& move, bool validateMove, bool animate);

public:
	BaseGame(StateManager& stateManager, States state);
	~BaseGame();

	virtual void onCreate() override = 0;
	virtual void onDestroy() override = 0;

	virtual void activate() override = 0;
	virtual void deactivate() override = 0;

	virtual void render();
	virtual bool update(float deltaTime) override;

private:
	bool handleEvent(const sf::Event & event) override;
	void endGame(ActionType gameResult);
};
