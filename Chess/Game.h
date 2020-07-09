#pragma once
#include "SFML/Graphics.hpp"

#ifdef NDEBUG 
#include "DebugOverlay.h"
#endif


struct SharedContext;
class TextureManager;
class FontManager;
class StateManager;

class DebugOverlay;

class Game {

private:
	const int windowWidth = 1280;
	const int windowHeight = 720;

	sf::Clock m_clock;
	float m_deltaTime;

	std::unique_ptr<SharedContext> m_context;
	std::unique_ptr<sf::RenderWindow> m_window;

	std::unique_ptr<StateManager> m_stateManager;
	std::unique_ptr<TextureManager> m_textureManager;
	std::unique_ptr<FontManager> m_fontManager;


	void initWindow();
	void registerStates();

public:
	Game();
	~Game();

	void update();
	void render();
	void lateUpdate();

	bool isRunning() const;

	sf::Sprite m_backdrop;

	//Debug Info
#ifdef NDEBUG
private:
	DebugOverlay m_debugOverlay;
#endif
};



