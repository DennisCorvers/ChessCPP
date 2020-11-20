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

	ChessPosition m_blackKing;
	ChessPosition m_whiteKing;

	unsigned short m_moveNumber = 0;
	unsigned char m_drawMoves = 0;

	inline ChessPiece& getPieceRef(char x, char y) {
		return m_board[y * 8 + x];
	}

	inline ChessPiece& getPieceRef(const ChessPosition& position) {
		return getPieceRef(position.x(), position.y());
	}

	inline const ChessPosition& getKing(const PieceColour colour) const {
		return colour == PieceColour::Black ? m_blackKing : m_whiteKing;
	}

	inline void setKing(const PieceColour colour, const ChessPosition& position) {
		if (colour == PieceColour::Black)
			m_blackKing = position;
		else
			m_whiteKing = position;
	}

	bool hasMoves(const PieceColour colour) const;

	static ActionType applyMove(ChessBoard& board, const ChessAction& action);

public:

	ChessBoard();
	ChessBoard(const ChessBoard& board);
	virtual ~ChessBoard();

	inline const ChessPiece getPiece(char x, char y) const {
		return m_board[y * 8 + x];
	}

	inline const ChessPiece getPiece(const ChessPosition& position) const {
		return getPiece(position.x(), position.y());
	}


	inline const ChessAction& getLastAction() const {
		return m_lastMove;
	}

	inline const PieceColour getPlayingColour() const {
		return m_moveNumber % 2 == 0 ? PieceColour::White : PieceColour::Black;
	}

	unsigned short getMoveNumber() const {
		return m_moveNumber;
	}


	ActionType simulateMove(ChessBoard& nextState, const ChessMove& newMove, bool validateBoardState = false) const;
	void resetBoard(const char(&boardData)[BOARDSIZE]);

	inline std::vector<ChessPosition> getValidPositions(const ChessPosition& selectedPosition) const {
		return ChessRules::getValidPositions(selectedPosition, *this);
	}

	ActionType getBoardState(const PieceColour colour) const;

	std::string getFENFormat() const;

	void netSerialize(sf::Packet& packet, bool isWriting);
};

