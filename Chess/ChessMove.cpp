#include <iostream>
#include "ChessMove.h"

namespace Mechanics {
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

	std::ostream& operator<<(std::ostream& output, const ChessMove& cp)
	{
		return output << cp.getPositionFrom() << " -> " << cp.getPositionTo();
	}
}


