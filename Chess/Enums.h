#pragma once
#include <string>

enum struct PieceColour : char
{
	White = 1,
	Black = -1
};
const std::string EnumtoString(const PieceColour pieceColour);

enum struct PieceType : char
{
	Pawn = 1,
	Rook = 2,
	Knight = 3,
	Bishop = 4,
	Queen = 5,
	King = 6
};
const std::string EnumtoString(const PieceType pieceType);

enum struct ActionType : char {
	None = -1,
	Normal = 0,
	Check = 1,
	Checkmate = 2,
	Take = 3,
	EnPassant = 4,
	Castling = 5
};

enum struct SessionState : char {
	None = 0,
	Playing = 1,
	Finished = 2,
	Waiting = 3
};



