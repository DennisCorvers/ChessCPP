#include "BoardManager.h"
#include "ChessPieceManager.h"
#include "ChessBoard.h"
#include "ChessAction.h"

BoardManager::BoardManager(const sf::FloatRect boardSizes,
	std::map<AssetFlags, sf::Texture>& textures,
	std::map <AssetFlags, sf::SoundBuffer>& sounds)
{
	m_board = new ChessBoard(BoardSettings::DEFAULTBOARD);
	m_pieceManager = new ChessPieceManager(boardSizes, textures);

	m_soundPieceCheck.setBuffer(sounds[AssetFlags::s_piece_check]);
	m_soundPieceMove.setBuffer(sounds[AssetFlags::s_piece_move]);
	m_soundPieceTake.setBuffer(sounds[AssetFlags::s_piece_take]);
}


BoardManager::~BoardManager()
{
	delete m_board;
	delete m_pieceManager;
}

bool BoardManager::isPieceMoving()
{
	return m_pieceManager->isPieceMoving();
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
	ChessAction moveResult = m_board->inputMove(move);
	if (moveResult.isValidMove()) {
		m_pieceManager->syncPieces(*m_board, animate);
		handleSound(moveResult, true);
	}

	return moveResult.isValidMove();
}

void BoardManager::handleSound(const ChessAction & chessAction, bool playSound)
{
	if (!playSound)
		return;

	switch (chessAction.actionType) {
	case ActionType::Check:
	case ActionType::Checkmate:
		m_soundPieceCheck.play();
		break;
	case ActionType::Castling:
	case ActionType::EnPassant:
	case ActionType::Normal:
		m_soundPieceMove.play();
		break;
	case ActionType::Take:
		m_soundPieceTake.play();
		break;
	default:
		break;
	}
}

void BoardManager::startSelection(const sf::Vector2f screenPosition, PieceColour playerColour) const
{
	m_pieceManager->startSelection(screenPosition, *m_board);
}

void BoardManager::updateMousePosition(const sf::Vector2f screenPosition) const
{
	m_pieceManager->updateSelection(screenPosition);
}

bool BoardManager::endSelection(const sf::Vector2f screenPosition, ChessMove& outMove)
{
	return m_pieceManager->endSelection(screenPosition, outMove);
}

void BoardManager::update(const float & deltaTime)
{
	m_pieceManager->update(deltaTime);
}

void BoardManager::render(sf::RenderTarget * const target)
{
	m_pieceManager->render(target);
}
