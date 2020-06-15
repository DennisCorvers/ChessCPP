#include "BoardManager.h"
#include "ChessPieceManager.h"
#include "ChessBoard.h"

BoardManager::BoardManager(const sf::FloatRect boardSizes, std::map<std::string, sf::Texture>& textures)
{
	m_board = new ChessBoard(BoardSettings::DEFAULTBOARD);
	m_pieceManager = new ChessPieceManager(boardSizes, textures);
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
	return m_pieceManager->getCenter();
}

void BoardManager::resetGame()
{
	m_board->resetBoard();
	m_pieceManager->syncPieces(*m_board, false);
}

bool BoardManager::inputMove(const ChessMove move, bool animate)
{
	bool moveApplied = m_board->inputMove(move, &m_positionCache);
	if (moveApplied) {
		m_pieceManager->syncPieces(*m_board, animate);
	}

	return moveApplied;
}

void BoardManager::reverseMove(const ChessMove move, bool animate)
{

}

void BoardManager::startSelection(const sf::Vector2f screenPosition, PieceColour playerColour)
{
	if (!m_pieceManager->boundsContains(screenPosition.x, screenPosition.y))
		return;

	auto pos = m_pieceManager->screenToBoard(screenPosition);
	ChessPosition newPos(pos.x, pos.y);

	//if (!m_board->isValidSelection(newPos, playerColour))
	//	return;

	m_positionCache = m_board->getValidPositions(newPos);
	m_pieceManager->startSelection(screenPosition, m_positionCache);
	return;
}

void BoardManager::updateMousePosition(const sf::Vector2f screenPosition)
{
	m_pieceManager->updateSelection(screenPosition);
}

void BoardManager::endSelection(const sf::Vector2f screenPosition)
{
	ChessMove newMove;
	if (m_pieceManager->endSelection(screenPosition, newMove)) {
		if (inputMove(newMove, false)) {
			//Store correct move
		}
	}
}

void BoardManager::update(const float & deltaTime)
{
	m_pieceManager->update(deltaTime);
}

void BoardManager::render(sf::RenderTarget * const target)
{
	m_pieceManager->render(target);
}
