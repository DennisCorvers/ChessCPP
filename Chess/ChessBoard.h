#pragma once
#include "ChessPiece.h"
#include "ChessAction.h"
#include "ChessRules.h"

struct ChessMove;

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
	const static char BOARDSIZE = 64;

	ChessPiece m_board[BOARDSIZE];
	const char* m_defaultBoard;
	ChessAction m_lastMove;

	std::map<PieceColour, ChessPosition> m_kingMap;
	int m_moveNumber = 0;

	inline ChessPiece& getPiece(char x, char y) {
		return m_board[y * 8 + x];
	}

	inline ChessPiece& getPiece(const ChessPosition position) {
		return getPiece(position.x(), position.y());
	}

	static void applyMove(ChessBoard& board, const ChessAction& action, const ChessPiece & piece);

public:

	ChessBoard(const char(&boardData)[BOARDSIZE]);
	ChessBoard(const ChessBoard& board);
	virtual ~ChessBoard();

	inline const ChessPiece& getPiece(char x, char y) const {
		return m_board[y * 8 + x];
	}

	inline const ChessPiece& getPiece(const ChessPosition position) const {
		return getPiece(position.x(), position.y());
	}

	inline const ChessAction& getLastMove() const {
		return m_lastMove;
	}

	inline const PieceColour getPlayingColour() const {
		return m_moveNumber % 2 == 0 ? PieceColour::White : PieceColour::Black;
	}

	ActionType simulateMove(const ChessMove& newMove, bool tryApplyMove);
	void resetBoard();

	inline std::vector<ChessPosition> getValidPositions(const ChessPosition& selectedPosition) const {
		return ChessRules::getValidPositions(selectedPosition, *this);
	}
};

