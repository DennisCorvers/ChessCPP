#pragma once
#include "BaseGame.h"

class EventArgs;
class SGameSandbox : public BaseGame
{

public:
	SGameSandbox(StateManager& stateManager);
	~SGameSandbox();

	virtual void onCreate() override;
	virtual void onDestroy() override;

	virtual void activate() override;
	virtual void deactivate() override;

	virtual bool onEvent(const sf::Event& event) override;

	virtual void onResetBoard() override;
	virtual void onSwitchBoard() override;
	virtual void onQuitGame() override;
};

