#pragma once
#include "ChessMove.h"
#include "AssetFlags.h"
#include "Entity.h"

class ChessPieceManager;
class ChessBoard;
struct ChessAction;

class BoardManager
{
private:
	std::unique_ptr<ChessPieceManager> m_pieceManager;
	std::unique_ptr<ChessBoard> m_board;

	sf::Sound m_soundPieceTake;
	sf::Sound m_soundPieceCheck;
	sf::Sound m_soundPieceMove;

	bool m_hasCachedMove = false;
	ChessMove m_cachedMove;

	void handleSound(const ActionType chessAction, bool playSound);
	bool validateMove(const ChessMove move) const;

public:
	BoardManager(const sf::FloatRect boardSizes, std::map<AssetFlags, sf::Texture>& textures, std::map <AssetFlags, sf::SoundBuffer>& sounds);
	~BoardManager();

	bool isPieceMoving();
	sf::Vector2f getBoardCenter();

	void resetGame();

	bool inputMove(const ChessMove move, bool animate);

	void startSelection(const sf::Vector2f screenPosition, PieceColour playerColour) const;
	void updateMousePosition(const sf::Vector2f screenPosition) const;
	bool endSelection(const sf::Vector2f screenPosition, ChessMove& outMove);

	void update(const float& deltaTime);
	void render(sf::RenderTarget* const target);
};

