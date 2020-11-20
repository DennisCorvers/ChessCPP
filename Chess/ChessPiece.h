#pragma once
#include "Entity.h"
#include "Enums.h"

class ChessPiece
{
private:
	static const unsigned char type_mask = 0b00111111;
	static const unsigned char colour_mask = 0b01000000;
	static const unsigned char moved_mask = 0b10000000;

	unsigned char m_pieceData = 0;

	void setType(PieceType type);
	void setColour(PieceColour colour);

public:
	ChessPiece();
	ChessPiece(const char pieceValue);
	ChessPiece(PieceColour colour, PieceType pieceType);
	virtual ~ChessPiece();

	inline bool isEmpty() const {
		return (m_pieceData & type_mask) == 0;
	}
	inline bool hasMoved() const {
		return (m_pieceData & moved_mask) == moved_mask;
	}
	void setMoved() {
		m_pieceData |= moved_mask;
	}

	void reset();
	void setTo(const ChessPiece piece, bool hasMoved = true);
	void setTo(PieceType newType);

	inline PieceColour getColour() const
	{
		if ((m_pieceData & colour_mask) == colour_mask)
			return PieceColour::White;

		return PieceColour::Black;
	}
	inline PieceType getType() const
	{
		return static_cast<PieceType>(m_pieceData & type_mask);
	}

	char getFEN() const;

	void netSerialize(sf::Packet& packet, bool isWriting) {
		if (isWriting)
			packet << m_pieceData;
		else
			packet >> m_pieceData;
	}
};


