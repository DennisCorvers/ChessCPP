#pragma once
#include "ChessPieceEntity.h"
#include "Enums.h"

namespace Mechanics {

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


