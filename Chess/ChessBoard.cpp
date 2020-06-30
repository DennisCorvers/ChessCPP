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
	m_drawMoves = board.m_drawMoves;
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

ActionType ChessBoard::getBoardState(const PieceColour colour) const
{
	if (m_drawMoves >= 50)
		return ActionType::Draw;

	const ChessPosition king = m_kingMap.at(colour);
	if (ChessRules::isCheck(king, *this)) {
		bool canKingMove = ChessRules::getValidPositions(king, *this).size() > 0;

		if (canKingMove)
			return ActionType::Check;

		if (hasMoves(colour))
			return ActionType::Check;
		else
			return ActionType::Checkmate;
	}
	else {
		if (!hasMoves(colour))
			return ActionType::Stalemate;
	}
	//If the only 2 pieces able to move are kings, Draw

	return ActionType::None;
}

bool ChessBoard::hasMoves(const PieceColour colour) const
{
	for (char y = 0; y < 8; y++)
	{
		for (char x = 0; x < 8; x++)
		{
			int index = x + y * 8;
			auto piece = m_board[index];
			if (piece.isEmpty() || piece.getColour() != colour)
				continue;

			auto positions = ChessRules::getValidPositions(ChessPosition(x, y), *this);
			if (positions.size() > 0) {
				return true;
			}
		}
	}
	return false;
}

ActionType ChessBoard::simulateMove(ChessBoard& nextState, const ChessMove& newMove, bool validateBoardState) const
{
	ChessPiece pieceFrom = this->getPiece(newMove.getPositionFrom());
	ChessPiece pieceTo = this->getPiece(newMove.getPositionTo());
	ChessAction newAction(pieceFrom, pieceTo, newMove);

	//Simulate the input move on a copy of the board.
	nextState = ChessBoard(*this);
	newAction.actionType = applyMove(nextState, newAction);

	if (ChessRules::isCheck(nextState.m_kingMap[pieceFrom.getColour()], nextState)) {
		newAction.actionType = ActionType::None;
	}
	else if (validateBoardState) {
		PieceColour enemyColour = pieceFrom.getColour() == PieceColour::Black ? PieceColour::White : PieceColour::Black;
		newAction.actionType |= nextState.getBoardState(enemyColour);
	}

	if (newAction.pieceFrom.getType() == PieceType::Pawn || newAction.actionType & ActionType::Take) {
		nextState.m_drawMoves = 0;
	}

	nextState.m_lastMove = newAction;
	return newAction.actionType;
}

ActionType ChessBoard::applyMove(ChessBoard & board, const ChessAction& action)
{
	ChessMove move(action.moveFrom, action.moveTo);
	ChessPiece& pieceFrom = board.getPieceRef(action.moveFrom);
	ChessPiece& pieceTo = board.getPieceRef(action.moveTo);

	if (pieceFrom.getType() == PieceType::King)
		board.m_kingMap[pieceFrom.getColour()] = action.moveTo;

	board.m_moveNumber++;
	board.m_drawMoves++;

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
			actionResult |= ActionType::Take;
		else
			actionResult |= ActionType::Normal;

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
