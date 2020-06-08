#include "ChessPosition.h"
#include <stdexcept>


ChessPosition::ChessPosition()
{	}

ChessPosition::ChessPosition(unsigned char x, unsigned char y)
{
	if (x > 7) throw std::invalid_argument("x must be within the range [0, 7]");
	if (y > 7) throw std::invalid_argument("y must be within the range [0, 7]");

	m_position = x << 4;
	m_position = m_position | y;
}

unsigned char ChessPosition::getX() const
{
	return m_position >> 4;
}

unsigned char ChessPosition::getY() const
{
	return m_position & 0x0F;
}

ChessPosition ChessPosition::distance(const ChessPosition & from, const ChessPosition & to)
{
	const char x = from.getX() - to.getX();
	const char y = from.getY() - to.getY();
	return ChessPosition(abs(x), abs(y));
}

bool ChessPosition::operator==(const ChessPosition & rhs)
{
	return m_position == rhs.m_position;
}

std::ostream& operator<<(std::ostream& output, const ChessPosition& cp)
{
	char x = cp.getX() + 65;
	int y = (8 - cp.getY());

	return output << x << y;
}

bool operator==(const ChessPosition & lhs, const ChessPosition & rhs)
{
	return lhs.m_position == rhs.m_position;
}



