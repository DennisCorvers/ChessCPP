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

ChessBoard::ChessBoard(const ChessBoard & board)
{
	for (char i = 0; i < BOARDSIZE; i++)
		m_board[i] = board.m_board[i];

	m_defaultBoard = board.m_defaultBoard;
	m_kingMap = board.m_kingMap;
	m_lastMove = board.m_lastMove;
	m_moveNumber = board.m_moveNumber;
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


	//Simulate the input move on a copy of the board.
	ChessBoard nextState = *this;
	applyMove(nextState, newAction, pieceFrom);

	if (ChessRules::isCheck(m_kingMap[pieceFrom.getColour()], nextState)) {
		newAction.actionType = ActionType::None;
	}
	else {
		if (tryApplyMove) {
			nextState.m_lastMove = newAction;

			//This state could be saved (or a delta) to replay/rewind games
			*this = nextState;
		}
	}

	return newAction.actionType;
}

void ChessBoard::applyMove(ChessBoard & board, const ChessAction & action, const ChessPiece & piece)
{
	if (action.actionType == ActionType::None)
		return;

	if (piece.getType() == PieceType::King)
		board.m_kingMap[piece.getColour()] = action.moveTo;

	ChessMove move(action.moveFrom, action.moveTo);
	ChessPiece& pieceFrom = board.getPiece(action.moveFrom);
	ChessPiece& pieceTo = board.getPiece(action.moveTo);

	switch (action.actionType) {

	case ActionType::EnPassant:
	{
		char mod = pieceFrom.getColour() == PieceColour::Black ? -1 : 1;
		ChessPiece& passantPiece = board.getPiece(move.getPositionTo().x(), move.getPositionTo().y() + mod);
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

		ChessPiece& rook = board.getPiece(xPos, yPos);
		ChessPiece& rookTo = board.getPiece(action.moveTo.x() + xMod, yPos);
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
