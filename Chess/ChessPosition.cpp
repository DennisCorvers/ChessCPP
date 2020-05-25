#include "ChessPosition.h"
#include <stdexcept>


namespace Mechanics {
	ChessPosition::ChessPosition()
	{	}

	ChessPosition::ChessPosition(unsigned char x, unsigned char y)
	{
		//Converts capital and small letters to 0-7 value.
		if (x > 72) x -= 97;
		if (x > 7) x -= 65;

		if (x > 7) throw std::invalid_argument("x must be within the range [0, 7]");
		if (y > 7) throw std::invalid_argument("y must be within the range [0, 7]");

		m_position = y << 4;
		m_position = m_position || x;
	}

	unsigned char ChessPosition::getXPosition() const
	{
		return m_position & 0b00001111;
	}

	unsigned char ChessPosition::getYPosition() const
	{
		return m_position & 0b11110000;
	}

	std::ostream& operator<<(std::ostream& output, const ChessPosition& cp)
	{
		char x = cp.getXPosition() + 65;
		int y = cp.getYPosition() + 1;

		return output << x << y;
	}
}



