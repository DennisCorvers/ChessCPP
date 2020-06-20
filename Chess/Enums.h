#pragma once

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



enum struct ActionType : char {
	None = -1,
	Normal = 0,
	Check = 1,
	Checkmate = 2,
	Take = 3,
	EnPassant = 4,
	Castling = 5,
	Promotion = 6
};

enum struct SessionState : char {
	None = 0,
	Playing = 1,
	Finished = 2,
	Waiting = 3
};

namespace Enums {

	const std::string EnumtoString(const PieceType pieceType);

	const std::string EnumtoString(const PieceColour pieceColour);
}