#pragma once
#include "BaseState.h"
#include "StateManager.h"

class SMainMenu : public BaseState
{
public:
	SMainMenu(StateManager& stateManager);
	virtual ~SMainMenu();

	virtual void onCreate() override;

	virtual void onDestroy() override;

	virtual void activate() override;

	virtual void deactivate() override;

	virtual void render() override;

	virtual bool update(float deltaTime) override;

	virtual bool handleEvent(const sf::Event & event) override;

private:
	tgui::Gui m_gui;

	void initializeUI();
};

