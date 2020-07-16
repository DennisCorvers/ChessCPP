#pragma once
#include "BaseGame.h"

class URIConnector;
class EventArgs;
class SGameSinglePlayer : public BaseGame
{
private:
	std::unique_ptr<URIConnector> m_stockfish;

public:
	SGameSinglePlayer(StateManager& stateManager);
	virtual ~SGameSinglePlayer();

	virtual void onCreate() override;
	virtual void onDestroy() override;

	virtual void activate() override;
	virtual void deactivate() override;

	virtual bool update(float deltaTime) override;
	virtual bool handleEvent(const sf::Event & event) override;

	// Inherited via BaseGame
	virtual void onResetBoard(const EventArgs & eventInfo) override;
	virtual void onSwitchBoard(const EventArgs & eventInfo) override {};
	virtual void onQuitGame(const EventArgs & eventInfo) override;
};
