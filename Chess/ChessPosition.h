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
		ASSERT(x < 8, "x must be within the range [0, 7]");
		ASSERT(y < 8, "y must be within the range [0, 7]");

		m_x = x;
		m_y = y;
	}

	inline unsigned char x() const {
		return m_x;
	}
	inline unsigned char y() const {
		return m_y;
	}

	bool operator==(const ChessPosition& rhs) const {
		return m_x == rhs.m_x && m_y == rhs.m_y;
	}
	bool operator!=(const ChessPosition& rhs) const {
		return !(*this == rhs);
	}

	std::string toChessNotation() const {
		std::string val; val.reserve(2);
		val += static_cast<char>(m_x + 97);
		val += static_cast<char>(8 - m_y + 48);
		return val;
	}

	friend std::ostream& operator<<(std::ostream& output, const ChessPosition& cp) {
		return output << cp.toChessNotation();
	}

	static ChessPosition fromChessNotation(const std::string& chessNotation) {
		if (chessNotation.size() < 2) throw std::exception("Invalid ChessNotation");
		char x = chessNotation[0];
		char y = chessNotation[1];
		if (x > 72) x -= 97; //Lower case to num
		if (x > 7) x -= 65;	//Upper case to num

		y = 8 - (y - 48);

		return ChessPosition(x, y);
	}
	static ChessPosition distance(const ChessPosition& from, const ChessPosition& to) {
		return ChessPosition(abs(from.m_x - to.m_x), abs(from.m_y - to.m_y));
	}

	void netSerialize(sf::Packet& packet, bool isWriting) {
		if (isWriting) {
			packet << m_x;
			packet << m_y;
		}
		else {
			packet >> m_x;
			packet >> m_y;
		}
	}
};



