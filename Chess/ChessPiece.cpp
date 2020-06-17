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

void ChessPiece::reset()
{
	m_pieceData = 0;
}

void ChessPiece::setTo(const ChessPiece piece, bool hasMoved = true)
{
	m_pieceData = piece.m_pieceData;
	if (hasMoved)
		setMoved();
}

void ChessPiece::setTo(PieceType newType)
{
	setType(newType);
}

