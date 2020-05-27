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

		m_position = x << 4;
		m_position = m_position | y;
	}

	unsigned char ChessPosition::X() const
	{
		return m_position >> 4;
	}

	unsigned char ChessPosition::Y() const
	{
		return m_position & 0x0F;
	}

	ChessPosition ChessPosition::distance(const ChessPosition & from, const ChessPosition & to)
	{
		const char x = from.X() - to.X();
		const char y = from.Y() - to.Y();
		return ChessPosition(abs(x), abs(y));
	}

	std::ostream& operator<<(std::ostream& output, const ChessPosition& cp)
	{
		char x = cp.X() + 65;
		int y = cp.Y() + 1;

		return output << x << y;
	}
}



