#include "pch.h"
#include "Game.h"
#include "SGame.h"
#include "SMainMenu.h"
#include "States.h"

const sf::RenderWindow& Game::getWindow() const {
	return m_window;
}

Game::Game() :
	m_stateManager(m_context)
{
	initWindow();
	registerStates();

	m_context.window = &m_window;

	m_stateManager.switchState(States::MainMenu);
}

Game::~Game() {
	m_deltaTime = 0;
}

void Game::initWindow()
{
	m_window.create(
		sf::VideoMode(windowWidth, windowHeight, 32),
		"Chess",
		sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize,
		sf::ContextSettings(0, 0, 16));

	//m_window.setFramerateLimit(60);
	m_window.setVerticalSyncEnabled(true);
}

void Game::registerStates() {
	m_stateManager.registerState<SGame>(States::Sandbox);
	m_stateManager.registerState<SMainMenu>(States::MainMenu);
}

void Game::update()
{
	if (m_stateManager.isEmpty())
		m_window.close();

	sf::Event event;
	while (m_window.pollEvent(event)) {

		switch (event.type) {

		case sf::Event::Closed:
			m_window.close();
			break;
		}

		m_stateManager.handleEvent(event);
	}

	m_stateManager.update(m_deltaTime);


#ifdef NDEBUG
	m_debugOverlay.update(m_deltaTime);
#endif
}

void Game::render()
{
	m_window.clear(sf::Color::Black);

	m_stateManager.render();

#ifdef NDEBUG
	m_debugOverlay.draw(m_window);
#endif

	m_window.display();
}

void Game::lateUpdate()
{
	m_stateManager.lateUpdate();
	m_deltaTime = m_clock.restart().asSeconds();
}


