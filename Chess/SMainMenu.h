#pragma once
#include "BaseState.hpp"

class GuiMainMenu;
class GuiContainer;
class SMainMenu : public BaseState
{
private:
	ConnectionContainer m_conContainer;
	std::unique_ptr<GuiContainer> m_gui;
	std::shared_ptr<GuiMainMenu> m_guiMainMenu;
	

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
	void onQuitPressed();
	void onSinglePlayerPressed();
	void onJoinGamePressed();
	void onHostGamePressed();
	void onSandboxPressed();
};

