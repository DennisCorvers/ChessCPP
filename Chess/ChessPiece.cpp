#include "ChessPiece.h"

namespace Mechanics {


	ChessPiece::ChessPiece(const PieceColour colour, const PieceType type)
	{
		m_type = (char)type;
		if (colour == PieceColour::Black) {
			m_type *= -1;
		}
	}

	const PieceColour ChessPiece::colour() const
	{
		if (m_type < 0) { return PieceColour::Black; }
		return PieceColour::White;
	}

	const PieceType ChessPiece::type() const
	{
		return PieceType(std::abs(m_type));
	}

	ChessPiece* ChessPiece::createPiece(PieceColour pieceColour, PieceType pieceType)
	{
		switch (pieceType)
		{
		case Mechanics::PieceType::None:
			return NULL;
			break;
		case Mechanics::PieceType::Pawn:
			return new Pawn(pieceColour, pieceType);
			break;
		case Mechanics::PieceType::Rook:
			return new Rook(pieceColour, pieceType);
			break;
		case Mechanics::PieceType::Knight:
			return new Knight(pieceColour, pieceType);
			break;
		case Mechanics::PieceType::Bishop:
			return new Bishop(pieceColour, pieceType);
			break;
		case Mechanics::PieceType::Queen:
			return new Queen(pieceColour, pieceType);
			break;
		case Mechanics::PieceType::King:
			return new King(pieceColour, pieceType);
			break;
		}
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
