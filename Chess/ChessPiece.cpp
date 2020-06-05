#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Enums.h"

ChessPiece::ChessPiece(const PieceColour colour, const PieceType type, sf::Texture& textureSheet)
{
	m_sprite.setTexture(textureSheet);

	transform(colour, type);
}

ChessPiece::~ChessPiece()
{
}

void ChessPiece::transform(const PieceColour colour, const PieceType type)
{
	m_pieceData = (char)type;
	if (colour == PieceColour::Black) {
		m_pieceData *= -1;
	}

	int size = 56;
	int yPos = colour == PieceColour::White ? 1 : 0;
	int xPos = 0;

	switch (type) {
	case PieceType::Pawn:
		xPos = 5; break;
	case PieceType::Rook:
		xPos = 0; break;
	case PieceType::Knight:
		xPos = 1; break;
	case PieceType::Bishop:
		xPos = 2; break;
	case PieceType::Queen:
		xPos = 3; break;
	case PieceType::King:
		xPos = 4; break;
	}

	m_sprite.setTextureRect(sf::IntRect(size * xPos, size * yPos, size, size));
}

PieceColour ChessPiece::getColour() const
{
	if (m_pieceData < 0) { return PieceColour::Black; }
	return PieceColour::White;
}

PieceType ChessPiece::getType() const
{
	return PieceType(std::abs(m_pieceData));
}
