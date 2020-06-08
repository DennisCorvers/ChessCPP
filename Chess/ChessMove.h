#pragma once
#include "ChessPosition.h"


struct ChessMove {
private:
	ChessPosition m_positionFrom;
	ChessPosition m_positionTo;

public:
	ChessMove(ChessPosition posFrom, ChessPosition posTo);

	ChessPosition getPositionFrom() const;
	ChessPosition getPositionTo() const;
	ChessPosition distance() const;
	float vectorDistance() const;

	friend std::ostream& operator<<(std::ostream& output, const ChessMove& cp);
};

