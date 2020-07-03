#pragma once
#include "SFML/Graphics.hpp"
#include "StateManager.h"
#include "EventManager.h"
#include "SharedContext.h"

class Game {

private:
	const int windowWidth = 1280;
	const int windowHeight = 720;

	sf::Clock m_clock;
	float m_deltaTime;

	SharedContext m_context;
	sf::RenderWindow m_window;

	StateManager m_stateManager;
	EventManager m_eventManager;

	void initWindow();
	void registerStates();

public:
	Game();
	~Game();

	void update();
	void render();
	void lateUpdate();

	float getFPS() const;
	const sf::RenderWindow& getWindow() const;
};



