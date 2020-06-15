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

void ChessBoard::applyMove(const ChessMove newMove)
{
	//Add special moves...
	int indexFrom = newMove.getPositionFrom().getY() * 8 + newMove.getPositionFrom().getX();
	int indexTo = newMove.getPositionTo().getY() * 8 + newMove.getPositionTo().getX();

	ChessPiece piece = m_currentBoard[indexFrom];
	m_currentBoard[indexFrom] = 0;
	m_currentBoard[indexTo] = piece;
}

bool ChessBoard::inputMove(const ChessMove newMove)
{
	if (isValidPosition(newMove.getPositionTo(), getValidPositions(newMove.getPositionFrom())))
	{
		applyMove(newMove);
		return true;
	}

	return false;
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

