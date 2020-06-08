#include <iostream>
#include "ChessMove.h"
#include "SFML/Graphics.hpp"

ChessMove::ChessMove(ChessPosition posFrom, ChessPosition posTo)
{
	m_positionFrom = posFrom;
	m_positionTo = posTo;
}

ChessPosition ChessMove::getPositionFrom() const
{
	return m_positionFrom;
}
ChessPosition ChessMove::getPositionTo() const
{
	return m_positionTo;
}

ChessPosition ChessMove::distance() const
{
	return ChessPosition::distance(m_positionFrom, m_positionTo);
}

float ChessMove::vectorDistance() const
{
		return std::sqrtf(
			std::powf(m_positionFrom.getX() - m_positionTo.getX(), 2) +
			std::powf(m_positionFrom.getY() - m_positionTo.getY(), 2));
}

std::ostream& operator<<(std::ostream& output, const ChessMove& cp)
{
	return output << cp.getPositionFrom() << " -> " << cp.getPositionTo();
}


