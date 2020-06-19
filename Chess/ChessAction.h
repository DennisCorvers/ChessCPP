#pragma once
#include "Enums.h"
#include "ChessPiece.h"
#include "ChessPosition.h"
#include "ChessMove.h"

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

	ChessAction(ChessPiece pieceFrom, ChessPiece pieceTo, ChessMove move)
		:actionType(ActionType::None) {
		this->pieceFrom = pieceFrom;
		this->pieceTo = pieceTo;
		moveFrom = move.getPositionFrom();
		moveTo = move.getPositionTo();
	}

	inline bool isValidMove() {
		return actionType != ActionType::None;
	}

	friend std::ostream& operator<<(std::ostream& output, const ChessAction& action) {
		return output << Enums::EnumtoString(action.pieceFrom.getColour()) << " " << Enums::EnumtoString(action.pieceFrom.getType()) << " " 
			<< action.moveFrom << " to " << action.moveTo;
	}
};


