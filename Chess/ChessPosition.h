#pragma once
#include <iostream>

struct ChessPosition {
private:
	unsigned char m_position;

public:
	ChessPosition();
	ChessPosition(unsigned char x, unsigned char y);

	unsigned char getX() const;
	unsigned char getY() const;

	static ChessPosition distance(const ChessPosition& from, const ChessPosition& to);

	bool operator==(const ChessPosition& rhs) const;
	bool operator!=(const ChessPosition& rhs) const;

	friend std::ostream& operator<<(std::ostream& output, const ChessPosition& cp);
};



