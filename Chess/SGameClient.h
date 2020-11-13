#pragma once
#include "BaseGame.h"
class NetClient;

class SGameClient : public BaseGame
{
private:
	NetClient& m_client;

public:
	SGameClient(StateManager& stateManager);
	virtual ~SGameClient();

	virtual void onCreate() override;
	virtual void onDestroy() override {};

	virtual void activate() override {};
	virtual void deactivate() override {};

	virtual bool update(float deltaTime) override;
	virtual bool onEvent(const sf::Event & event) override;

	// Inherited via BaseGame
	virtual void onResetBoard() override;
	virtual void onSwitchBoard() override;
	virtual void onQuitGame() override;
};