#pragma once
#include "BaseMenu.hpp"

class SPauseMenu : public BaseMenu
{
public:
	SPauseMenu(StateManager& stateManager);
	virtual ~SPauseMenu();

	virtual void onCreate() override;

	virtual void onDestroy() override;

	virtual void activate() override;

	virtual void deactivate() override;

	virtual void render() override;

	virtual bool update(float deltaTime) override;

	virtual bool handleEvent(const sf::Event & event) override;

	void onPauseMenuOpen(States callingState);

private:

	States m_previousState;
	sf::RectangleShape m_backdrop;

	void initializeUI() override;
	void onResumePressed();
	void onNewGamePressed();
	void onSwapColourPressed();
	void onQuitGamePressed();
};

