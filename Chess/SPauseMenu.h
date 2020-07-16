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

private:

	sf::RectangleShape m_backdrop;

	void initializeUI();
	void createButtons(std::vector<tgui::Button::Ptr>& buttons);

	void onResumePressed();
	void onNewGamePressed();
	void onSwapColourPressed();
	void onQuitGamePressed();
};

