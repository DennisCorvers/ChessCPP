#pragma once
#include "Entity.h"
#include "ChessMove.h"
#include "Enums.h"

namespace BoardSettings {
	static const char DEFAULTBOARD[64]{
	 -2, -3, -4, -5, -6, -4, -3, -2,
	 -1, -1, -1, -1, -1, -1, -1, -1,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,
	  1,  1,  1,  1,  1,  1,  1,  1,
	  2,  3,  4,  5,  6,  4,  3,  2,
	};
}

class ChessBoard : public Entity
{
private:
	const static char SIZE = 64;

	char m_currentBoard[SIZE];
	const char* m_defaultBoard;
	sf::Texture m_boardTexture;

	void applyMove(const ChessMove newMove);

public:
	ChessBoard(const sf::Texture& boardTexture, const char(&boardData)[SIZE]);
	virtual ~ChessBoard() override;

	const char* getBoard() const;

	inline const char getPiece(char x, char y) const {
		return m_currentBoard[y * 8 + x];
	}

	inline const char getPiece(ChessPosition position) const {
		return getPiece(position.getX(), position.getY());
	}

	bool isValidSelection(const ChessPosition position, const PieceColour playerColour) const;

	bool inputMove(const ChessMove newMove, const std::vector<ChessPosition>* possiblePositions);

	void resetBoard();
	virtual void update(const float& deltaTime) override;
	virtual void render(sf::RenderTarget* const target) override;

	std::vector<ChessPosition> getValidPositions(const ChessPosition& selectedPosition) const;
};

