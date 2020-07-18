#pragma once
#include "SFML/Graphics.hpp"
#include "ChessPosition.h"
#include "Entity.h"
#include "assetNames.h"

struct ChessMove;
struct ChessAction;
class AnimatorSystem;
class ChessBoard;
class ChessPieceEntity;
class Entity;
class TextureManager;

struct MoveAction {
private:
	bool m_isMoving;

public:
	ChessPosition moveFrom;
	ChessPosition moveTo;
	ChessPieceEntity* movingPiece;
	std::vector<ChessPosition> validPositions;
	char selectCount;

	inline void setMoving(bool state) {
		m_isMoving = state;
	}

	inline void reset() {
		m_isMoving = false;
		movingPiece = nullptr;
		selectCount = 0;
	}

	inline void newSelection(ChessPosition moveFrom, ChessPieceEntity* movingPiece) {
		this->moveFrom = moveFrom;
		this->movingPiece = movingPiece;
		selectCount = 0;
	}

	inline bool hasSelection() {
		return movingPiece != nullptr;
	}

	inline bool isMoving() {
		return movingPiece && m_isMoving;
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

	void render(sf::RenderTarget& target) {
		for (size_t i = 0; i < renderCount; i++)
			target.draw(m_moveMarkers[i]);
	}
};

/**
Handles all player interaction with the pieces on the board.
*/
class ChessPieceManager : public Entity
{
private:
	const static int PIECECOUNT = 32;
	ChessPieceEntity* m_chessPieces[PIECECOUNT];
	const ChessBoard* m_board;

	MoveAction m_moveAction;
	sf::Vector2f m_squareSize;
	sf::Vector2f m_lastScreenPosition;
	PieceColour m_viewOrientation;

	std::unique_ptr<AnimatorSystem> m_animatorSystem;
	std::unique_ptr<MoveMarkerContainer> m_markerContainer;
	sf::RectangleShape m_selectionMarker;
	sf::RectangleShape m_warningMarker;

	void snapPieceToBoard(const ChessPosition newPosition, ChessPieceEntity& piece);
	void snapMarkerToBoard(const ChessPosition newPosition, sf::Shape& marker);
	ChessPieceEntity* getClickedPiece(const sf::Vector2f clickPosition) const;
	ChessPieceEntity* getClickedPiece(const ChessPosition chessPosition) const;

	sf::Vector2i screenToBoard(const sf::Vector2f mousePosition) const;
	sf::Vector2f boardToScreen(const sf::Vector2i boardPosition) const;
	sf::Vector2f clampToBoard(const sf::Vector2f mousePosition) const;

	inline sf::Vector2i transformVector(const sf::Vector2i position) const {
		if (m_viewOrientation == PieceColour::Black)
			return sf::Vector2i(7 - position.x, 7 - position.y);
		return position;
	}

	void selectChessPiece(const MoveAction& moveData);
	void initMarkers();

	void animationCallback();

public:
	ChessPieceManager(TextureManager& textureManager, int pixelSize, PieceColour orientation);
	virtual ~ChessPieceManager() override;

	virtual void update(const float& deltaTime) override;
	virtual void render(sf::RenderTarget& target) override;

	void startSelection(const sf::Vector2f screenPosition, int forceColour = 0);
	void updateSelection(const sf::Vector2f screenPosition);
	bool endSelection(const sf::Vector2f screenPosition, ChessMove& outMove);

	void flipBoard(const PieceColour orientation, bool forceRefresh = false);
	void flipBoard();
	void updateBoard(const ChessBoard& board);
	void refreshBoard(const ChessBoard& board);
	void refreshBoard();
	void inputMove(const ChessBoard& board, bool animate = false);
	void inputMove(const ChessAction& newAction, bool animate = false);
};

