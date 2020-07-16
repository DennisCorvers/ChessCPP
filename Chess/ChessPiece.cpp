#include "pch.h"
#include "ChessPiece.h"
#include "Enums.h"

ChessPiece::ChessPiece() :
	m_pieceData(0)
{}

ChessPiece::ChessPiece(const char pieceValue)
{
	if (pieceValue < 0)
		setColour(PieceColour::Black);
	else
		setColour(PieceColour::White);
	setType(static_cast<PieceType>(abs(pieceValue)));
}

ChessPiece::ChessPiece(PieceColour colour, PieceType pieceType)
{
	setColour(colour);
	setType(pieceType);
}

ChessPiece::~ChessPiece()
{

}

char ChessPiece::getFEN() const
{
	char value = 0;
	switch (getType()) {
	case PieceType::Pawn:
		value = 'P'; break;
	case PieceType::Rook:
		value = 'R'; break;
	case PieceType::Knight:
		value = 'N'; break;
	case PieceType::Bishop:
		value = 'B'; break;
	case PieceType::Queen:
		value = 'Q'; break;
	case PieceType::King:
		value = 'K'; break;
	default:
		return 0;
	}

	if (getColour() == PieceColour::Black)
		value += 32;

	return value;
}


