#include "pch.h"
#include "ChessBoard.h"
#include "ChessRules.h"
#include "ChessMove.h"

ChessBoard::ChessBoard()
{
	m_kingMap[PieceColour::Black] = ChessPosition(0, 0);
	m_kingMap[PieceColour::White] = ChessPosition(0, 0);
}

ChessBoard::ChessBoard(const ChessBoard & board)
{
	for (char i = 0; i < BOARDSIZE; i++)
		m_board[i] = board.m_board[i];

	m_kingMap = board.m_kingMap;
	m_lastMove = board.m_lastMove;
	m_moveNumber = board.m_moveNumber;
}

ChessBoard::~ChessBoard()
{

}

void ChessBoard::resetBoard(const char(&boardData)[BOARDSIZE])
{
	for (char x = 0; x < 8; x++)
	{
		for (char y = 0; y < 8; y++)
		{
			int index = x * 8 + y;
			m_board[index] = boardData[index];

			const ChessPiece& piece = m_board[index];
			if (piece.getType() == PieceType::King)
				m_kingMap[piece.getColour()] = ChessPosition(y, x);
		}
	}
}

ActionType ChessBoard::simulateMove(const ChessBoard & thisState, ChessBoard & nextStateOut, const ChessMove & newMove, bool validateCheckmate)
{
	ChessPiece pieceFrom = thisState.getPiece(newMove.getPositionFrom());
	ChessPiece pieceTo = thisState.getPiece(newMove.getPositionTo());
	ChessAction newAction(pieceFrom, pieceTo, newMove);

	if (!pieceTo.isEmpty())
	{
		if (pieceTo.getType() == PieceType::King) {
			newAction.actionType = ActionType::None;
		}
		else {
			newAction.actionType = ActionType::Take;
		}
	}
	else {
		newAction.actionType = ActionType::Normal;
	}

	if (pieceFrom.getType() == PieceType::King && !pieceFrom.hasMoved() && newMove.distance().x() == 2)
		newAction.actionType = ActionType::Castling;

	if (ChessRules::isPromotion(newMove, pieceFrom, thisState))
		newAction.actionType = ActionType::Promotion;

	if (ChessRules::isEnpassant(newMove, pieceFrom, thisState))
		newAction.actionType = ActionType::EnPassant;

	//Simulate the input move on a copy of the board.
	nextStateOut = ChessBoard(thisState);

	applyMove(nextStateOut, newAction, pieceFrom);

	if (ChessRules::isCheck(nextStateOut.m_kingMap[pieceFrom.getColour()], nextStateOut)) {
		newAction.actionType = ActionType::None;
	}
	else {
		if (validateCheckmate) {
			PieceColour enemyColour = pieceFrom.getColour() == PieceColour::Black ? PieceColour::White : PieceColour::Black;
			if (ChessRules::isCheck(nextStateOut.m_kingMap[enemyColour], nextStateOut)) {
				newAction.actionType = ActionType::Check;
				//Check for checkmate?
			}
		}
	}

	nextStateOut.m_lastMove = newAction;
	return newAction.actionType;
}

void ChessBoard::applyMove(ChessBoard & board, const ChessAction & action, const ChessPiece & piece)
{
	if (action.actionType == ActionType::None)
		return;

	if (piece.getType() == PieceType::King)
		board.m_kingMap[piece.getColour()] = action.moveTo;

	ChessMove move(action.moveFrom, action.moveTo);
	ChessPiece& pieceFrom = board.getPieceRef(action.moveFrom);
	auto c = pieceFrom.getColour();
	ChessPiece& pieceTo = board.getPieceRef(action.moveTo);

	switch (action.actionType) {

	case ActionType::EnPassant:
	{
		char mod = pieceFrom.getColour() == PieceColour::Black ? -1 : 1;
		ChessPiece& passantPiece = board.getPieceRef(move.getPositionTo().x(), move.getPositionTo().y() + mod);
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

		ChessPiece& rook = board.getPieceRef(xPos, yPos);
		ChessPiece& rookTo = board.getPieceRef(action.moveTo.x() + xMod, yPos);
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

	board.m_moveNumber++;
}
