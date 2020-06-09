#include "ChessPieceManager.h"
#include "ChessPiece.h"
#include "ChessMove.h"
#include "Enums.h"
#include "Utils.h"

ChessPieceManager::ChessPieceManager(const sf::FloatRect boardSizes, const sf::Texture& texture)
{
	m_boardSizes = boardSizes;

	m_moveAction.movingPiece = NULL;
	m_moveAction.m_isMoving = false;

	m_boardCollider = sf::FloatRect(boardSizes.left, boardSizes.top, 8 * boardSizes.width, 8 * boardSizes.height);

	for (char i = 0; i < PIECECOUNT; i++) {
		m_chessPieces[i] = new ChessPiece(PieceColour::Black, PieceType::Pawn, texture);
		m_chessPieces[i]->setScale(0.95, 0.95);

		m_chessPieces[i]->isEnabled = false;
	}

	initMarkers();
}

ChessPieceManager::~ChessPieceManager()
{
	for (char i = 0; i < 32; i++)
		delete m_chessPieces[i];
}

bool ChessPieceManager::hasSelection()
{
	return m_moveAction.m_isMoving;
}

sf::FloatRect ChessPieceManager::getBoardSizes()
{
	return m_boardSizes;
}

void ChessPieceManager::reset(const char* const chessBoard)
{
	m_moveAction.reset();

	char pieceCount = 0;
	for (char x = 0; x < 8; x++)
	{
		for (char y = 0; y < 8; y++)
		{
			int i = x * 8 + y;
			char val = chessBoard[i];
			if (val == 0) continue;

			PieceColour colour = val < 0 ? PieceColour::Black : PieceColour::White;
			PieceType type = PieceType(abs(val));
			m_chessPieces[pieceCount]->isEnabled = true;
			m_chessPieces[pieceCount]->transform(colour, type);

			sf::Vector2f pos = boardToScreen(sf::Vector2i(y, x));
			m_chessPieces[pieceCount]->setCenter(pos.x, pos.y);

			pieceCount++;
		}
	}
}

void ChessPieceManager::initMarkers()
{
	m_selectionMarker.setFillColor(sf::Color(246, 248, 121, 255));
	m_selectionMarker.setSize(sf::Vector2f(m_boardSizes.width, m_boardSizes.height));

	m_warningMarker.setFillColor(sf::Color(200, 0, 0, 255));
	m_warningMarker.setSize(sf::Vector2f(m_boardSizes.width, m_boardSizes.height));
}

void ChessPieceManager::update(const float & deltaTime)
{
	//Animate moving pieces?
}

void ChessPieceManager::inputMove(const ChessMove newMove, bool animate)
{
	inputMove(newMove.getPositionFrom(), newMove.getPositionTo(), animate);
}

void ChessPieceManager::inputMove(const ChessPosition posFrom, const ChessPosition posTo, bool animate)
{
	//Input remote moves/replay moves that should be animated/set directly
	ChessPiece* pieceFrom = getClickedPiece(boardToScreen(posFrom));
	ChessPiece* pieceTo = getClickedPiece(boardToScreen(posTo));

	if (pieceFrom)
		snapEntityToBoard(posTo, pieceFrom);

	if (pieceTo)
		pieceTo->isEnabled = false;
}

void ChessPieceManager::render(sf::RenderTarget* const target)
{
	//Render overlays for possible moves etc...
	if (m_moveAction.hasSelection()) {
		target->draw(m_selectionMarker);
	}

	//Rendering of all pieces
	for (char i = 0; i < PIECECOUNT; i++)
		m_chessPieces[i]->render(target);

	//Render again to ensure it's always on top.
	if (m_moveAction.m_isMoving && m_moveAction.hasSelection())
		m_moveAction.movingPiece->render(target);
}

void ChessPieceManager::startSelection(const sf::Vector2f screenPosition, const ChessBoard & board)
{
	//Change selection from Vector2f to ChessPosition
	sf::Vector2i pos = screenToBoard(screenPosition);

	ChessPiece* piece = getClickedPiece(screenPosition);
	if (!piece) return;

	if (m_moveAction.hasSelection() &&
		piece->getColour() != m_moveAction.movingPiece->getColour())
		return;

	m_moveAction.movingPiece = piece;
	m_moveAction.moveFrom = ChessPosition(pos.x, pos.y);
	m_moveAction.m_isMoving = true;

	snapMarkerToBoard(m_moveAction.moveFrom, m_selectionMarker);
	//auto possibleMoves = board.getPossibleMoves(m_moveAction.moveFrom);
	//foreach... gather and draw the valid moves on board...

}

