#pragma once
#include "ChessPiece.h"

namespace Mechanics {

	enum struct ActionType : char {
		None = -1,
		Normal = 0,
		Check = 1,
		Checkmate = 2,
		Take = 3,
		EnPassant = 4,
		Castling = 5
	};

	struct ChessAction
	{
	private:
		char m_resultingAction;
	public:
		ChessAction();
		ChessAction(ActionType actionType, PieceType pieceType);

		ActionType getActionType() const;
		PieceType getPieceType() const;
	};
}


