#include "GameState.h"
#include "ChessBoard.h"


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
	m_board = new ChessBoard(sf::Vector2f(28, 27), textures);
}

void GameState::initView()
{
	view.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
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

void GameState::initKeybinds()
{
}

void GameState::updateView(const float & deltaTime)
{	}

void GameState::updateInput(const float & deltaTime)
{	}

void GameState::update(const float & deltaTime)
{	}

void GameState::render(sf::RenderTarget * target)
{
	if (m_board->isEnabled)
		m_board->render(target);

	target->setView(view);
}
