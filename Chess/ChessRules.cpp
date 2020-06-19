#include "pch.h"
#include "ChessRules.h" 
#include "ChessBoard.h"

namespace {

	using MoveSet = std::vector<sf::Vector2i>;

	const MoveSet kingMoves{
		sf::Vector2i(-1,1),
		sf::Vector2i(0,1),
		sf::Vector2i(1,1),
		sf::Vector2i(1,0),
		sf::Vector2i(1,-1),
		sf::Vector2i(0,-1),
		sf::Vector2i(-1,-1),
		sf::Vector2i(-1,0)
	};

	const MoveSet knightMoves{
		sf::Vector2i(1,-2),
		sf::Vector2i(2,-1),
		sf::Vector2i(2,1),
		sf::Vector2i(1,2),
		sf::Vector2i(-1,2),
		sf::Vector2i(-2,1),
		sf::Vector2i(-2,-1),
		sf::Vector2i(-1,-2)
	};

	inline bool isOutOfBounds(const sf::Vector2i pos) {
		return pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7;
	}

	void slide(sf::Vector2i pos, sf::Vector2i dir, const ChessBoard& board, std::vector<ChessPosition>& validMoves)
	{
		ChessPiece piece = board.getPiece(pos.x, pos.y);
		for (char i = 1; i < 8; i++)
		{
			sf::Vector2i newPos(pos.x + i * dir.x, pos.y + i * dir.y);
			if (isOutOfBounds(newPos))
				break;

			ChessPiece other = board.getPiece(newPos.x, newPos.y);
			if (!other.isEmpty()) {
				if (piece.getColour() != other.getColour())
					validMoves.emplace_back(newPos.x, newPos.y);
				break;
			}
			else {
				validMoves.emplace_back(newPos.x, newPos.y);
			}
		}
	}

	void addDiagonal(sf::Vector2i pos, const ChessBoard& board, std::vector<ChessPosition>& validMoves) {
		slide(pos, sf::Vector2i(1, 1), board, validMoves);
		slide(pos, sf::Vector2i(-1, -1), board, validMoves);
		slide(pos, sf::Vector2i(-1, 1), board, validMoves);
		slide(pos, sf::Vector2i(1, -1), board, validMoves);
	}
	void addOrthogonal(sf::Vector2i pos, const ChessBoard& board, std::vector<ChessPosition>& validMoves) {
		slide(pos, sf::Vector2i(1, 0), board, validMoves);
		slide(pos, sf::Vector2i(-1, 0), board, validMoves);
		slide(pos, sf::Vector2i(0, 1), board, validMoves);
		slide(pos, sf::Vector2i(0, -1), board, validMoves);
	}
	void addMoveset(sf::Vector2i pos, const ChessBoard& board, std::vector<ChessPosition>& validMoves, const MoveSet& moveSet) {

		ChessPiece piece = board.getPiece(pos.x, pos.y);
		for (char i = 0; i < moveSet.size(); i++)
		{
			sf::Vector2i kMove = moveSet[i];
			sf::Vector2i newPos(pos.x + kMove.x, pos.y + kMove.y);
			if (isOutOfBounds(newPos))
				continue;

			ChessPiece other = board.getPiece(newPos.x, newPos.y);
			if (!other.isEmpty()) {
				if (other.getColour() != piece.getColour())
					validMoves.emplace_back(newPos.x, newPos.y);
				else
					continue;
			}
			else {
				validMoves.emplace_back(newPos.x, newPos.y);
			}
		}
	}

	std::vector<ChessPosition> getPawnPositions(sf::Vector2i pos, const ChessBoard& board) {
		std::vector<ChessPosition> moves;
		moves.reserve(2);

		ChessPiece piece = board.getPiece(pos.x, pos.y);
		int mod = piece.getColour() == PieceColour::White ? -1 : 1;

		//Move 1 ahead
		sf::Vector2i newPos(pos.x, pos.y + mod);
		ChessPiece other = board.getPiece(newPos.x, newPos.y);
		if (!isOutOfBounds(newPos) && other.isEmpty())
		{
			moves.emplace_back(newPos.x, newPos.y);
			//Try move 2 is 1 is clear.
			if (!piece.hasMoved())
			{
				sf::Vector2i newPos(pos.x, pos.y + 2 * mod);
				other = board.getPiece(newPos.x, newPos.y);
				if (!isOutOfBounds(newPos) && other.isEmpty())
					moves.emplace_back(newPos.x, newPos.y);
			}
		}

		//Attack moves
		for (char i = -1; i <= 1; i += 2)
		{
			sf::Vector2i newPos(pos.x + i, pos.y + mod);
			if (isOutOfBounds(newPos))
				continue;

			ChessPiece other = board.getPiece(newPos.x, newPos.y);
			if (!other.isEmpty() && other.getColour() != piece.getColour())
				moves.push_back(ChessPosition(newPos.x, newPos.y));
		}

		//En Passant
		ChessPosition posFrom(pos.x, pos.y);
		for (char i = -1; i <= 1; i += 2)
		{
			newPos = sf::Vector2i(pos.x + i, pos.y + mod);
			if (isOutOfBounds(newPos))
				continue;

			ChessMove newMove(posFrom, ChessPosition(newPos.x, newPos.y));
			if (ChessRules::isEnpassant(newMove, piece, board))
				moves.emplace_back(newPos.x, newPos.y);
		}

		return moves;
	}
	std::vector<ChessPosition> getRookPositions(sf::Vector2i pos, const ChessBoard& board) {
		std::vector<ChessPosition> moves;
		moves.reserve(8);

		addOrthogonal(pos, board, moves);
		return moves;
	}
	std::vector<ChessPosition> getBishopPositions(sf::Vector2i pos, const ChessBoard& board) {
		std::vector<ChessPosition> moves;
		moves.reserve(8);

		addDiagonal(pos, board, moves);
		return moves;
	}
	std::vector<ChessPosition> getKnightPositions(sf::Vector2i pos, const ChessBoard& board) {
		std::vector<ChessPosition> moves;
		moves.reserve(4);

		addMoveset(pos, board, moves, knightMoves);
		return moves;
	}
	std::vector<ChessPosition> getQueenPositions(sf::Vector2i pos, const ChessBoard& board) {
		std::vector<ChessPosition> moves;
		moves.reserve(8);

		addOrthogonal(pos, board, moves);
		addDiagonal(pos, board, moves);
		return moves;
	}
	std::vector<ChessPosition> getKingPositions(sf::Vector2i pos, const ChessBoard& board) {
		std::vector<ChessPosition> moves;
		moves.reserve(4);

		//Castling moves
		ChessPiece piece = board.getPiece(pos.x, pos.y);
		for (char i = -2; i <= 2; i += 4)
		{
			sf::Vector2i newPos(pos.x + i, pos.y);
			if (isOutOfBounds(newPos))
				continue;

			ChessMove newMove(ChessPosition(pos.x, pos.y), ChessPosition(newPos.x, newPos.y));
			if (ChessRules::isCastling(newMove, piece, board))
				moves.emplace_back(newPos.x, newPos.y);
		}

		addMoveset(pos, board, moves, kingMoves);
		return moves;
	}
}

