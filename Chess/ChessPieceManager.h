#pragma once
#include "SFML/Graphics.hpp"
#include <memory>
#include "ChessPosition.h"
#include "Entity.h"
#include "AssetFlags.h"


struct ChessMove;
class ChessBoard;
class ChessPieceEntity;
class Entity;

struct MoveAction {
	ChessPosition moveFrom;
	ChessPosition moveTo;
	ChessPieceEntity* movingPiece;
	std::vector<ChessPosition> validPositions;

	bool m_isMoving;

	void reset() {
		m_isMoving = false;
		movingPiece = nullptr;
	}

	bool hasSelection()
	{
		return movingPiece;
	}
};

struct MoveMarkerContainer {
private:
	sf::CircleShape m_moveMarkers[32];
	int index = 0;
	int renderCount = 0;

	inline void moveShape(sf::Shape& shape, sf::Vector2f position) {
		shape.setPosition(
			position.x - shape.getGlobalBounds().width / 2.f,
			position.y - shape.getGlobalBounds().height / 2.f
		);
	}

public:
	MoveMarkerContainer(const sf::CircleShape& moveMarker) {
		for (size_t i = 0; i < 32; i++)
			m_moveMarkers[i] = moveMarker;
	}

	inline void moveMarker(sf::Vector2f position) {
		moveShape(m_moveMarkers[index++], position);
	}

	void finalize() {
		renderCount = index;
		index = 0;
	}

	void render(sf::RenderTarget* const target) {
		for (size_t i = 0; i < renderCount; i++)
			target->draw(m_moveMarkers[i]);
	}
};

/**
Handles all player interaction with the pieces on the board.
*/
class ChessPieceManager : public Entity
{
private:
	const static int PIECECOUNT = 32;
	MoveAction m_moveAction;
	ChessPieceEntity* m_chessPieces[PIECECOUNT];
	sf::FloatRect m_boardSizes;
	sf::FloatRect m_boardCollider;

	std::unique_ptr<MoveMarkerContainer> m_markerContainer;
	sf::RectangleShape m_selectionMarker;
	sf::RectangleShape m_warningMarker;

	void snapEntityToBoard(const ChessPosition newPosition, Entity* const piece);
	void snapMarkerToBoard(const ChessPosition newPosition, sf::Shape& marker);
	ChessPieceEntity* getClickedPiece(const sf::Vector2f clickPosition) const;

	sf::Vector2i screenToBoard(const sf::Vector2f mousePosition) const;
	sf::Vector2f boardToScreen(const sf::Vector2i boardPosition) const;
	sf::Vector2f clampToBoard(const sf::Vector2f mousePosition) const;

	void selectChessPiece(const MoveAction& moveData, const ChessBoard& board);
	void initMarkers();

public:
	ChessPieceManager(const sf::FloatRect boardSizes, std::map<AssetFlags, sf::Texture>& textures);
	virtual ~ChessPieceManager() override;

	bool isPieceMoving();

	virtual void update(const float& deltaTime) override;
	virtual void render(sf::RenderTarget* const target) override;

	void startSelection(const sf::Vector2f screenPosition, const ChessBoard& board);
	void updateSelection(const sf::Vector2f screenPosition);
	bool endSelection(const sf::Vector2f screenPosition, ChessMove& outMove);

	const bool boundsContains(float x, float y) const;
	void syncPieces(const ChessBoard& chessBoard, bool animate);
};

