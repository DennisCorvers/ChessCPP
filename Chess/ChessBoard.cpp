#include "pch.h"
#include "ChessRules.h"
#include "ChessBoard.h"

ChessBoard::ChessBoard(const char(&boardData)[SIZE])
{
	m_moveHistory.reserve(32);
	m_defaultBoard = boardData;
}

ChessBoard::~ChessBoard()
{	}

bool ChessBoard::isValidPosition(const ChessPosition position, const std::vector<ChessPosition>& validPositions) const
{
	for (auto it = validPositions.begin(); it != validPositions.end(); ++it)
	{
		if (position == *it)
			return true;
	}
	return false;
}

bool ChessBoard::isValidSelection(const ChessPosition position, const PieceColour playerColour) const
{
	ChessPiece piece = getPiece(position);

	return piece.getColour() == playerColour;
}

bool ChessBoard::tryGetLastMove(ChessAction& out) const
{
	if (m_moveHistory.size() < 1)
		return false;

	out = m_moveHistory.back();
	return true;
}

ChessAction ChessBoard::applyMove(const ChessMove newMove)
{
	ChessPiece& pieceFrom = getPiece(newMove.getPositionFrom());
	ChessPiece& pieceTo = getPiece(newMove.getPositionTo());
	ChessAction newAction(pieceFrom, pieceTo, newMove);

	//Apply promotion
	if (ChessRules::isPromotion(newMove, pieceFrom, *this)) {
		newAction.actionType = ActionType::Promotion;

		pieceTo.setTo(pieceFrom);
		pieceTo.setTo(PieceType::Queen);
		pieceFrom.reset();

		return newAction;
	}

	//Apply EnPassant
	if (ChessRules::isEnpassant(newMove, pieceFrom, *this)) {
		newAction.actionType = ActionType::EnPassant;
		char mod = pieceFrom.getColour() == PieceColour::Black ? -1 : 1;
		ChessPiece& passantPiece = getPiece(newMove.getPositionTo().x(), newMove.getPositionTo().y() + mod);
		passantPiece.reset();

		pieceTo.setTo(pieceFrom);
		pieceFrom.reset();

		return newAction;
	}

	//Apply Castling
	if (ChessRules::isCastling(newMove, pieceFrom, *this)) {
		newAction.actionType = ActionType::Castling;
		bool castleLeft = newAction.moveTo.x() < 5;
		char yPos = newAction.moveFrom.y();
		char xPos = castleLeft ? 0 : 7;
		char xMod = castleLeft ? 1 : -1;

		pieceTo.setTo(pieceFrom);
		pieceFrom.reset();

		ChessPiece& rook = getPiece(xPos, yPos);
		ChessPiece& rookTo = getPiece(newAction.moveTo.x() + xMod, yPos);
		rookTo.setTo(rook);
		rook.reset();

		return newAction;
	}

	if (!newAction.pieceTo.isEmpty())
		newAction.actionType = ActionType::Take;
	else
		newAction.actionType = ActionType::Normal;

	pieceTo.setTo(pieceFrom);
	pieceFrom.reset();

	return newAction;
}

ChessAction ChessBoard::inputMove(const ChessMove newMove)
{
	sf::Clock c; c.restart();
	if (isValidPosition(newMove.getPositionTo(), getValidPositions(newMove.getPositionFrom()))) {
		auto result = applyMove(newMove);
		m_moveHistory.push_back(result);
		std::cout << "Time: " << c.getElapsedTime().asMicroseconds() << std::endl;
		return result;
	}

	return ChessAction();
}

void ChessBoard::resetBoard()
{
	m_moveHistory.clear();

	for (char x = 0; x < 8; x++)
	{
		for (char y = 0; y < 8; y++)
		{
			int index = x * 8 + y;
			m_currentBoard[index] = m_defaultBoard[index];
		}
	}
}

std::vector<ChessPosition> ChessBoard::getValidPositions(const ChessPosition& selectedPosition) const
{
	return ChessRules::getValidPositions(selectedPosition, *this);
}

