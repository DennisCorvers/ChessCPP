#pragma once
#include <vector>

#include "ChessMove.h"
#include "ChessPiece.h"
#include "ChessPosition.h"


namespace Mechanics {

	class ChessBoard
	{
	private:
		static const char m_defaultBoard[8][8];
		ChessPiece m_board[8][8];

	public:
		ChessBoard();

		void resetBoard();
		const ChessPiece* Board() const;

		~ChessBoard();
	};
}

