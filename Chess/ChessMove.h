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
		return std::sqrtf(
			std::powf(static_cast<float>(m_positionFrom.x() - m_positionTo.x()), 2) +
			std::powf(static_cast<float>(m_positionFrom.y() - m_positionTo.y()), 2));
	}

	friend std::ostream& operator<<(std::ostream& output, const ChessMove& cp) {
		return output << cp.getPositionFrom() << " -> " << cp.getPositionTo();
	}
};

