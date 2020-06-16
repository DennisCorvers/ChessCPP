#include <algorithm>
#include "ChessRules.h"
#include "ChessBoard.h"
#include "Enums.h"

ChessBoard::ChessBoard(const char(&boardData)[SIZE])
{
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

ChessAction ChessBoard::applyMove(const ChessMove newMove)
{
	//Add special moves...
	int indexFrom = newMove.getPositionFrom().getY() * 8 + newMove.getPositionFrom().getX();
	int indexTo = newMove.getPositionTo().getY() * 8 + newMove.getPositionTo().getX();

	ChessAction newAction;
	newAction.pieceFrom = m_currentBoard[indexFrom];
	newAction.pieceTo = m_currentBoard[indexTo];
	newAction.moveFrom = newMove.getPositionFrom();
	newAction.moveTo = newMove.getPositionTo();

	m_currentBoard[indexFrom] = 0;
	m_currentBoard[indexTo] = newAction.pieceFrom;
	if (!newAction.pieceTo.isEmpty())
		newAction.actionType = ActionType::Take;
	else
		newAction.actionType = ActionType::Normal;

	return newAction;
}

ChessAction ChessBoard::inputMove(const ChessMove newMove)
{
	if (isValidPosition(newMove.getPositionTo(), getValidPositions(newMove.getPositionFrom())))
		return applyMove(newMove);

	return ChessAction();
}

void ChessBoard::resetBoard()
{
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

