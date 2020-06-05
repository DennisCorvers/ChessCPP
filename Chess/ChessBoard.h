#pragma once
#include "Entity.h"
#include "Enums.h"
#include "ChessMove.h"
#include "ChessPosition.h"
#include "ChessPiece.h"

namespace Mechanics {
	struct ChessMove;
	struct ChessPosition;
}

namespace BoardSettings {
	static const char PIECECOUNT = 32;
	static const char DEFAULTBOARD[8][8]{
	{ -2, -3, -4, -5, -6, -4, -3, -2},
	{ -1, -1, -1, -1, -1, -1, -1, -1 },
	{  0,  0,  0,  0,  0,  0,  0,  0 },
	{  0,  0,  0,  0,  0,  0,  0,  0 },
	{  0,  0,  0,  0,  0,  0,  0,  0 },
	{  0,  0,  0,  0,  0,  0,  0,  0 },
	{  1,  1,  1,  1,  1,  1,  1,  1 },
	{  2,  3,  4,  5,  6,  4,  3,  2 },
	};
}

struct BoardSizes {

	BoardSizes(float leftOffset, float topOffset, float fieldSize) {
		this->leftOffset = leftOffset;
		this->topOffset = topOffset;
		this->fieldXSize = fieldSize;
		this->fieldYSize = fieldSize;
	}

	BoardSizes(float leftOffset, float topOffset, float fieldXSize, float fieldYSize) {
		this->leftOffset = leftOffset;
		this->topOffset = topOffset;
		this->fieldXSize = fieldXSize;
		this->fieldYSize = fieldYSize;
	}

	float leftOffset;
	float topOffset;
	float fieldXSize;
	float fieldYSize;
};

class ChessBoard : public Entity
{
private:
	char m_board[64];
	ChessPiece* m_pieces[BoardSettings::PIECECOUNT];

	const BoardSizes m_boardSizes;
	std::map<std::string, sf::Texture>& textures;

	void initPieces();
	void initBoard();

public:
	ChessBoard(BoardSizes boardsize, std::map<std::string, sf::Texture>& textures);
	virtual ~ChessBoard() override;

	void resetBoard();
	virtual void update() override;
	virtual void render(sf::RenderTarget* target) override;

	void snapPieceToBoard(const ChessPosition newPosition, ChessPiece* piece);
	ChessPiece* getClickedPiece(const sf::Vector2f clickPosition) const;
	bool tryMove(const ChessMove move);
	bool tryScreenToBoard(const sf::Vector2f mousePosition, sf::Vector2i* const boardPosition) const;
	sf::Vector2f boardToScreen(const sf::Vector2i boardPosition) const;
};

