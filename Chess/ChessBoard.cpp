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
	{	}

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

	const ChessPiece* ChessBoard::Board() const {
		return *m_board;
	}

	ChessBoard::~ChessBoard()
	{
	}
}
