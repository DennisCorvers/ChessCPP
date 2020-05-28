#include "ChessPiece.h"

namespace Mechanics {
	ChessPiece::ChessPiece()
	{
	}
	ChessPiece::ChessPiece(const PieceColour colour, const PieceType type)
	{
		m_pieceData = (char)type;
		if (colour == PieceColour::Black) {
			m_pieceData *= -1;
		}
	}

	ChessPiece::ChessPiece(const char pieceData) {
		if (pieceData < -8 || pieceData > 8) {
			throw std::invalid_argument("x must be within the range [-8, 8]");
		}
		m_pieceData = pieceData;
	}

	bool ChessPiece::canMove(const ChessMove move) const
	{
		switch (getType())
		{
		case Mechanics::PieceType::None:
			return false;
		case Mechanics::PieceType::Pawn:
			return Pawn::canMove(move);
		case Mechanics::PieceType::Rook:
			return Rook::canMove(move);
		case Mechanics::PieceType::Knight:
			return Knight::canMove(move);
		case Mechanics::PieceType::Bishop:
			return Bishop::canMove(move);
		case Mechanics::PieceType::Queen:
			return Queen::canMove(move);
		case Mechanics::PieceType::King:
			return King::canMove(move);
		}

		return false;
	}

	PieceColour ChessPiece::getColour() const
	{
		if (m_pieceData < 0) { return PieceColour::Black; }
		return PieceColour::White;
	}
	PieceType ChessPiece::getType() const
	{
		return PieceType(std::abs(m_pieceData));
	}


	bool Pawn::canMove(const ChessMove move)
	{
		return false;
	}

	bool Knight::canMove(const ChessMove move)
	{
		return false;
	}

	bool Bishop::canMove(const ChessMove move)
	{
		return false;
	}

	bool Rook::canMove(const ChessMove move)
	{
		return false;
	}

	bool Queen::canMove(const ChessMove move)
	{
		return false;
	}

	bool King::canMove(const ChessMove move)
	{
		return false;
	}

}
