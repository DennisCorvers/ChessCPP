#include <algorithm>
#include "ChessRules.h"
#include "ChessBoard.h"
#include "Enums.h"

ChessBoard::ChessBoard(const char(&boardData)[SIZE])
{
	m_moveHistory.reserve(32);
	m_defaultBoard = boardData;
}

ChessBoard::~ChessBoard()
{	}

bool ChessBoard::isValidPosition(const ChessPosition position, const std::vector<ChessPosition>& validPositions) const
{
	for (auto it = validPositions.begin(); it != validPositions.end(); ++it)
	{
		if (position == *it)
			return true;
	}
	return false;
}

bool ChessBoard::isValidSelection(const ChessPosition position, const PieceColour playerColour) const
{
	ChessPiece piece = getPiece(position);

	return piece.getColour() == playerColour;
}

bool ChessBoard::tryGetLastMove(ChessAction& out) const
{
	if (m_moveHistory.size() < 1)
		return false;

	out = m_moveHistory.back();
	return true;
}

ChessAction ChessBoard::applyMove(const ChessMove newMove)
{
	//Add special moves...
	int indexFrom = newMove.getPositionFrom().getY() * 8 + newMove.getPositionFrom().getX();
	int indexTo = newMove.getPositionTo().getY() * 8 + newMove.getPositionTo().getX();

	ChessPiece& pieceFrom = m_currentBoard[indexFrom];
	ChessPiece& pieceTo = m_currentBoard[indexTo];
	pieceFrom.setMoved(); pieceTo.setMoved();

	ChessAction newAction(pieceFrom, pieceTo, newMove);

	//Apply promotion
	if (ChessRules::isPromotion(newMove, pieceFrom, *this)) {
		newAction.actionType = ActionType::Promotion;

		pieceTo.setTo(pieceFrom);
		pieceTo.setTo(PieceType::Queen);
		pieceFrom.reset();

		return newAction;
	}

	//Apply EnPassant
	if (ChessRules::isEnpassant(newMove, pieceFrom, *this)) {
		newAction.actionType = ActionType::EnPassant;
		char mod = pieceFrom.getColour() == PieceColour::Black ? -1 : 1;
		int passantIndex = (newMove.getPositionTo().getY() + mod) * 8 + newMove.getPositionTo().getX();

		m_currentBoard[passantIndex].reset();
		pieceTo.setTo(pieceFrom);
		pieceFrom.reset();

		return newAction;
	}

	if (!newAction.pieceTo.isEmpty())
		newAction.actionType = ActionType::Take;
	else
		newAction.actionType = ActionType::Normal;

	pieceTo.setTo(pieceFrom);
	pieceFrom.reset();
	//Apply Castling


	return newAction;
}

ChessAction ChessBoard::inputMove(const ChessMove newMove)
{
	if (isValidPosition(newMove.getPositionTo(), getValidPositions(newMove.getPositionFrom()))) {
		auto result = applyMove(newMove);
		m_moveHistory.push_back(result);
		return result;
	}

	return ChessAction();
}

void ChessBoard::resetBoard()
{
	m_moveHistory.clear();

	for (char x = 0; x < 8; x++)
	{
		for (char y = 0; y < 8; y++)
		{
			int index = x * 8 + y;
			m_currentBoard[index] = m_defaultBoard[index];
		}
	}
}

std::vector<ChessPosition> ChessBoard::getValidPositions(const ChessPosition& selectedPosition) const
{
	return ChessRules::getValidPositions(selectedPosition, *this);
}

