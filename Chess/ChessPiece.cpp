#include "pch.h"
#include "ChessPiece.h"
#include "Enums.h"

void ChessPiece::setType(PieceType type) {
	m_pieceData = (m_pieceData & (~type_mask)) | (static_cast<char>(type));
}

void ChessPiece::setColour(PieceColour colour) {
	char value = colour == PieceColour::Black ? 0 : colour_mask;
	m_pieceData = (m_pieceData & (~colour_mask)) | value;
}

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

void ChessPiece::reset() {
	m_pieceData = 0;
}

void ChessPiece::setTo(const ChessPiece piece, bool hasMoved) {
	m_pieceData = piece.m_pieceData;

	if (hasMoved)
		setMoved();
}

void ChessPiece::setTo(PieceType newType) {
	setType(newType);
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


