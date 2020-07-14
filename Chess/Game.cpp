#include "pch.h"
#include "Game.h"
#include "SGame.h"
#include "SMainMenu.h"
#include "SPauseMenu.h"
#include "States.h"

#include "StateManager.h"
#include "SharedContext.h"

#include "ResourceManagers.hpp"
#include "SoundManager.hpp"

#include "DebugOverlay.hpp"

Game::Game()
{
	m_context = std::make_unique<SharedContext>();
	m_stateManager = std::make_unique<StateManager>(*m_context);

	m_textureManager = std::make_unique<MyTextureManager>(false);
	m_fontManager = std::make_unique<MyFontManager>();
	m_audioManager = std::make_unique<MyAudioManager>();
	m_soundManager = std::make_unique<SoundManager>(*m_audioManager);
	m_themeManager = std::make_unique<ThemeManager>();
	m_debugOverlay = std::make_unique<DebugOverlay>();


	initWindow();


	m_context->window = m_window.get();
	m_context->textureManager = m_textureManager.get();
	m_context->fontManager = m_fontManager.get();
	m_context->soundManager = m_soundManager.get();
	m_context->themeManager = m_themeManager.get();


	initUI();

	registerStates();
	m_stateManager->switchState(States::MainMenu);
}

Game::~Game() {
	m_deltaTime = 0;

	//Finalizes all states
	m_stateManager.reset();
	m_soundManager.reset();
}

void Game::initWindow()
{
	m_defaultView = std::make_unique<sf::View>(
		sf::FloatRect(0, 0, DEFAULT_SIZE.x, DEFAULT_SIZE.y));

	m_window = std::make_unique<sf::RenderWindow>(
		sf::VideoMode(DEFAULT_SIZE.x, DEFAULT_SIZE.y, 0),
		"Chess",
		sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize,
		sf::ContextSettings(0, 0, 0));

	m_window->setView(*m_defaultView);

	m_window->setFramerateLimit(FPS_LIMIT);
	//m_window->setVerticalSyncEnabled(true);
}

void Game::initUI()
{
	//Set background application-wide
	auto& bgtx = *m_textureManager->requireAndGet(AssetNames::t_background);
	m_backdrop.setTexture(bgtx, true);
	auto& view = m_window->getView();
	m_backdrop.setScale(
		view.getSize().x / m_backdrop.getLocalBounds().width,
		view.getSize().y / m_backdrop.getLocalBounds().height
	);
}

void Game::registerStates() {
	m_stateManager->registerState<SGame>(States::Sandbox);
	m_stateManager->registerState<SMainMenu>(States::MainMenu);
	m_stateManager->registerState<SPauseMenu>(States::Pause);
}

void Game::update()
{
	if (m_stateManager->isEmpty())
		m_window->close();

	sf::Event event;
	while (m_window->pollEvent(event)) {

		switch (event.type) {

		case sf::Event::Closed:
		{
			m_window->close();
			break;
		}
		case sf::Event::Resized:
		{
			auto newSize = Graphics::clampWindow(*m_window, MIN_SIZE);
			Graphics::applyResize(*m_defaultView, newSize.x, newSize.y);
			break;
		}
		case sf::Event::LostFocus:
		{
			m_window->setFramerateLimit(FPS_INACTIVE);
			continue;
		}
		case sf::Event::GainedFocus:
		{
			m_window->setFramerateLimit(FPS_LIMIT);
			continue;
		}
		}

		m_stateManager->handleEvent(event);
	}

	m_stateManager->update(m_deltaTime);
	m_soundManager->update(m_deltaTime);

#ifdef NDEBUG
	m_debugOverlay->update(m_deltaTime);
#endif
}

void Game::render()
{
	m_window->clear(sf::Color::Black);

	m_window->setView(*m_defaultView);
	m_window->draw(m_backdrop);

	m_stateManager->render();

#ifdef NDEBUG
	m_window->setView(*m_defaultView);
	m_debugOverlay->draw(*m_window);
#endif
	m_window->display();
}

void Game::lateUpdate()
{
	m_stateManager->lateUpdate();
	m_deltaTime = m_clock.restart().asSeconds();
}

bool Game::isRunning() const {
	return m_window->isOpen();
}


