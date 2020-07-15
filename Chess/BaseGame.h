#pragma once
#include "BaseState.hpp"

class BoardManager;
class BaseGame : public BaseState
{
protected:
	using EType = sf::Event::EventType;

	sf::RenderWindow* m_window;
	std::unique_ptr<BoardManager> m_boardManager;

	void loadAssets();

public:
	BaseGame(StateManager& stateManager);
	~BaseGame();

	virtual void onCreate() override;
	virtual void onDestroy() override;

	virtual void activate() override;
	virtual void deactivate() override;

	virtual void render() override;
	virtual bool update(float deltaTime) override;
	virtual bool handleEvent(const sf::Event & event) override;
};
