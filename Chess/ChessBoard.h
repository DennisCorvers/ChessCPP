#pragma once
#include "ChessPiece.h"
#include "ChessMove.h"
#include "Enums.h"
#include "ChessAction.h"

namespace BoardSettings {
	static const char DEFAULTBOARD[64]{
	 -2, -3, -4, -5, -6, -4, -3, -2,
	 -1, -1, -1, -1, -1, -1, -1, -1,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  1,  1,  1,  1,  1,  1,  1,  1,
	  2,  3,  4,  5,  6,  4,  3,  2,
	};
}

class ChessBoard
{
private:
	const static char SIZE = 64;

	ChessPiece m_currentBoard[SIZE];
	const char* m_defaultBoard;

	ChessAction applyMove(const ChessMove newMove);
	std::vector<ChessAction> m_moveHistory;

	inline ChessPiece& getPiece(char x, char y) {
		return m_currentBoard[y * 8 + x];
	}

	inline ChessPiece& getPiece(ChessPosition position) {
		return getPiece(position.x(), position.y());
	}

public:
	ChessBoard(const char(&boardData)[SIZE]);
	virtual ~ChessBoard();

	inline const ChessPiece getPiece(char x, char y) const {
		return m_currentBoard[y * 8 + x];
	}

	inline const ChessPiece getPiece(ChessPosition position) const {
		return getPiece(position.x(), position.y());
	}

	bool isValidPosition(const ChessPosition position, const std::vector<ChessPosition>& validPositions) const;
	bool isValidSelection(const ChessPosition position, const PieceColour playerColour) const;
	bool tryGetLastMove(ChessAction& out) const;

	ChessAction inputMove(const ChessMove newMove);

	void resetBoard();

	std::vector<ChessPosition> getValidPositions(const ChessPosition& selectedPosition) const;
};

