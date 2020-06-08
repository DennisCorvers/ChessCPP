#include "GameState.h"
#include "ChessBoard.h"
#include "EventManager.h"

void GameState::initTextures()
{
	if (!textures["BOARD"].loadFromFile("Assets\\Sprites\\Board.png"))
		throw std::exception("Unable to load Board texture");

	if (!textures["PIECES"].loadFromFile("Assets\\Sprites\\Pieces.png"))
		throw std::exception("Unable to load Pieces texture");
}

void GameState::initFonts()
{
	if (!m_font.loadFromFile("Assets\\Fonts\\OpenSans-Regular.ttf"))
		throw std::exception("Unable to load OpenSans-Regular font!");
}

void GameState::initGame()
{
	sf::FloatRect boardSizes(28, 28, 56, 56);
	m_board = new ChessBoard(boardSizes, textures);
	m_board->resetBoard();
}

void GameState::initView()
{
	view.setSize(sf::Vector2f(stateData->window->getSize().x, stateData->window->getSize().y));
	//view.setCenter(sf::Vector2f(window->getSize().x / 2.f, window->getSize().y / 2.f));
	view.setCenter(m_board->getCenter()); //Focus on the centre of the board.
}

GameState::GameState(StateData* data)
	: State(data)
{
	initTextures();
	initFonts();

	initGame();
	initView();
}

GameState::~GameState()
{
	delete m_board;
}

void GameState::updateView(const float & deltaTime)
{	}

void GameState::updateInput(const float & deltaTime)
{
	MyEvent nextEvent;
	while (stateData->eventManager->pollEvent(&nextEvent)) {

		if (nextEvent.keyCode == sf::Mouse::Left) {
			sf::Vector2f mousePos = EventManager::GetPixelPosition(*stateData->window, view);
			if (nextEvent.eventType == MyEventType::MouseDown) {
				m_board->updateMousePosition(mousePos, MouseEvent::MouseDown);
			}

			if (nextEvent.eventType == MyEventType::MouseUp) {
				m_board->updateMousePosition(mousePos, MouseEvent::MouseUp);
			}
		}

		if (nextEvent.eventType == MyEventType::KeyUp && nextEvent.keyCode == sf::Keyboard::R) {
			m_board->resetBoard();
		}
	}
}

void GameState::update(const float & deltaTime)
{
	updateInput(deltaTime);

	if (m_board->hasPieceSelected()) {
		sf::Vector2f position = EventManager::GetPixelPosition(*stateData->window, view);
		m_board->updateMousePosition(position, MouseEvent::MouseMove);
	}
}

void GameState::render(sf::RenderTarget * target)
{
	m_board->render(target);

	target->setView(view);
}
