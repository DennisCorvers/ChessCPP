#pragma once
//#include "ChessMove.h"
#include "Entity.h"
#include "Enums.h"



class ChessPiece : public Entity
{
private:
	char m_pieceData;

public:
	ChessPiece(const PieceColour colour, const PieceType type, sf::Texture& textureSheet);
	void transform(const PieceColour colour, const PieceType type);

	virtual ~ChessPiece();

	PieceColour getColour() const;
	PieceType getType() const;
};