void ChessPieceManager::updateSelection(const sf::Vector2f screenPosition)
{
	if (m_moveAction.m_isMoving)
	{
		auto clampedMouse = clampToBoard(screenPosition);
		m_moveAction.movingPiece->setCenter(clampedMouse.x, clampedMouse.y);
	}
}

void ChessPieceManager::endSelection(const sf::Vector2f screenPosition, const ChessBoard& board)
{
	if (!m_moveAction.hasSelection())
		return;

	sf::Vector2i pos = screenToBoard(screenPosition);
	m_moveAction.moveTo = ChessPosition(pos.x, pos.y);

	//Piece has been selected but not moved
	if (m_moveAction.moveFrom == m_moveAction.moveTo) {
		snapEntityToBoard(m_moveAction.moveFrom, m_moveAction.movingPiece);
		m_moveAction.m_isMoving = false;
	}
	//Piece has been selected and moved
	else {

		//Check if move is valid...

		snapEntityToBoard(m_moveAction.moveFrom, m_moveAction.movingPiece);
		inputMove(m_moveAction.moveFrom, m_moveAction.moveTo, false);
		m_moveAction.reset();

		//Disable rendering of markers...
		//Clear/Overwrite possible moves list...
	}
}

const bool ChessPieceManager::boundsContains(float x, float y) const
{
	return m_boardCollider.contains(x, y);
}



void ChessPieceManager::snapEntityToBoard(const ChessPosition newPosition, Entity* const piece)
{
	sf::Vector2i boardPos(newPosition.getX(), newPosition.getY());
	sf::Vector2f newPos(boardToScreen(boardPos));

	piece->setCenter(newPos.x, newPos.y);
}

void ChessPieceManager::snapMarkerToBoard(const ChessPosition newPosition, sf::Shape& marker)
{
	sf::Vector2i boardPos(newPosition.getX(), newPosition.getY());
	sf::Vector2f newPos(boardToScreen(boardPos));

	marker.setPosition(
		newPos.x - marker.getGlobalBounds().width / 2.f,
		newPos.y - marker.getGlobalBounds().height / 2.f
	);
}

ChessPiece* ChessPieceManager::getClickedPiece(const sf::Vector2f clickPosition) const
{
	for (char i = 0; i < PIECECOUNT; i++)
	{
		ChessPiece* piece = m_chessPieces[i];
		if (!piece->isEnabled) continue;
		if (piece->boundsContains(clickPosition.x, clickPosition.y))
			return piece;
	}
	return nullptr;
}

sf::Vector2i ChessPieceManager::screenToBoard(const sf::Vector2f mousePosition) const
{
	auto mousePos = clampToBoard(mousePosition);

	int xPos = (int)((mousePos.x - m_boardSizes.left) / m_boardSizes.width);
	int yPos = (int)((mousePos.y - m_boardSizes.top) / m_boardSizes.height);

	return sf::Vector2i(xPos, yPos);
}

sf::Vector2f ChessPieceManager::boardToScreen(const ChessPosition boardPosition) const {
	return boardToScreen(sf::Vector2i(boardPosition.getX(), boardPosition.getY()));
}
sf::Vector2f ChessPieceManager::boardToScreen(const sf::Vector2i boardPosition) const
{
	sf::Vector2f pos(
		m_boardSizes.left + m_boardSizes.width / 2 + m_boardSizes.width * boardPosition.x,
		m_boardSizes.top + m_boardSizes.height / 2 + m_boardSizes.height * boardPosition.y
	);
	return pos;
}

sf::Vector2f ChessPieceManager::clampToBoard(const sf::Vector2f mousePosition) const
{
	return sf::Vector2f(
		Math::clamp(mousePosition.x, m_boardSizes.left, m_boardCollider.width + m_boardCollider.left),
		Math::clamp(mousePosition.y, m_boardSizes.top, m_boardCollider.height + m_boardCollider.top)
	);
}


