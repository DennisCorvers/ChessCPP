#pragma once
#include "SFML/Graphics.hpp"

struct SharedContext;
class StateManager;

class MyTextureManager;
class MyFontManager;
class MyAudioManager;
class SoundManager;
class EventManager;

class DebugOverlay;

class Game {

private:
	const int FPS_LIMIT = 60;
	const int FPS_INACTIVE = 15;
	const sf::Vector2i DEFAULT_SIZE = sf::Vector2i(1920, 1027);
	const sf::Vector2i MIN_SIZE = sf::Vector2i(DEFAULT_SIZE.x / 2, DEFAULT_SIZE.y / 2);

	sf::Clock m_clock;
	float m_deltaTime;

	std::unique_ptr<SharedContext> m_context;
	std::unique_ptr<sf::RenderWindow> m_window;

	std::unique_ptr<StateManager> m_stateManager;
	//std::unique_ptr<EventManager> m_eventManager;

	//RESOURCE MANAGERS
	std::unique_ptr<MyTextureManager> m_textureManager;
	std::unique_ptr<MyFontManager> m_fontManager;
	std::unique_ptr<MyAudioManager> m_audioManager;
	//std::unique_ptr<SoundManager> m_soundManager;

	void initWindow();
	void initUI();
	void registerStates();

public:
	Game();
	~Game();

	void update();
	void render();
	void lateUpdate();

	bool isRunning() const;

	sf::Sprite m_backdrop;
	std::unique_ptr<sf::View> m_defaultView;
	std::unique_ptr<DebugOverlay> m_debugOverlay;
};



