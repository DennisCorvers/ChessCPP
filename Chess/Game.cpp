#include "Game.h"

Game::Game()
	: m_window("Chess", sf::Vector2u(500, 500))
{
	m_clock.restart();
	srand(time(nullptr));

	m_spriteManager = new SpriteManager();

	m_session = new Session();
	m_session->startSession();

	m_spriteManager->loadPiecesSprites(m_session->Board());
}

Game::~Game() {
	delete m_session;
	delete m_spriteManager;
}

Window* Game::GetWindow() {
	return &m_window;
}

float Game::ElapsedTime() const {
	return m_elapsed.asSeconds();
}

float Game::FPS() const {
	return 1 / m_elapsed.asSeconds();
}

void Game::update() {
	auto window = m_window.RenderWindow();
	sf::Event event;
	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

	while (window->pollEvent(event)) {


		switch (event.type) {

		case sf::Event::Closed:
			window->close();
			break;

		case sf::Event::MouseButtonPressed:

			break;

		case sf::Event::MouseButtonReleased:

			break;

		default:
			break;
		}

	}
}

void Game::render() {
	m_window.beginDraw();

	//Draw board
	m_spriteManager->drawSprites(m_window);

	m_window.endDraw();
}

void Game::lateUpdate() {

	m_elapsed = m_clock.restart();
}



