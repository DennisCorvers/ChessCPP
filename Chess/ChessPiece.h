#pragma once
#include "Entity.h"
#include "Enums.h"

class ChessPiece
{
private:
	char m_pieceData;

public:
	ChessPiece();
	ChessPiece(const char pieceValue);
	virtual ~ChessPiece();

	inline bool isEmpty() const {
		return m_pieceData == 0;
	}
	void reset();
	void setTo(const ChessPiece piece);

	inline PieceColour getColour() const
	{
		if (m_pieceData < 0) { return PieceColour::Black; }
		return PieceColour::White;
	}
	inline PieceType getType() const
	{
		return static_cast<PieceType>(abs(m_pieceData));
	}

	void operator=(const int& value);
};


