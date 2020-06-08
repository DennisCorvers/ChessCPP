#pragma once
#include "ChessPosition.h"
#include "SFML/Graphics.hpp"


class ChessPiece;
class Entity;
struct ChessMove;

struct MoveAction {
	ChessPosition moveFrom;
	ChessPosition moveTo;
	ChessPiece* movingPiece;
	bool m_isMoving;

	void reset() {
		m_isMoving = false;movingPiece = nullptr;
	}

	bool hasSelection() { return movingPiece; }
};

enum struct MouseEvent {
	MouseDown = 1, MouseUp = 2, MouseMove = 0
};

/**
Handles all player interaction with the pieces on the board.
*/
class ChessPieceManager
{
private:
	const static int PIECECOUNT = 32;
	MoveAction m_moveAction;
	ChessPiece* m_chessPieces[PIECECOUNT];
	sf::FloatRect m_boardSizes;
	sf::Vector2f m_boardBoundaries;

	sf::RectangleShape m_selectionMarker;
	sf::RectangleShape m_warningMarker;
	std::vector<sf::CircleShape> m_moveMarkers;

	void snapEntityToBoard(const ChessPosition newPosition, Entity* const piece);
	void snapMarkerToBoard(const ChessPosition newPosition, sf::Shape& marker);
	ChessPiece* getClickedPiece(const sf::Vector2f clickPosition) const;

	sf::Vector2i screenToBoard(const sf::Vector2f mousePosition) const;
	sf::Vector2f boardToScreen(const sf::Vector2i boardPosition) const;
	sf::Vector2f clampToBoard(const sf::Vector2f mousePosition) const;

	void startSelect(const sf::Vector2f screenposition);
	inline void movePiece(const sf::Vector2f screenposition);
	void endSelect(const sf::Vector2f screenposition);

public:
	ChessPieceManager(const sf::FloatRect boardSizes, const sf::Texture& texture);
	~ChessPieceManager();

	bool hasSelection();

	void initialize(char* const chessboard);
	void initMarkers();

	void update(const float& deltaTime);
	void render(sf::RenderTarget* const target);

	void updateMousePosition(const sf::Vector2f screenposition, const MouseEvent mouseEvent);

	void reset(char* const chessBoard);
	void inputMove(const ChessMove & newMove, bool animate);
	void inputMove(const ChessPosition& posFrom, const ChessPosition& posTo, bool animate);
};

