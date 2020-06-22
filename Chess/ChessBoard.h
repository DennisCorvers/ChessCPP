#pragma once
#include "Enums.h"
#include "ChessPosition.h"
#include "ChessPiece.h"
#include "ChessAction.h"
#include "ChessRules.h"

struct ChessMove;

class ChessBoard
{
private:
	const static char BOARDSIZE = 64;

	ChessPiece m_board[BOARDSIZE];
	ChessAction m_lastMove;

	std::map<PieceColour, ChessPosition> m_kingMap;
	int m_moveNumber = 0;

	inline ChessPiece& getPieceRef(char x, char y) {
		return m_board[y * 8 + x];
	}

	inline ChessPiece& getPieceRef(const ChessPosition position) {
		return getPieceRef(position.x(), position.y());
	}

	static ActionType applyMove(ChessBoard& board, const ChessAction& action, const ChessPiece& piece);

public:

	ChessBoard();
	ChessBoard(const ChessBoard& board);
	virtual ~ChessBoard();

	inline const ChessPiece getPiece(char x, char y) const {
		return m_board[y * 8 + x];
	}

	inline const ChessPiece getPiece(const ChessPosition position) const {
		return getPiece(position.x(), position.y());
	}

	inline const ChessAction& getLastAction() const {
		return m_lastMove;
	}

	inline const PieceColour getPlayingColour() const {
		return m_moveNumber % 2 == 0 ? PieceColour::White : PieceColour::Black;
	}

	/**
	Apply the input to the board to create the next game state.
	*/
	static std::unique_ptr<ChessBoard> simulateMove(const ChessBoard& thisState, const ChessMove& newMove, bool validateCheckmate);
	void resetBoard(const char(&boardData)[BOARDSIZE]);

	inline std::vector<ChessPosition> getValidPositions(const ChessPosition& selectedPosition) {
		return ChessRules::getValidPositions(selectedPosition, *this);
	}
};

