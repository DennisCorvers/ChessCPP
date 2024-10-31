#pragma once
#include "Entities/Entity.h"

class ChessPieceEntity : public Entity
{
private:
	static int SIZE;
	char m_pieceData;

public:
	ChessPieceEntity(const PieceColour colour, const PieceType type, const sf::Texture& textureSheet);
	void transform(const PieceColour colour, const PieceType type);

	virtual ~ChessPieceEntity();

	PieceColour getColour() const;
	PieceType getType() const;

	char xPos;
	char yPos;
};

