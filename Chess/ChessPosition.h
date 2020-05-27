#pragma once
#include <iostream>

namespace Mechanics {
	struct ChessPosition {
	private:
		unsigned char m_position;

	public:
		ChessPosition();
		ChessPosition(unsigned char x, unsigned char y);

		unsigned char X() const;
		unsigned char Y() const;

		static ChessPosition distance(const ChessPosition& from, const ChessPosition& to);

		friend std::ostream& operator<<(std::ostream& output, const ChessPosition& cp);
	};
}



