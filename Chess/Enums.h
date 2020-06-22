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
	None = 0,
	Normal = 1,
	Check = 2,
	Checkmate = 4,
	Take = 8,
	EnPassant = 16,
	Castling = 32,
	Promotion = 64
};

inline ActionType operator|(ActionType a, ActionType b) {
	return static_cast<ActionType>(static_cast<char>(a) | static_cast<char>(b));
}

inline char operator&(ActionType a, ActionType b) {
	return static_cast<char>(a) & static_cast<char>(b);
}

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