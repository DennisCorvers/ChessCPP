#pragma once
#include "BaseGame.h"

namespace UCI { class UCIConnector; }
class EventArgs;
class SGameSinglePlayer : public BaseGame
{
private:
	std::unique_ptr<UCI::UCIConnector> m_chessEngine;

	PieceColour m_myColour;

public:
	SGameSinglePlayer(StateManager& stateManager);
	virtual ~SGameSinglePlayer();

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
