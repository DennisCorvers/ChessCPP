#include "Session.h"



Session::Session()
{
	m_board = new ChessBoard();
	m_state = SessionState::None;
}


Session::~Session()
{
	delete m_board;
}

const ChessBoard& Session::Board() const
{
	return *m_board;
}

void Session::startSession()
{
	m_board->resetBoard();
	m_state = SessionState::Waiting;
}

void Session::inputMove(ChessMove move)
{

}

