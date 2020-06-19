#pragma once
#include <iostream>
#include <stdexcept>

struct ChessPosition {
private:
	unsigned char m_x;
	unsigned char m_y;

public:
	ChessPosition() : m_x(0), m_y(0) {}
	ChessPosition(unsigned char x, unsigned char y) {
		if (x > 7) throw std::invalid_argument("x must be within the range [0, 7]");
		if (y > 7) throw std::invalid_argument("y must be within the range [0, 7]");

		m_x = x;
		m_y = y;
	}

	inline unsigned char x() const {
		return m_x;
	}

	inline unsigned char y() const {
		return m_y;
	}

	static ChessPosition distance(const ChessPosition& from, const ChessPosition& to) {
		return ChessPosition(abs(from.m_x - to.m_x), abs(from.m_y - to.m_y));
	}

	bool operator==(const ChessPosition& rhs) const {
		return m_x == rhs.m_x && m_y == rhs.m_y;
	}
	bool operator!=(const ChessPosition& rhs) const {
		return !(*this == rhs);
	}

	friend std::ostream& operator<<(std::ostream& output, const ChessPosition& cp) {
		return output << (cp.m_x + 65) << (8 - cp.m_y);
	}
};



