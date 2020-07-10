#pragma once
#include "SFML/Graphics.hpp"

struct SharedContext;
class TextureManager;
class FontManager;
class StateManager;

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
	std::unique_ptr<TextureManager> m_textureManager;
	std::unique_ptr<FontManager> m_fontManager;


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



