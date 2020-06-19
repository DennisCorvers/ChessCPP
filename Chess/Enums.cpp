#include "pch.h"

const std::string Enums::EnumtoString(const PieceType pieceType) {
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
	default:
		return "";
	}
}

const std::string Enums::EnumtoString(const PieceColour pieceColour) {
	switch (pieceColour)
	{
	case PieceColour::Black:
		return "Black";
	case PieceColour::White:
		return "White";
	default:
		return "";
	}
}
