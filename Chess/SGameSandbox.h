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

	virtual bool handleEvent(const sf::Event& event) override;

	virtual void onResetBoard(const EventArgs & eventInfo) override;
	virtual void onSwitchBoard(const EventArgs & eventInfo) override;
	virtual void onQuitGame(const EventArgs & eventInfo) override;
};

