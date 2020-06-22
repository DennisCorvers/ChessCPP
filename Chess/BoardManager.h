#pragma once
#include "ChessMove.h"
#include "AssetFlags.h"
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

class ChessPieceManager;
class ChessBoard;
struct ChessAction;

class BoardManager
{
private:
	std::unique_ptr<ChessPieceManager> m_pieceManager;
	std::unique_ptr<ChessBoard> m_board;

	std::map<ActionType, sf::Sound> m_soundMap;

	bool m_hasCachedMove = false;
	ChessMove m_cachedMove;

	void handleSound(const ActionType chessAction, bool playSound);
	bool validateMove(const ChessMove move) const;

public:
	BoardManager(const sf::FloatRect boardSizes, 
		std::map<AssetFlags, sf::Texture>& textures, 
		std::map <AssetFlags, sf::SoundBuffer>& sounds,
		PieceColour startOrientation = PieceColour::White);
	~BoardManager();

	sf::Vector2f getBoardCenter();

	void resetGame();

	void flipBoard(PieceColour orientation);
	void flipBoard();

	bool inputMove(const ChessMove move, bool animate);

	void startSelection(const sf::Vector2f screenPosition, PieceColour playerColour) const;
	void updateMousePosition(const sf::Vector2f screenPosition) const;
	bool endSelection(const sf::Vector2f screenPosition, ChessMove& outMove);

	void update(const float& deltaTime);
	void render(sf::RenderTarget* const target);
};

