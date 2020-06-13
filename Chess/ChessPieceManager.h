#pragma once
#include <functional>
#include "ChessPosition.h"
#include "SFML/Graphics.hpp"

struct ChessMove;
class ChessBoard;
class ChessPieceEntity;
class Entity;

struct MoveAction {
	ChessPosition moveFrom;
	ChessPosition moveTo;
	ChessPieceEntity* movingPiece;
	bool m_isMoving;

	void reset() {
		m_isMoving = false;movingPiece = nullptr;
	}

	bool hasSelection() { return movingPiece; }
};

struct MoveMarkerContainer {
	sf::RectangleShape selectionMarker;
	sf::RectangleShape warningMarker;
	sf::CircleShape moveMarkers[32];
	int count = 0;
};

/**
Handles all player interaction with the pieces on the board.
*/
class ChessPieceManager
{
private:
	const static int PIECECOUNT = 32;
	MoveAction m_moveAction;
	ChessPieceEntity* m_chessPieces[PIECECOUNT];
	sf::FloatRect m_boardSizes;
	sf::FloatRect m_boardCollider;

	MoveMarkerContainer m_markerContainer;

	void snapEntityToBoard(const ChessPosition newPosition, Entity* const piece);
	void snapMarkerToBoard(const ChessPosition newPosition, sf::Shape& marker);
	ChessPieceEntity* getClickedPiece(const sf::Vector2f clickPosition) const;

	sf::Vector2i screenToBoard(const sf::Vector2f mousePosition) const;
	sf::Vector2f boardToScreen(const sf::Vector2i boardPosition) const;
	sf::Vector2f clampToBoard(const sf::Vector2f mousePosition) const;

	void initMarkers();

public:
	ChessPieceManager(const sf::FloatRect boardSizes, const sf::Texture& texture);
	~ChessPieceManager();

	bool hasSelection();
	sf::FloatRect getBoardSizes();

	void update(const float& deltaTime);
	void render(sf::RenderTarget* const target);

	void startSelection(const sf::Vector2f screenPosition, const std::vector<ChessPosition>& possiblePositions);
	void updateSelection(const sf::Vector2f screenPosition);
	bool endSelection(const sf::Vector2f screenPosition, ChessMove& outMove);

	const bool boundsContains(float x, float y) const;
	void syncPieces(const ChessBoard& chessBoard, bool animate);
};

