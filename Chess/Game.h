#pragma once

#include "SFML/Graphics.hpp"

class State;

class Game {

private:
	sf::RenderWindow* m_window;

	sf::Clock m_clock;
	float m_deltaTime;

	State* m_state;

	void initVariables();
	void initWindow();


public:
	Game();
	~Game();

	void update();
	void render();
	void lateUpdate();

	void run();

	float FPS() const;
};


