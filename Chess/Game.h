#pragma once

#include "SFML/Graphics.hpp"
#include "State.h"
#include "EventManager.h"

class Game {

private:
	const int windowWidth = 1280;
	const int windowHeight = 720;

	StateData m_stateData;
	sf::RenderWindow* m_window;

	sf::Clock m_clock;
	float m_deltaTime;

	State* m_state;
	EventManager* m_eventManager;

	void initWindow();
	void initStateDate();


public:
	Game();
	~Game();

	void update();
	void render();
	void lateUpdate();

	void run();

	float FPS() const;
};


