#pragma once
#include <vector>

#include "ChessMove.h"
#include "ChessPiece.h"
#include "ChessPosition.h"

namespace Mechanics {

	class ChessBoard
	{
	private:
		std::vector<ChessPiece> m_board;

	public:
		ChessBoard();
		~ChessBoard();
	};
}

