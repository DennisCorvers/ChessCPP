#include "ChessPiece.h"
#include "Enums.h"

ChessPiece::ChessPiece() {
	m_pieceData = 0;
}

ChessPiece::ChessPiece(const char pieceValue)
{
	m_pieceData = pieceValue;
}

ChessPiece::~ChessPiece()
{
}

void ChessPiece::reset()
{
	m_pieceData = 0;
}

void ChessPiece::setTo(const ChessPiece piece)
{
	m_pieceData = piece.m_pieceData;
}

void ChessPiece::operator=(const int & value)
{
	m_pieceData = value;
}

