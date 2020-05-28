#pragma once

#include "Window.h"
#include "Session.h"
#include "SpriteManager.h"

#include <iostream>

using namespace Graphics;

class Game {

private:
	Session* m_session;
	Window m_window;
	sf::Clock m_clock;
	sf::Time m_elapsed;
	SpriteManager* m_spriteManager;

public:
	Game();
	~Game();

	void update();
	void render();
	void lateUpdate();

	Window* GetWindow();
	float ElapsedTime() const;
	float FPS() const;
};


