#include "ChessBoard.h"

namespace Mechanics {

	const char ChessBoard::m_defaultBoard[8][8] = {
		{ -2, -3, -4, -5, -6, -4, -3, -2},
		{ -1, -1, -1, -1, -1, -1, -1, -1 },
		{  0,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  0,  0,  0,  0,  0 },
		{  1,  1,  1,  1,  1,  1,  1,  1 },
		{  2,  3,  4,  5,  6,  4,  3,  2 },
	};

	ChessBoard::ChessBoard()
	{
		m_isDirty = false;
	}

	ChessBoard::~ChessBoard()
	{
	}

	const ChessPiece* ChessBoard::getBoard() const {
		return *m_board;
	}

	void ChessBoard::resetBoard()
	{
		for (char x = 0; x < 8; x++)
		{
			for (char y = 0; y < 8; y++)
			{
				char pieceValue = m_defaultBoard[x][y];
				m_board[x][y] = ChessPiece(pieceValue);
			}
		}
	}

	bool ChessBoard::tryMovePiece(ChessMove move)
	{
		ChessPosition moveFrom = move.getPositionFrom();
		ChessPiece piece = m_board[moveFrom.X()][moveFrom.Y()];

		if (piece.canMove(move, this))
		{
			m_isDirty = true;

		}
		return false;
	}
}
