#include "pch.h"
#include "BoardManager.h"
#include "ChessPieceManager.h"
#include "ChessBoard.h"
#include "ChessAction.h"

BoardManager::BoardManager(const sf::FloatRect boardSizes,
	std::map<AssetFlags, sf::Texture>& textures,
	std::map <AssetFlags, sf::SoundBuffer>& sounds)
{
	m_board = std::make_unique<ChessBoard>();
	m_pieceManager = std::make_unique<ChessPieceManager>(boardSizes, textures);

	m_soundPieceCheck.setBuffer(sounds[AssetFlags::s_piece_check]);
	m_soundPieceMove.setBuffer(sounds[AssetFlags::s_piece_move]);
	m_soundPieceTake.setBuffer(sounds[AssetFlags::s_piece_take]);
}

BoardManager::~BoardManager()
{

}

sf::Vector2f BoardManager::getBoardCenter()
{
	return m_pieceManager->getCenter();
}

void BoardManager::resetGame()
{
	m_board->resetBoard(BoardSettings::DEFAULTBOARD);
	m_pieceManager->syncPieces(*m_board, false);
}

bool BoardManager::inputMove(const ChessMove move, bool animate)
{
	if (!m_hasCachedMove) {
		std::cout << "No cached move found!" << std::endl;

		if (!validateMove(move))
			return false;
	}
	else
		m_hasCachedMove = false;

	auto nextState = ChessBoard::simulateMove(*m_board, move, true);
	auto actionResult = nextState->getLastAction().actionType;
	if (actionResult != ActionType::None)
	{
		m_board = std::move(nextState);
		handleSound(actionResult, true);
		m_pieceManager->syncPieces(*m_board, animate);
	}

	return true;
}

bool BoardManager::validateMove(const ChessMove move) const
{
	auto validPositions = m_board->getValidPositions(move.getPositionFrom());
	for (auto it = validPositions.begin(); it != validPositions.end(); ++it)
		if (move.getPositionTo() == *it) {
			return true;
		}

	return false;
}

void BoardManager::handleSound(const ActionType chessAction, bool playSound)
{
	if (!playSound)
		return;

	switch (chessAction) {
	case ActionType::Check:
	case ActionType::Checkmate:
		m_soundPieceCheck.play();
		break;

	case ActionType::Castling:
	case ActionType::Normal:
		m_soundPieceMove.play();
		break;

	case ActionType::Take:
	case ActionType::EnPassant:
	case ActionType::Promotion:
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
	bool result = m_pieceManager->endSelection(screenPosition, outMove);
	if (result) {
		m_hasCachedMove = true;
		m_cachedMove = outMove;
	}

	return result;
}

void BoardManager::update(const float & deltaTime)
{
	m_pieceManager->update(deltaTime);
}

void BoardManager::render(sf::RenderTarget * const target)
{
	m_pieceManager->render(target);
}
