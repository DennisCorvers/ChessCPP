#pragma once
#include "SFML/Graphics.hpp"
#include "StateManager.h"
#include "EventManager.h"
#include "SharedContext.h"
#include "DebugOverlay.h"

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
	TextureManager m_textureManager;

	void initWindow();
	void registerStates();

public:
	Game();
	~Game();

	void update();
	void render();
	void lateUpdate();


	inline float getFPS() const {
		return 1 / m_deltaTime;
	}
	const sf::RenderWindow& getWindow() const;

	//Debug Info
#ifdef NDEBUG
private:
	DebugOverlay m_debugOverlay;
#endif
};



