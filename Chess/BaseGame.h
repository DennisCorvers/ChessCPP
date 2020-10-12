#pragma once
#include "BaseState.hpp"

class BoardManager;
class GuiContainer;
class GuiPauseMenu;
class BaseGame : public BaseState
{
private:
	States m_gameState;
	
	std::unique_ptr<GuiContainer> m_gui;
	std::shared_ptr<GuiPauseMenu> m_pauseMenu;

protected:
	using EType = sf::Event::EventType;

	std::unique_ptr<BoardManager> m_boardManager;

	void loadAssets();

	virtual void onResetBoard() = 0;
	virtual void onSwitchBoard() = 0;
	virtual void onQuitGame() = 0;
	virtual bool onEvent(const sf::Event& event) = 0;

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
};