std::vector<ChessPosition> ChessRules::getValidPositions(const ChessPosition & selectedPosition, const ChessBoard& board)
{
	ChessPiece pieceValue = board.getPiece(selectedPosition);
	sf::Vector2i position(selectedPosition.x(), selectedPosition.y());

	switch (pieceValue.getType())
	{
	case PieceType::Pawn:
		return getPawnPositions(position, board);
	case PieceType::Rook:
		return getRookPositions(position, board);
	case PieceType::Knight:
		return getKnightPositions(position, board);
	case PieceType::Bishop:
		return getBishopPositions(position, board);
	case PieceType::Queen:
		return getQueenPositions(position, board);
	case PieceType::King:
		return getKingPositions(position, board);
	default:
		break;
	}

	return std::vector<ChessPosition>();
}

bool ChessRules::isPromotion(const ChessMove & move, const ChessPiece& piece, const ChessBoard & board)
{
	if (piece.getType() != PieceType::Pawn)
		return false;

	char yPos = move.getPositionTo().y();
	if ((piece.getColour() == PieceColour::White && yPos == 0) ||
		(piece.getColour() == PieceColour::Black && yPos == 7)) {
		return true;
	}

	return false;
}

bool ChessRules::isEnpassant(const ChessMove & move, const ChessPiece& piece, const ChessBoard& board) {

	//Moving piece must be a pawn
	if (piece.getType() != PieceType::Pawn)
		return false;

	//Move is diagonal
	ChessPosition diagMove = move.distance();
	if (diagMove.x() != 1 || diagMove.y() != 1)
		return false;

	//There must be at least one prior move
	ChessAction lastAction = board.getLastMove();
	if (lastAction.actionType != ActionType::Normal)
		return false;

	//Prior move must have been a pawn of the opposite colour
	const ChessPiece& lastPiece = lastAction.pieceFrom;
	if (lastPiece.getType() != PieceType::Pawn || piece.getColour() == lastPiece.getColour())
		return false;

	//Last move was a jump of 2
	ChessPosition lastMove = ChessPosition::distance(lastAction.moveFrom, lastAction.moveTo);
	if (lastMove.y() != 2)
		return false;

	//Move To must be empty
	if (!board.getPiece(move.getPositionTo()).isEmpty())
		return false;

	//Opposite pawn exists at EnPassant position
	char mod = piece.getColour() == PieceColour::Black ? -1 : 1;
	const ChessPiece otherPawn = board.getPiece(move.getPositionTo().x(), move.getPositionTo().y() + mod);
	if (otherPawn.getType() != PieceType::Pawn || otherPawn.getColour() == piece.getColour())
		return false;

	return true;
}

bool ChessRules::isCastling(const ChessMove & move, const ChessPiece& piece, const ChessBoard & board)
{
	if (piece.getType() != PieceType::King || piece.hasMoved())
		return false;

	//Validate move distance
	char xDistance = move.getPositionFrom().x() - move.getPositionTo().x();
	if (abs(xDistance) != 2)
		return false;

	char yPos = piece.getColour() == PieceColour::White ? 7 : 0;
	char rookX = xDistance > 0 ? 0 : 7;

	//Check if rook (has moved)
	ChessPiece rook = board.getPiece(rookX, yPos);
	if (rook.getType() != PieceType::Rook || rook.hasMoved())
		return false;

	// Check that the adjacent squares are empty
	char mod = xDistance > 0 ? -1 : 1;
	char kingX = move.getPositionFrom().x();
	char len = abs(kingX - rookX);
	for (int i = 1; i < len; i++)
	{
		//TODO Check for every position if it results in a CHECK
		ChessPosition newPos(kingX + i * mod, yPos);
		if (!board.getPiece(kingX + i * mod, yPos).isEmpty())
			return false;
	}

	return true;
}

bool ChessRules::isCheck(const ChessPosition & king, const ChessBoard & board)
{
	return false;
}

//https://github.com/bdidemus/chess/blob/master/project/Chess/LegalMoveSet.cs




