#pragma once
#include <iostream>

namespace Mechanics {
	struct ChessPosition {
	private:
		unsigned char m_position;

	public:
		ChessPosition();
		ChessPosition(unsigned char x, unsigned char y);

		unsigned char getXPosition() const;
		unsigned char getYPosition() const;

		friend std::ostream& operator<<(std::ostream& output, const ChessPosition& cp);
	};
}



