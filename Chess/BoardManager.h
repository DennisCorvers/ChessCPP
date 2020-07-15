#pragma once
#include "Enums.h"
#include "ChessMove.h"
#include "assetNames.h"
#include "Entity.h"

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

class TextureManager;
class ChessPieceManager;
class ChessBoard;
class SoundManager;
struct ChessAction;

class BoardManager
{
private:
	std::unique_ptr<ChessPieceManager> m_pieceManager;
	std::unique_ptr<ChessBoard> m_board;

	SoundManager& m_soundManager;

	bool m_hasCachedMove = false;
	ChessMove m_cachedMove;

	void handleSound(const ActionType chessAction, bool playSound);
	bool validateMove(const ChessMove move) const;

public:
	BoardManager(
		TextureManager& textureManager,
		SoundManager& soundManager,
		int pixelSize,
		PieceColour startOrientation = PieceColour::White);
	~BoardManager();

	const sf::Vector2f getBoardCenter();

	void resetGame();

	void flipBoard(PieceColour orientation);
	void flipBoard();

	bool inputMove(const ChessMove move, bool animate);

	void startSelection(const sf::Vector2f screenPosition, bool forceColour) const;
	void startSelection(const sf::Vector2f screenPosition, PieceColour validColour) const;
	void updateMousePosition(const sf::Vector2f screenPosition) const;
	bool endSelection(const sf::Vector2f screenPosition, ChessMove& outMove);

	void update(const float& deltaTime);
	void render(sf::RenderTarget& target);
};

