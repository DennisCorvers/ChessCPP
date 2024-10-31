#pragma once

enum struct PieceColour : char
{
	White = 1,
	Black = -1
};

struct ChessColour {
private:
	PieceColour m_pieceColour;

public:
	ChessColour() : m_pieceColour(PieceColour::White) { }
	ChessColour(PieceColour colour) : m_pieceColour(colour) { }

	PieceColour swap() {
		m_pieceColour = m_pieceColour == PieceColour::Black ? PieceColour::White : PieceColour::Black;
		return m_pieceColour;
	}

	PieceColour opposite() {
		return m_pieceColour == PieceColour::Black ? PieceColour::White : PieceColour::Black;
	}

	operator PieceColour() const {
		return m_pieceColour;
	}

	void operator= (PieceColour colour) {
		m_pieceColour = colour;
	}
};