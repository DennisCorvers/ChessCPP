#include "ChessBoard.h"
#include "ChessRules.h" 

namespace {
	const sf::Vector2i kingMoves[8]{
		sf::Vector2i(-1,1),
		sf::Vector2i(0,1),
		sf::Vector2i(1,1),
		sf::Vector2i(1,0),
		sf::Vector2i(1,-1),
		sf::Vector2i(0,-1),
		sf::Vector2i(-1,-1),
		sf::Vector2i(-1,0)
	};

	const sf::Vector2i knightMoves[8]{
		sf::Vector2i(1,-2),
		sf::Vector2i(2,-1),
		sf::Vector2i(2,1),
		sf::Vector2i(1,2),
		sf::Vector2i(-1,2),
		sf::Vector2i(-2,1),
		sf::Vector2i(-2,-1),
		sf::Vector2i(-2,-2)
	};

	const sf::Vector2i pawnMoves[4]{
		sf::Vector2i(0,-1),
		sf::Vector2i(0,-2),
		sf::Vector2i(1,-1),
		sf::Vector2i(-1,-1)
	};


	inline bool isOutOfBounds(const sf::Vector2i pos) {
		return pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7;
	}

	void slide(PieceColour colour, sf::Vector2i pos, sf::Vector2i dir, const ChessBoard& board, std::vector<ChessPosition>& validMoves)
	{
		for (char i = 1; i < 8; i++)
		{
			sf::Vector2i newPos(pos.x + i * dir.x, pos.y + i * dir.y);
			if (isOutOfBounds(newPos))
				break;

			char piece = board.getPiece(newPos.x, newPos.y);
			if (piece != 0) {
				if (piece > 0 && colour == PieceColour::Black)
					validMoves.push_back(ChessPosition(newPos.x, newPos.y));
				else
					break;
			}
			else {
				validMoves.push_back(ChessPosition(newPos.x, newPos.y));
			}
		}
	}

	void addDiagonal(PieceColour colour, sf::Vector2i pos, const ChessBoard& board, std::vector<ChessPosition>& validMoves) {
		slide(colour, pos, sf::Vector2i(1, 1), board, validMoves);
		slide(colour, pos, sf::Vector2i(-1, -1), board, validMoves);
		slide(colour, pos, sf::Vector2i(-1, 1), board, validMoves);
		slide(colour, pos, sf::Vector2i(1, -1), board, validMoves);
	}
	void addOrthogonal(PieceColour colour, sf::Vector2i pos, const ChessBoard& board, std::vector<ChessPosition>& validMoves) {
		slide(colour, pos, sf::Vector2i(1, 0), board, validMoves);
		slide(colour, pos, sf::Vector2i(-1, 0), board, validMoves);
		slide(colour, pos, sf::Vector2i(0, 1), board, validMoves);
		slide(colour, pos, sf::Vector2i(0, -1), board, validMoves);
	}

	std::vector<ChessPosition> getPawnPositions(PieceColour colour, sf::Vector2i pos, const ChessBoard& board) {
		std::vector<ChessPosition> ret;
		ret.reserve(2);
		//char nY = piece > 0 ? 7 - y : y;
		//char mod = piece > 0 ? -1 : 1;

		//if (board.getPiece(x, nY + 1) == 0)
		//{
		//	ret.push_back(ChessPosition(x, y + 1));
		//	if (nY == 2)
		//		ret.push_back(ChessPosition(x, y + 2));
		//}

		return std::vector<ChessPosition>();
	}
	std::vector<ChessPosition> getRookPositions(PieceColour colour, sf::Vector2i pos, const ChessBoard& board) {
		std::vector<ChessPosition> moves;
		moves.reserve(8);

		addOrthogonal(colour, pos, board, moves);
		return moves;
	}
	std::vector<ChessPosition> getBishopPositions(PieceColour colour, sf::Vector2i pos, const ChessBoard& board) {
		std::vector<ChessPosition> moves;
		moves.reserve(8);

		addDiagonal(colour, pos, board, moves);
		return moves;
	}
	std::vector<ChessPosition> getKnightPositions(PieceColour colour, sf::Vector2i pos, const ChessBoard& board) {
		std::vector<ChessPosition> moves;
		moves.reserve(8);
		
		for (char i = 0; i < 8; i++)
		{
			sf::Vector2i kMove = knightMoves[i];
			sf::Vector2i newPos(pos.x + kMove.x, pos.y + kMove.y);
			if (isOutOfBounds(newPos))
				continue;

			//Something's on the board...
			char piece = board.getPiece(newPos.x, newPos.y);
			if (piece != 0) {
				if (piece > 0 && colour == PieceColour::Black)
					moves.push_back(ChessPosition(newPos.x, newPos.y));
				else
					continue;
			}
			else {
				moves.push_back(ChessPosition(newPos.x, newPos.y));
			}
		}
		return moves;
	}
	std::vector<ChessPosition> getQueenPositions(PieceColour colour, sf::Vector2i pos, const ChessBoard& board) {
		std::vector<ChessPosition> moves;
		moves.reserve(8);

		addOrthogonal(colour, pos, board, moves);
		addDiagonal(colour, pos, board, moves);
		return moves;
	}
	std::vector<ChessPosition> getKingPositions(PieceColour colour, sf::Vector2i pos, const ChessBoard& board) {
		return std::vector<ChessPosition>();
	}
}

std::vector<ChessPosition> ChessRules::getValidPositions(const ChessPosition & selectedPosition, const ChessBoard& board)
{
	char pieceValue = board.getPiece(selectedPosition);
	PieceType type = static_cast<PieceType>(abs(pieceValue));
	PieceColour colour = pieceValue < 0 ? PieceColour::Black : PieceColour::White;
	sf::Vector2i position(selectedPosition.getX(), selectedPosition.getY());

	switch (type)
	{
	case PieceType::Pawn:
		return getPawnPositions(colour, position, board);
	case PieceType::Rook:
		return getRookPositions(colour, position, board);
	case PieceType::Knight:
		return getKnightPositions(colour, position, board);
	case PieceType::Bishop:
		return getBishopPositions(colour, position, board);
	case PieceType::Queen:
		return getQueenPositions(colour, position, board);
	case PieceType::King:
		return getKingPositions(colour, position, board);
	default:
		break;
	}

	return std::vector<ChessPosition>();
}

//https://github.com/bdidemus/chess/blob/master/project/Chess/LegalMoveSet.cs
