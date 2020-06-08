#pragma once

class ChessPieceManager;
class ChessBoard;

class BoardManager
{
private:
	ChessPieceManager* m_pieceManager;
	ChessBoard* m_board;

public:
	BoardManager();
	~BoardManager();
};

