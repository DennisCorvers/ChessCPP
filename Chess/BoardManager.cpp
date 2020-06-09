#include "BoardManager.h"
#include "ChessPieceManager.h"
#include "ChessBoard.h"

BoardManager::BoardManager(const sf::FloatRect boardSizes, std::map<std::string, sf::Texture>& textures)
{
	m_board = new ChessBoard(textures["BOARD"], BoardSettings::DEFAULTBOARD);
	m_pieceManager = new ChessPieceManager(boardSizes, textures["PIECES"]);
}


BoardManager::~BoardManager()
{
	delete m_board;
	delete m_pieceManager;
}

bool BoardManager::hasPieceSelected()
{
	return m_pieceManager->hasSelection();
}

sf::Vector2f BoardManager::getBoardCenter()
{
	return m_board->getCenter();
}

void BoardManager::resetGame()
{
	m_board->resetBoard();
	m_pieceManager->reset(m_board->getBoard());
}

sf::Vector2i BoardManager::screenToBoardPosition(const sf::Vector2f screenPosition)
{
	auto m_boardSizes = m_pieceManager->getBoardSizes();

	int xPos = (int)((screenPosition.x - m_boardSizes.left) / m_boardSizes.width);
	int yPos = (int)((screenPosition.y - m_boardSizes.top) / m_boardSizes.height);

	return sf::Vector2i(xPos, yPos);
}

void BoardManager::inputMove(const ChessMove move, bool animate)
{

}

void BoardManager::reverseMove(const ChessMove move, bool animate)
{

}

sf::Vector2i BoardManager::startSelection(const sf::Vector2f screenPosition, PieceColour playerColour)
{
	if (!m_pieceManager->boundsContains(screenPosition.x, screenPosition.y))
		return sf::Vector2i(-1, -1);

	auto pos = screenToBoardPosition(screenPosition);
	if (!m_board->isValidSelection(ChessPosition(pos.x, pos.y), playerColour))
		return sf::Vector2i(-1, -1);

	m_pieceManager->startSelection(screenPosition, *m_board);
	return sf::Vector2i(-1, -1);
}

void BoardManager::updateMousePosition(const sf::Vector2f screenPosition)
{
	m_pieceManager->updateSelection(screenPosition);
}

sf::Vector2i BoardManager::endSelection(const sf::Vector2f screenPosition)
{
	m_pieceManager->endSelection(screenPosition, *m_board);
	return sf::Vector2i(-1, -1);
}

void BoardManager::update(const float & deltaTime)
{
	m_pieceManager->update(deltaTime);
}

void BoardManager::render(sf::RenderTarget * const target)
{
	m_board->render(target);
	m_pieceManager->render(target);
}
