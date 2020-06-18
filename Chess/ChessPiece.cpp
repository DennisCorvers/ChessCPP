#include "ChessPiece.h"
#include "Enums.h"

ChessPiece::ChessPiece() {
	m_pieceData = 0;
}

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


