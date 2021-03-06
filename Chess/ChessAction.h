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

	ChessAction(ChessMove move)
		:actionType(ActionType::None) {
		moveFrom = move.getPositionFrom();
		moveTo = move.getPositionTo();
	}

	inline bool isValidMove() {
		return actionType != ActionType::None;
	}

	void netSerialize(sf::Packet& packet, bool isWriting) {
		if (isWriting)
			packet << actionType;
		else
			packet >> actionType;

		moveFrom.netSerialize(packet, isWriting);
		moveTo.netSerialize(packet, isWriting);
		pieceFrom.netSerialize(packet, isWriting);
		pieceTo.netSerialize(packet, isWriting);
	}
};


