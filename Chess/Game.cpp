#include "Game.h"
#include "GameState.h"

float Game::FPS() const {
	return 1 / m_deltaTime;
}

Game::Game()
{
	initVariables();
	initWindow();

	
	initStateDate();

	m_state = new GameState(&m_stateData);
}

Game::~Game() {
	delete m_window;
	delete m_eventManager;
	delete m_state;
}

void Game::initVariables()
{
	m_window = NULL;
	m_deltaTime = 0.f;
}

void Game::initWindow()
{
	m_window = new sf::RenderWindow(
		sf::VideoMode(windowWidth, windowHeight, 32),
		"Chess",
		sf::Style::Titlebar | sf::Style::Close);

	m_window->setFramerateLimit(60);
	m_window->setVerticalSyncEnabled(true);

	m_eventManager = new EventManager();
}

void Game::initStateDate()
{
	m_stateData.window = m_window;
	m_stateData.eventManager = m_eventManager;
}

void Game::update() {

	sf::Event event;
	while (m_window->pollEvent(event)) {

		switch (event.type) {

		case sf::Event::Closed:
			m_window->close();
			break;
		}

		m_eventManager->handleEvent(event);
	}
	m_eventManager->update();
	m_state->update(m_deltaTime);
}

void Game::render() {
	m_window->clear(sf::Color::Black);

	m_state->render(m_window);

	m_window->display();
}

void Game::lateUpdate() {

	m_deltaTime = m_clock.restart().asSeconds();
}

void Game::run()
{
	m_deltaTime = 0;

	while (m_window->isOpen()) {
		update();
		render();
		lateUpdate();
	}
}



