#pragma once
#include "SFML/Graphics.hpp"

struct SharedContext;
class StateManager;

class MyTextureManager;
class MyFontManager;
class MyAudioManager;
class SoundManager;
class EventManager;
class ThemeManager;

class NetClient;

class DebugOverlay;

class Game {

private:
	sf::Clock m_clock;
	float m_deltaTime;

	std::unique_ptr<SharedContext> m_context;
	std::unique_ptr<sf::RenderWindow> m_window;

	std::unique_ptr<StateManager> m_stateManager;
	std::unique_ptr<EventManager> m_eventManager;

	//RESOURCE MANAGERS
	std::unique_ptr<MyTextureManager> m_textureManager;
	std::unique_ptr<MyFontManager> m_fontManager;
	std::unique_ptr<MyAudioManager> m_audioManager;
	std::unique_ptr<SoundManager> m_soundManager;
	std::unique_ptr<ThemeManager> m_themeManager;

	//NET
	std::unique_ptr<NetClient> m_netClient;


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



