#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "ChessMove.h"
#include "Enums.h"
#include "AssetFlags.h"
#include "Entity.h"

class ChessPieceManager;
class ChessBoard;

class BoardManager
{
private:
	ChessPieceManager* m_pieceManager;
	ChessBoard* m_board;

	sf::Sound m_soundPieceTake;
	sf::Sound m_soundPieceCheck;
	sf::Sound m_soundPieceMove;

public:
	BoardManager(const sf::FloatRect boardSizes, std::map<AssetFlags, sf::Texture>& textures, std::map <AssetFlags, sf::SoundBuffer>& sounds);
	~BoardManager();

	bool isPieceMoving();
	sf::Vector2f getBoardCenter();

	void resetGame();

	/**
	Applies a chess move directly to the board without checking validity.
	*/
	bool inputMove(const ChessMove move, bool animate);
	/**
	Reverses the board to a previous state.
	*/
	void reverseMove(const ChessMove move, bool animate);

	void startSelection(const sf::Vector2f screenPosition, PieceColour playerColour);
	void updateMousePosition(const sf::Vector2f screenPosition);
	void endSelection(const sf::Vector2f screenPosition);

	void update(const float& deltaTime);
	void render(sf::RenderTarget* const target);
};

