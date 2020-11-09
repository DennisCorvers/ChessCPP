#include "pch.h"
#include "ChessBoard.h"
#include "ChessRules.h"
#include "ChessMove.h"

ChessBoard::ChessBoard() :
	m_blackKing(), m_whiteKing()
{ }

ChessBoard::ChessBoard(const ChessBoard & board)
{
	std::memcpy(this->m_board, board.m_board, BOARDSIZE * sizeof(ChessPiece));

	m_whiteKing = board.m_whiteKing;
	m_blackKing = board.m_blackKing;

	m_lastMove = board.m_lastMove;
	m_moveNumber = board.m_moveNumber;
	m_drawMoves = board.m_drawMoves;
}

ChessBoard::~ChessBoard()
{

}

void ChessBoard::resetBoard(const char(&boardData)[BOARDSIZE])
{
	m_lastMove = ChessAction();
	m_moveNumber = 0;
	m_drawMoves = 0;

	for (char x = 0; x < 8; x++)
	{
		for (char y = 0; y < 8; y++)
		{
			int index = y * 8 + x;
			m_board[index] = boardData[index];

			const ChessPiece& piece = m_board[index];
			if (piece.getType() == PieceType::King)
				setKing(piece.getColour(), ChessPosition(x, y));
		}
	}
}

ActionType ChessBoard::getBoardState(const PieceColour colour) const
{
	if (m_drawMoves >= 100)
		return ActionType::Draw;

	const ChessPosition& king = getKing(colour);
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

std::string ChessBoard::getFENFormat() const
{
	std::string FENString;
	FENString.reserve(90); //Max FEN size
	char emptyCount = 0;

	//PIECE PLACEMENT
	for (char y = 0; y < 8; y++) {
		for (char x = 0; x < 8; x++) {
			char pieceVal = getPiece(x, y).getFEN();
			if (pieceVal == 0) {
				emptyCount++;
				if (emptyCount == 8) {
					FENString += '8';
					emptyCount = 0;
					break;
				}
				continue;
			}

			if (emptyCount > 0) {
				FENString += (emptyCount + 48);
				emptyCount = 0;
			}
			FENString += pieceVal;
		}

		if (emptyCount > 0) {
			FENString += (emptyCount + 48);
			emptyCount = 0;
		}
		FENString += '/';
	}
	FENString[FENString.size() - 1] = ' ';

	//SIDE TO MOVE
	FENString += getPlayingColour() == PieceColour::Black ? "b " : "w ";

	//CASTLING ABILITY
	char castlingRights[4] = { 'K', 'Q', 'k', 'q' };
	//White castling rights
	if (getPiece(m_whiteKing).hasMoved()) {
		castlingRights[0] = 0;
		castlingRights[1] = 0;
	}
	else {
		ChessPiece rookK = getPiece(7, 7);
		if (rookK.getType() != PieceType::Rook || rookK.hasMoved())
			castlingRights[0] = 0;
		ChessPiece rookQ = getPiece(0, 7);
		if (rookQ.getType() != PieceType::Rook || rookQ.hasMoved())
			castlingRights[1] = 0;
	}

	//Black castling rights
	if (getPiece(m_blackKing).hasMoved()) {
		castlingRights[2] = 0;
		castlingRights[3] = 0;
	}
	else {
		ChessPiece rookK = getPiece(7, 0);
		if (rookK.getType() != PieceType::Rook || rookK.hasMoved())
			castlingRights[2] = 0;
		ChessPiece rookQ = getPiece(0, 0);
		if (rookQ.getType() != PieceType::Rook || rookQ.hasMoved())
			castlingRights[3] = 0;
	}

	int rightsCount = 0;
	for (char cr : castlingRights) {
		if (cr != 0) {
			FENString += cr;
			rightsCount++;
		}
	}
	if (rightsCount == 0)
		FENString += '-';
	FENString += ' ';

	//EN PASSANT TARGET SQUARE
	if (m_lastMove.pieceFrom.getType() == PieceType::Pawn
		&& ChessPosition::distance(m_lastMove.moveFrom, m_lastMove.moveTo).y() == 2) {
		ChessPosition enPassantPosition = ChessPosition(
			(m_lastMove.moveFrom.x() + m_lastMove.moveTo.x()) / 2,
			(m_lastMove.moveFrom.y() + m_lastMove.moveTo.y()) / 2
		);
		FENString += enPassantPosition.toChessNotation() + ' ';
	}
	else
		FENString += "- ";


	//HALFMOVE CLOCK
	FENString += std::to_string(m_drawMoves) + ' ';

	//FULLMOVE COUNTER
	FENString += std::to_string(m_moveNumber / 2 + 1);

	return FENString;
}

void ChessBoard::netSerialize(sf::Packet& packet, bool isWriting)
{
	for (auto chessPiece : m_board) {
		chessPiece.netSerialize(packet, isWriting);
	}

	m_lastMove.netSerialize(packet, isWriting);
	m_blackKing.netSerialize(packet, isWriting);
	m_whiteKing.netSerialize(packet, isWriting);

	if (isWriting) {
		packet << m_moveNumber;
		packet << m_drawMoves;
	}
	else {
		packet >> m_moveNumber;
		packet >> m_drawMoves;
	}
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

	if (ChessRules::isCheck(nextState.getKing(pieceFrom.getColour()), nextState)) {
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
		board.setKing(pieceFrom.getColour(), action.moveTo);

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
