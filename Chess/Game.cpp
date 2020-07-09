#include "pch.h"
#include "Game.h"
#include "SGame.h"
#include "SMainMenu.h"
#include "States.h"

#include "StateManager.h"
#include "FontManager.h"
#include "TextureManager.h"
#include "SharedContext.h"


Game::Game()
{
	m_context = std::make_unique<SharedContext>();
	m_stateManager = std::make_unique<StateManager>(*m_context);
	m_textureManager = std::make_unique<TextureManager>(true);
	m_fontManager = std::make_unique<FontManager>();

	initWindow();
	registerStates();

	m_context->window = m_window.get();
	m_context->textureManager = m_textureManager.get();
	m_context->fontManager = m_fontManager.get();

	//Set background application-wide
	auto& bgtx = *m_textureManager->aquireAndGet(States::MainMenu, AssetFlags::t_background, "Assets\\Sprites\\backdrop.jpg");
	bgtx.setSmooth(false);
	m_backdrop.setTexture(bgtx, true);
	auto& view = m_window->getView();
	m_backdrop.setScale(
		view.getSize().x / m_backdrop.getLocalBounds().width,
		view.getSize().y / m_backdrop.getLocalBounds().height
	);

	m_stateManager->switchState(States::Sandbox);
}

Game::~Game() {
	m_deltaTime = 0;

	//Force correct order of destruction
	m_stateManager.reset();
	m_textureManager.reset();
	m_fontManager.reset();

	m_window.reset();
	m_context.reset();
}

void Game::initWindow()
{
	m_window = std::make_unique<sf::RenderWindow>(
		sf::VideoMode(windowWidth, windowHeight, 0),
		"Chess",
		sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize,
		sf::ContextSettings(0, 0, 0));

	//m_window.setFramerateLimit(60);
	m_window->setVerticalSyncEnabled(true);
}

void Game::registerStates() {
	m_stateManager->registerState<SGame>(States::Sandbox);
	m_stateManager->registerState<SMainMenu>(States::MainMenu);
}

void Game::update()
{
	if (m_stateManager->isEmpty())
		m_window->close();

	sf::Event event;
	while (m_window->pollEvent(event)) {

		switch (event.type) {

		case sf::Event::Closed:
			m_window->close();
			break;
		}

		m_stateManager->handleEvent(event);
	}

	m_stateManager->update(m_deltaTime);


#ifdef NDEBUG
	m_debugOverlay.update(m_deltaTime);
#endif
}

void Game::render()
{
	m_window->clear(sf::Color::Black);

	m_window->setView(m_window->getDefaultView());
	m_window->draw(m_backdrop);

	m_stateManager->render();

#ifdef NDEBUG
	m_debugOverlay.draw(*m_window);
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


