#pragma once

#include "ChessBoard.h";

using namespace Mechanics;

enum struct SessionState :char {
	None = 0,
	Playing = 1,
	Finished = 2,
	Waiting = 3
};

class Session
{
public:

	Session();
	~Session();

	const ChessBoard& Board() const;

	void startSession();
	void inputMove(ChessMove move);

private:
	ChessBoard* m_board;
	SessionState m_state;
	std::vector<int> m_chessMoves;

	//Network (Client) information...
};



