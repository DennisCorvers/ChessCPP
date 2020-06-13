#include "ChessBoard.h"
#include "ChessRules.h"

namespace {
	sf::Vector2i kingMoves[8]{
	sf::Vector2i(-1,1),
	sf::Vector2i(0,1),
	sf::Vector2i(1,1),
	sf::Vector2i(1,0),
	sf::Vector2i(1,-1),
	sf::Vector2i(0,-1),
	sf::Vector2i(-1,-1),
	sf::Vector2i(-1,0)
	};

	sf::Vector2i knightMoves[8]{

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

			//Something's on the board...
			validMoves.push_back(ChessPosition(newPos.x, newPos.y)); continue;
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
		std::vector<ChessPosition> ret(8);
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
		return std::vector<ChessPosition>();
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
