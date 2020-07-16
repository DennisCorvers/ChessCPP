#pragma once
#include "BaseMenu.hpp"

class SMainMenu : public BaseMenu
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
	void initializeUI();

	void onQuitPressed();
	void onSinglePlayerPressed();
	void onJoinGamePressed();
	void onHostGamePressed();
	void onSandboxPressed();
};

