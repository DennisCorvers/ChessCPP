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
	std::memcpy(this->m_board, board.m_board, BOARDSIZE * sizeof(ChessPiece));
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

ActionType ChessBoard::simulateMove(ChessBoard& nextState, const ChessMove& newMove, bool validateCheckmate) const
{
	ChessPiece pieceFrom = this->getPiece(newMove.getPositionFrom());
	ChessPiece pieceTo = this->getPiece(newMove.getPositionTo());
	ChessAction newAction(pieceFrom, pieceTo, newMove);

	//Simulate the input move on a copy of the board.
	nextState = ChessBoard(*this);
	newAction.actionType = applyMove(nextState, newAction, pieceFrom);

	if (ChessRules::isCheck(nextState.m_kingMap[pieceFrom.getColour()], nextState)) {
		newAction.actionType = ActionType::None;
	}
	else if (validateCheckmate) {
		PieceColour enemyColour = pieceFrom.getColour() == PieceColour::Black ? PieceColour::White : PieceColour::Black;
		if (ChessRules::isCheck(nextState.m_kingMap[enemyColour], nextState)) {
			newAction.actionType = newAction.actionType | ActionType::Check;
			//Check for checkmate?
		}
	}

	nextState.m_lastMove = newAction;
	return newAction.actionType;
}

ActionType ChessBoard::applyMove(ChessBoard & board, const ChessAction& action, const ChessPiece & piece)
{
	if (piece.getType() == PieceType::King)
		board.m_kingMap[piece.getColour()] = action.moveTo;

	ChessMove move(action.moveFrom, action.moveTo);
	ChessPiece& pieceFrom = board.getPieceRef(action.moveFrom);
	ChessPiece& pieceTo = board.getPieceRef(action.moveTo);

	board.m_moveNumber++;;

	//Do NOT take a king!
	if (pieceTo.getType() == PieceType::King)
		return ActionType::None;

	//CASTLING
	if (pieceFrom.getType() == PieceType::King && !pieceFrom.hasMoved() && move.distance().x() == 2) {
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

		return ActionType::Castling | ActionType::Normal;
	}

	//EN PASSANT
	if (ChessRules::isEnpassant(move, pieceFrom, board)) {
		char mod = pieceFrom.getColour() == PieceColour::Black ? -1 : 1;
		ChessPiece& passantPiece = board.getPieceRef(move.getPositionTo().x(), move.getPositionTo().y() + mod);
		passantPiece.reset();

		pieceTo.setTo(pieceFrom);
		pieceFrom.reset();

		return ActionType::EnPassant | ActionType::Take;
	}

	//PROMOTION
	if (ChessRules::isPromotion(move, pieceFrom, board)) {
		ActionType actionResult = ActionType::Promotion;
		if (!pieceTo.isEmpty())
			actionResult = actionResult | ActionType::Take;
		else
			actionResult = actionResult | ActionType::Normal;

		pieceTo.setTo(pieceFrom);
		pieceTo.setTo(PieceType::Queen);
		pieceFrom.reset();

		return actionResult;
	}


	//NORMAL MOVES
	bool isTake = !pieceTo.isEmpty();
	pieceTo.setTo(pieceFrom);
	pieceFrom.reset();

	if (isTake)
		return ActionType::Take;
	else
		return ActionType::Normal;
}
