#pragma once
#include <iostream>
#include "ChessPosition.h"

struct ChessMove {
private:
	ChessPosition m_positionFrom;
	ChessPosition m_positionTo;

public:
	ChessMove() :
		m_positionFrom(0, 0),
		m_positionTo(0, 0) {}

	ChessMove(ChessPosition posFrom, ChessPosition posTo) {
		m_positionFrom = posFrom;
		m_positionTo = posTo;
	}

	inline ChessPosition getPositionFrom() const {
		return m_positionFrom;
	}
	inline ChessPosition getPositionTo() const {
		return m_positionTo;
	}
	ChessPosition distance() const {
		return ChessPosition::distance(m_positionFrom, m_positionTo);
	}

	float vectorDistance() const {
		return std::sqrt(
			std::pow(static_cast<float>(m_positionFrom.x() - m_positionTo.x()), 2) +
			std::pow(static_cast<float>(m_positionFrom.y() - m_positionTo.y()), 2));
	}

	bool operator==(const ChessMove& rhs) const {
		return m_positionFrom == rhs.m_positionFrom && m_positionTo == rhs.m_positionTo;
	}
	bool operator!=(const ChessMove& rhs) const {
		return !(*this == rhs);
	}

	static ChessMove fromChessNotation(const std::string& chessNotation) {
		if (chessNotation.size() < 4)
			throw std::exception("Invalid Chess Notation");
		return ChessMove(
			ChessPosition::fromChessNotation(chessNotation),
			ChessPosition::fromChessNotation(chessNotation.substr(2, 2))
		);
	}
	std::string toChessNotation() const {
		return m_positionFrom.toChessNotation() + m_positionTo.toChessNotation();
	}
	friend std::ostream& operator<<(std::ostream& output, const ChessMove& cp) {
		return output << cp.toChessNotation();
	}
};

