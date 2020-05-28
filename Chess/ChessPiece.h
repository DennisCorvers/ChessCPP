#pragma once
#include "ChessMove.h"

namespace Mechanics {

	enum struct PieceColour : char
	{
		White = 1,
		Black = -1
	};

	enum struct PieceType : char
	{
		None = 0,
		Pawn = 1,
		Rook = 2,
		Knight = 3,
		Bishop = 4,
		Queen = 5,
		King = 6
	};

	class ChessPiece {

	public:
		ChessPiece();
		ChessPiece(const PieceColour colour, const PieceType type);
		ChessPiece(const char pieceData);

		bool canMove(const ChessMove move) const;
		PieceColour getColour() const;
		PieceType getType() const;

		static std::string toString(const PieceType pieceType) {
			switch (pieceType)
			{
			case PieceType::Pawn:
				return "Pawn";
			case PieceType::Rook:
				return "Rook";
			case PieceType::Knight:
				return "Knight";
			case PieceType::Bishop:
				return "Bishop";
			case PieceType::Queen:
				return "Queen";
			case PieceType::King:
				return "King";
			case PieceType::None:
				return "Empty";
			}
		}
		static std::string toString(const PieceColour pieceColour) {
			switch (pieceColour)
			{
			case PieceColour::Black:
				return "Black";
			case PieceColour::White:
				return "White";
			}
		}

	protected:
		char m_pieceData;
	};

	class Pawn {

	public:
		static bool canMove(const ChessMove move);
	};

	class Knight {

	public:
		static bool canMove(const ChessMove move);
	};

	class Bishop {

	public:
		static bool canMove(const ChessMove move);
	};

	class Rook {

	public:
		static bool canMove(const ChessMove move);
	};

	class Queen {

	public:
		static bool canMove(const ChessMove move);
	};

	class King {

	public:
		static bool canMove(const ChessMove move);
	};
}

