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
	BoardSizes sizes(28, 27, 56, 56);
	m_board = new ChessBoard(sizes, textures);
}

void GameState::initView()
{
	view.setSize(sf::Vector2f(stateData->window->getSize().x, stateData->window->getSize().y));
	//view.setCenter(sf::Vector2f(window->getSize().x / 2.f, window->getSize().y / 2.f));
	view.setCenter(m_board->getCenter()); //Focus on the centre of the board.
}

void GameState::startDragging(const MyEvent& nextEvent)
{
	sf::Vector2i posStart;
	ChessPiece* piece;
	sf::Vector2i mousePos(nextEvent.mousePos.x, nextEvent.mousePos.y);
	sf::Vector2f pixelPos = stateData->window->mapPixelToCoords(mousePos, view);

	if (m_board->tryScreenToBoard(pixelPos, &posStart)) {
		piece = m_board->getClickedPiece(pixelPos);
		if (piece) {
			m_moveAction.movingPiece = piece;
			m_moveAction.moveFrom = ChessPosition(posStart.x, posStart.y);
		}
	}
}

void GameState::stopDragging(const MyEvent& nextEvent)
{
	sf::Vector2i posEnd;
	sf::Vector2i mousePos(nextEvent.mousePos.x, nextEvent.mousePos.y);
	sf::Vector2f pixelPos = stateData->window->mapPixelToCoords(mousePos, view);

	if (m_board->tryScreenToBoard(pixelPos, &posEnd)
		&& m_moveAction.movingPiece) {
		m_moveAction.moveTo = ChessPosition(posEnd.x, posEnd.y);

		//TODO: Test move conditions, add to move queue/buffer...
		m_board->snapPieceToBoard(m_moveAction.moveTo, m_moveAction.movingPiece);
	}
	else {
		//Snap back
		m_board->snapPieceToBoard(m_moveAction.moveFrom, m_moveAction.movingPiece);
	}
	m_moveAction.movingPiece = NULL;
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
			if (nextEvent.eventType == MyEventType::MouseDown) {
				startDragging(nextEvent);
			}

			if (nextEvent.eventType == MyEventType::MouseUp) {
				if (m_moveAction.movingPiece)
					stopDragging(nextEvent);
			}
		}
	}
}

void GameState::update(const float & deltaTime)
{
	updateInput(deltaTime);

	//If there's a piece currently being dragged... animate it
	if (m_moveAction.movingPiece) {
		sf::Vector2i pos = sf::Mouse::getPosition(*stateData->window);
		sf::Vector2f pixelPos = stateData->window->mapPixelToCoords(pos, view);
		m_moveAction.movingPiece->setCenter(pixelPos.x, pixelPos.y);
	}
}

void GameState::render(sf::RenderTarget * target)
{
	if (m_board->isEnabled)
		m_board->render(target);

	//Render the moving piece at the very last, so that it appears on top.
	if (m_moveAction.movingPiece)
		m_moveAction.movingPiece->render(target);

	target->setView(view);
}
