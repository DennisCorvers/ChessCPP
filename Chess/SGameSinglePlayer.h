#pragma once
#include "BaseGame.h"
#include "ChessColour.hpp"

namespace UCI { class UCIConnector; }

class GuiBotInput;
class SGameSinglePlayer : public BaseGame
{
private:
	std::unique_ptr<UCI::UCIConnector> m_chessEngine;
	std::shared_ptr<GuiBotInput> m_botLevelWindow;

	ChessColour m_myColour;

	void onBotLevelEntered(int level);

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
};
