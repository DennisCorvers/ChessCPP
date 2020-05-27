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
		ChessPiece(const PieceColour colour, const PieceType type);
		virtual bool canMove(const ChessMove move) = 0;
		const PieceColour colour() const;
		const PieceType type() const;

		static ChessPiece* createPiece(PieceColour pieceColour, PieceType pieceType);

		static std::string toString(const PieceType pieceType) {

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
		char m_type;
	};

	class Pawn : public ChessPiece {

	public:
		Pawn(const PieceColour colour, const PieceType type) : ChessPiece(colour, type) { }
		bool canMove(const ChessMove move) override;
	};

	class Knight : public ChessPiece {

	public:
		Knight(const PieceColour colour, const PieceType type) : ChessPiece(colour, type) { }
		bool canMove(const ChessMove move) override;
	};

	class Bishop : public ChessPiece {

	public:
		Bishop(const PieceColour colour, const PieceType type) : ChessPiece(colour, type) { }
		bool canMove(const ChessMove move) override;
	};

	class Rook : public ChessPiece {

	public:
		Rook(const PieceColour colour, const PieceType type) : ChessPiece(colour, type) { }
		bool canMove(const ChessMove move) override;
	};

	class Queen : public ChessPiece {

	public:
		Queen(const PieceColour colour, const PieceType type) : ChessPiece(colour, type) { }
		bool canMove(const ChessMove move) override;
	};

	class King : public ChessPiece {

	public:
		King(const PieceColour colour, const PieceType type) : ChessPiece(colour, type) { }
		bool canMove(const ChessMove move) override;
	};
}

