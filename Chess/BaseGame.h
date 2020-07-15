#pragma once
#include "BaseState.hpp"

struct ChessMove;
class BoardManager;
class BaseGame : public BaseState
{
private:
	States m_gameState;

protected:
	using EType = sf::Event::EventType;

	sf::RenderWindow* m_window;
	std::unique_ptr<BoardManager> m_boardManager;

	void loadAssets();

public:
	BaseGame(StateManager& stateManager, States state);
	~BaseGame();

	virtual void onCreate() override = 0;
	virtual void onDestroy() override = 0;

	virtual void activate() override = 0;
	virtual void deactivate() override = 0;

	virtual void render();
	virtual bool update(float deltaTime) override;
	virtual bool handleEvent(const sf::Event & event) override;
};
