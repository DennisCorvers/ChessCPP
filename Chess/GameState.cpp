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

GameState::GameState()
{
	initTextures();
	initFonts();

	initGame();
}


GameState::~GameState()
{
	delete m_board;
}

void GameState::initKeybinds()
{
}

void GameState::updateInput(const float & deltaTime)
{
}

void GameState::update(const float & deltaTime)
{
}

void GameState::render(sf::RenderTarget * target)
{
	if (m_board->isEnabled)
		m_board->render(target);
}
