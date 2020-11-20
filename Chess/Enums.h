#pragma once
#include "ChessColour.hpp"

namespace sf { class Packet; }

enum struct PieceType : unsigned char
{
	None = 0,
	Pawn = 1,
	Rook = 2,
	Knight = 3,
	Bishop = 4,
	Queen = 5,
	King = 6
};

enum struct ActionType : unsigned short {
	None = 0,
	Normal = 1,
	Check = 2,
	Checkmate = 4,
	Take = 8,
	EnPassant = 16,
	Castling = 32,
	Promotion = 64,
	Draw = 128,
	Stalemate = 256
};

inline ActionType operator|(ActionType a, ActionType b) {
	return static_cast<ActionType>(static_cast<int>(a) | static_cast<int>(b));
}

inline int operator&(ActionType a, ActionType b) {
	return static_cast<int>(a) & static_cast<int>(b);
}

inline ActionType& operator|=(ActionType& a, ActionType b) {
	a = a | b;	return a;
}


sf::Packet& operator <<(sf::Packet& packet, const ActionType& actionType);
sf::Packet& operator >>(sf::Packet& packet, ActionType& actionType);

sf::Packet& operator <<(sf::Packet& packet, const PieceColour& packetType);
sf::Packet& operator >>(sf::Packet& packet, PieceColour& packetType);