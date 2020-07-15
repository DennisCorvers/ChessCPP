#pragma once
#include "BaseGame.h"

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
};

