#include "pch.h"
#include "ChessBoard.h"
#include "ChessRules.h"
#include "ChessMove.h"

ChessBoard::ChessBoard(const char(&boardData)[BOARDSIZE])
{
	m_defaultBoard = boardData;
	m_kingMap[PieceColour::Black] = ChessPosition(0, 0);
	m_kingMap[PieceColour::White] = ChessPosition(0, 0);
}

ChessBoard::~ChessBoard()
{

}

void ChessBoard::resetBoard()
{
	for (char x = 0; x < 8; x++)
	{
		for (char y = 0; y < 8; y++)
		{
			int index = x * 8 + y;
			m_board[index] = m_defaultBoard[index];

			const ChessPiece& piece = m_board[index];
			if (piece.getType() == PieceType::King)
				m_kingMap[piece.getColour()] = ChessPosition(y, x);
		}
	}
}

ActionType ChessBoard::simulateMove(const ChessMove& newMove, bool tryApplyMove = true)
{
	ChessPiece& pieceFrom = getPiece(newMove.getPositionFrom());
	ChessPiece& pieceTo = getPiece(newMove.getPositionTo());
	ChessAction newAction(pieceFrom, pieceTo, newMove);
	newAction.actionType = ActionType::Normal;

	if (!newAction.pieceTo.isEmpty())
		newAction.actionType = ActionType::Take;
	else
		newAction.actionType = ActionType::Normal;

	if (ChessRules::isEnpassant(newMove, pieceFrom, *this)) {
		newAction.actionType = ActionType::EnPassant;
	}
	else if (ChessRules::isPromotion(newMove, pieceFrom, *this)) {
		newAction.actionType = ActionType::Promotion;
	}
	else if (ChessRules::isCastling(newMove, pieceFrom, *this)) {
		newAction.actionType = ActionType::Castling;
	}

	//Move game state one move ahead
	applyMove(newAction, pieceFrom);

	if (ChessRules::isCheck(m_kingMap[pieceFrom.getColour()], *this)) {
		reverseMove(newAction);
		newAction.actionType = ActionType::None;
	}
	else {
		if (tryApplyMove)
			m_lastMove = newAction;
		else
			reverseMove(newAction);
	}

	return newAction.actionType;
}

void ChessBoard::applyMove(const ChessAction& action, const ChessPiece& piece)
{
	if (action.actionType == ActionType::None)
		return;

	if (piece.getType() == PieceType::King)
		m_kingMap[piece.getColour()] = action.moveTo;

	ChessMove move(action.moveFrom, action.moveTo);
	ChessPiece& pieceFrom = getPiece(action.moveFrom);
	ChessPiece& pieceTo = getPiece(action.moveTo);

	switch (action.actionType) {

	case ActionType::EnPassant:
	{
		char mod = pieceFrom.getColour() == PieceColour::Black ? -1 : 1;
		ChessPiece& passantPiece = getPiece(move.getPositionTo().x(), move.getPositionTo().y() + mod);
		passantPiece.reset();

		pieceTo.setTo(pieceFrom);
		pieceFrom.reset();
		break;
	}
	case ActionType::Castling:
	{
		bool castleLeft = action.moveTo.x() < 5;
		char yPos = action.moveFrom.y();
		char xPos = castleLeft ? 0 : 7;
		char xMod = castleLeft ? 1 : -1;

		pieceTo.setTo(pieceFrom);
		pieceFrom.reset();

		ChessPiece& rook = getPiece(xPos, yPos);
		ChessPiece& rookTo = getPiece(action.moveTo.x() + xMod, yPos);
		rookTo.setTo(rook);
		rook.reset();
		break;
	}
	case ActionType::Promotion:
	{
		pieceTo.setTo(pieceFrom);
		pieceTo.setTo(PieceType::Queen);
		pieceFrom.reset();
		break;
	}
	default:
	{
		pieceTo.setTo(pieceFrom);
		pieceFrom.reset();
		break;
	}
	}

	m_moveNumber++;
}

void ChessBoard::reverseMove(const ChessAction& action)
{
	if (action.actionType == ActionType::None)
		return;

	//if (piece.getType() == PieceType::King)
	//	m_kingMap[piece.getColour()] = action.moveTo;

	ChessMove move(action.moveFrom, action.moveTo);
	ChessPiece& pieceFrom = getPiece(action.moveFrom);
	ChessPiece& pieceTo = getPiece(action.moveTo);

	switch (action.actionType) {
	case ActionType::Promotion:
	{

	}
	case ActionType::EnPassant:
	{

	}
	case ActionType::Castling:
	{

	}
	default:
	{

	}
	}

	m_moveNumber--;
}
