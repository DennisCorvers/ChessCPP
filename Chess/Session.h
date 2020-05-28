#pragma once

#include "ChessBoard.h";

using Mechanics::ChessBoard;

enum struct SessionState :char {
	None = 0,
	Playing = 1,
	Finished = 2
};

class Session
{
public:

	Session();
	~Session();

	const ChessBoard& Board() const;

	void startSession();
	void inputMove();

private:
	ChessBoard* m_board;
	SessionState m_state;
	//List of past moves...
	//Network (Client) information...
};



