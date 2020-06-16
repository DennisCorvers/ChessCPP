#pragma once
#include "Enums.h"
#include "ChessPiece.h"
#include "ChessPosition.h"

struct ChessAction
{
public:
	ActionType actionType;
	ChessPosition moveFrom;
	ChessPosition moveTo;
	ChessPiece pieceFrom;
	ChessPiece pieceTo;

	ChessAction()
		:actionType(ActionType::None) {
	}

	inline bool isValidMove() {
		return actionType != ActionType::None;
	}
};


