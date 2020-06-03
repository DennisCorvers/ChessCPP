#pragma once

#include "SFML/Graphics.hpp"
#include "State.h"

class Game {

private:
	const int windowWidth = 1024;
	const int windowHeight = 576;

	StateData m_stateData;
	sf::RenderWindow* m_window;

	sf::Clock m_clock;
	float m_deltaTime;

	State* m_state;

	void initVariables();
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


