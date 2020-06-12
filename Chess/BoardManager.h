#pragma once
#include "ChessMove.h"
#include "SFML/Graphics.hpp"
#include "Enums.h"

class ChessPieceManager;
class ChessBoard;

class BoardManager
{
private:
	ChessPieceManager* m_pieceManager;
	ChessBoard* m_board;
	std::vector<ChessPosition> m_positionCache;

	sf::Vector2i screenToBoardPosition(const sf::Vector2f screenPosition);

public:
	BoardManager(const sf::FloatRect boardSizes, std::map<std::string, sf::Texture>& textures);
	~BoardManager();

	bool hasPieceSelected();
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

