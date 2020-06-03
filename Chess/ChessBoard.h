#pragma once
#include "Entity.h"
#include "Enums.h"
#include "ChessMove.h"
#include "ChessPosition.h"

class ChessPiece;

namespace Mechanics {
	struct ChessMove;
	struct ChessPosition;
}

class ChessBoard : public Entity
{
private:
	static const char m_defaultBoard[8][8];

	ChessPiece* m_board[64];
	sf::Vector2f m_boardOffset;

	std::map<std::string, sf::Texture>& textures;
	void initBoard();
	void createPiece(PieceColour colour, PieceType pieceType, const sf::Vector2f position);

public:
	ChessBoard(sf::Vector2f boardOffset, std::map<std::string, sf::Texture>& textures);
	virtual ~ChessBoard() override;

	void resetBoard();
	virtual void update() override;
	virtual void render(sf::RenderTarget* target) override;
};

