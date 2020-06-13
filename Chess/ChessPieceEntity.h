#pragma once
#include "Entity.h"
#include "Enums.h"


class ChessPieceEntity : public Entity
{
private:
	char m_pieceData;

public:
	ChessPieceEntity(const PieceColour colour, const PieceType type, const sf::Texture& textureSheet);
	void transform(const PieceColour colour, const PieceType type);

	virtual ~ChessPieceEntity();

	PieceColour getColour() const;
	PieceType getType() const;
};

