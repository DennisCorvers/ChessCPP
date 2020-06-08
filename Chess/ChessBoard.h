#pragma once
#include "Entity.h"
#include "ChessMove.h"
#include "ChessPieceManager.h"

namespace BoardSettings {
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

class ChessBoard : public Entity
{
private:
	char m_board[64];
	ChessPieceManager* m_pieceManager;

	const sf::FloatRect m_boardSizes;
	std::map<std::string, sf::Texture>& textures;

	std::vector<ChessPosition> m_validPositions;

public:
	ChessBoard(sf::FloatRect boardsize, std::map<std::string, sf::Texture>& textures);
	virtual ~ChessBoard() override;

	bool hasPieceSelected() const;

	void resetBoard();
	virtual void update(const float& deltaTime) override;
	virtual void render(sf::RenderTarget* const target) override;

	void updateMousePosition(const sf::Vector2f screenPosition, const MouseEvent mEvent);
	bool validateMove(const ChessMove& move);
	std::vector<ChessPosition> getValidPositions(const ChessPosition& selectedPosition);
};

