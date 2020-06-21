#include "pch.h"
#include "GameState.h"
#include "EventManager.h"
#include "BoardManager.h"

void GameState::initTextures()
{
	if (!textures[AssetFlags::t_board].loadFromFile("Assets\\Sprites\\Board.png"))
		throw std::exception("Unable to load Board texture");

	if (!textures[AssetFlags::t_pieces].loadFromFile("Assets\\Sprites\\Pieces.png"))
		throw std::exception("Unable to load Pieces texture");
}

void GameState::initFonts()
{
	if (!m_font.loadFromFile("Assets\\Fonts\\OpenSans-Regular.ttf"))
		throw std::exception("Unable to load OpenSans-Regular font!");
}

void GameState::initSounds() {
	if (!sounds[AssetFlags::s_piece_check].loadFromFile("Assets\\Sounds\\piece_check.ogg"))
		throw std::exception("Unable to load piece_check.ogg");

	if (!sounds[AssetFlags::s_piece_move].loadFromFile("Assets\\Sounds\\piece_move.ogg"))
		throw std::exception("Unable to load piece_move.ogg");

	if (!sounds[AssetFlags::s_piece_take].loadFromFile("Assets\\Sounds\\piece_take.ogg"))
		throw std::exception("Unable to load piece_take.ogg");
}

void GameState::initGame()
{
	sf::FloatRect boardSizes(28, 28, 56, 56);
	m_boardManager = new BoardManager(boardSizes, textures, sounds);
	m_boardManager->resetGame();
}

void GameState::initView()
{
	float x = static_cast<float>(stateData->window->getSize().x);
	float y = static_cast<float>(stateData->window->getSize().y);

	view.setSize(sf::Vector2f(x, y));
	view.setCenter(m_boardManager->getBoardCenter()); //Focus on the centre of the board.
}

GameState::GameState(StateData* data)
	: State(data)
{
	initTextures();
	initFonts();
	initSounds();

	initGame();
	initView();
}

GameState::~GameState()
{
	delete m_boardManager;
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
				m_boardManager->startSelection(mousePos, PieceColour::Black);
			}

			if (nextEvent.eventType == MyEventType::MouseUp) {
				ChessMove newMove;
				if (m_boardManager->endSelection(mousePos, newMove)) {
					m_moveBuffer.push(newMove);
				}
			}
		}

		if (nextEvent.eventType == MyEventType::KeyUp && nextEvent.keyCode == sf::Keyboard::R) {
			m_boardManager->resetGame();
		}
	}
}

void GameState::update(const float & deltaTime)
{
	updateInput(deltaTime);

	sf::Vector2f position = EventManager::GetPixelPosition(*stateData->window, view);
	m_boardManager->updateMousePosition(position);

	if (!m_moveBuffer.empty()) {
		m_boardManager->inputMove(m_moveBuffer.front(), true);
		m_moveBuffer.pop();
	}
}

void GameState::render(sf::RenderTarget * target)
{
	m_boardManager->render(target);

	target->setView(view);
}
