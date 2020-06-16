#include "ChessPieceManager.h"
#include "ChessPieceEntity.h"
#include "ChessMove.h"
#include "ChessBoard.h"
#include "Enums.h"
#include "Utils.h"

ChessPieceManager::ChessPieceManager(const sf::FloatRect boardSizes, std::map<AssetFlags, sf::Texture>& textures)
{
	m_boardSizes = boardSizes;
	m_sprite.setTexture(textures[AssetFlags::t_board]);

	m_moveAction.movingPiece = NULL;
	m_moveAction.m_isMoving = false;

	m_boardCollider = sf::FloatRect(boardSizes.left, boardSizes.top, 8 * boardSizes.width, 8 * boardSizes.height);

	for (char i = 0; i < PIECECOUNT; i++) {
		m_chessPieces[i] = new ChessPieceEntity(PieceColour::Black, PieceType::Pawn, textures[AssetFlags::t_pieces]);
		m_chessPieces[i]->setScale(0.95f, 0.95f);

		m_chessPieces[i]->setActive(false);
	}

	initMarkers();
}

ChessPieceManager::~ChessPieceManager()
{
	for (char i = 0; i < 32; i++)
		delete m_chessPieces[i];
}

bool ChessPieceManager::isPieceMoving()
{
	return m_moveAction.m_isMoving;
}

void ChessPieceManager::syncPieces(const ChessBoard& chessBoard, bool animate)
{
	m_moveAction.reset();

	char pieceCount = 0;
	for (char x = 0; x < 8; x++)
	{
		for (char y = 0; y < 8; y++)
		{
			int i = x * 8 + y;
			ChessPiece val = chessBoard.getPiece(x, y);
			if (val.isEmpty())
				continue;

			m_chessPieces[pieceCount]->setActive(true);
			m_chessPieces[pieceCount]->transform(val.getColour(), val.getType());

			sf::Vector2f pos = boardToScreen(sf::Vector2i(x, y));
			m_chessPieces[pieceCount]->setCenter(pos.x, pos.y);

			pieceCount++;
		}
	}

	for (; pieceCount < PIECECOUNT; pieceCount++)
		m_chessPieces[pieceCount]->setActive(false);
}

void ChessPieceManager::initMarkers()
{
	m_selectionMarker.setFillColor(sf::Color(246, 248, 121, 255));
	m_selectionMarker.setSize(sf::Vector2f(m_boardSizes.width, m_boardSizes.height));

	m_warningMarker.setFillColor(sf::Color(200, 0, 0, 255));
	m_warningMarker.setSize(sf::Vector2f(m_boardSizes.width, m_boardSizes.height));

	sf::CircleShape moveMarker;
	moveMarker.setFillColor(sf::Color(0, 200, 0, 100));
	moveMarker.setRadius(m_boardSizes.width / 3);
	m_markerContainer = std::make_unique<MoveMarkerContainer>(moveMarker);
}

void ChessPieceManager::update(const float & deltaTime)
{
	//Animate moving pieces?
}

void ChessPieceManager::render(sf::RenderTarget* const target)
{
	target->draw(m_sprite);

	//Render overlays for possible moves etc...
	if (m_moveAction.hasSelection())
	{
		target->draw(m_selectionMarker);
		m_markerContainer->render(target);
	}

	//Rendering of all pieces
	for (char i = 0; i < PIECECOUNT; i++)
		m_chessPieces[i]->render(target);

	//Render again to ensure it's always on top.
	if (m_moveAction.m_isMoving && m_moveAction.hasSelection())
		m_moveAction.movingPiece->render(target);
}

void ChessPieceManager::startSelection(const sf::Vector2f screenPosition, const ChessBoard& board)
{
	if (!boundsContains(screenPosition.x, screenPosition.y))
		return;

	ChessPieceEntity* piece = getClickedPiece(screenPosition);

	sf::Vector2i pos = screenToBoard(screenPosition);
	ChessPosition newChessPos(pos.x, pos.y);

	if (piece) {
		if (m_moveAction.hasSelection()
			&& piece->getColour() != m_moveAction.movingPiece->getColour()) {

			if (!board.isValidPosition(newChessPos, m_moveAction.validPositions))
				m_moveAction.reset();

			return;
		}
		m_moveAction.moveFrom = newChessPos;
		m_moveAction.movingPiece = piece;
		m_moveAction.m_isMoving = true;
		m_moveAction.validPositions = board.getValidPositions(m_moveAction.moveFrom);

		selectChessPiece(m_moveAction, board);
	}
	else {
		if (m_moveAction.hasSelection()) {
			if (!board.isValidPosition(newChessPos, m_moveAction.validPositions))
				m_moveAction.reset();
			return;
		}
		m_moveAction.reset();
	}
}

void ChessPieceManager::updateSelection(const sf::Vector2f screenPosition)
{
	if (m_moveAction.m_isMoving)
	{
		auto clampedMouse = clampToBoard(screenPosition);
		m_moveAction.movingPiece->setCenter(clampedMouse.x, clampedMouse.y);
	}
}

bool ChessPieceManager::endSelection(const sf::Vector2f screenPosition, ChessMove& outMove)
{
	if (!m_moveAction.hasSelection())
		return false;

	sf::Vector2i pos = screenToBoard(screenPosition);
	m_moveAction.moveTo = ChessPosition(pos.x, pos.y);

	snapEntityToBoard(m_moveAction.moveFrom, m_moveAction.movingPiece);
	m_moveAction.m_isMoving = false;


	if (m_moveAction.moveFrom == m_moveAction.moveTo) {
		return false;
	}

	outMove = ChessMove(m_moveAction.moveFrom, m_moveAction.moveTo);
	m_moveAction.reset();
	return true;
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

ChessPieceEntity* ChessPieceManager::getClickedPiece(const sf::Vector2f clickPosition) const
{
	for (char i = 0; i < PIECECOUNT; i++)
	{
		ChessPieceEntity* piece = m_chessPieces[i];
		if (!piece->isActive()) continue;
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

void ChessPieceManager::selectChessPiece(const MoveAction & moveData, const ChessBoard & board)
{
	snapMarkerToBoard(m_moveAction.moveFrom, m_selectionMarker);
	for (auto it = m_moveAction.validPositions.begin(); it != m_moveAction.validPositions.end(); ++it)
	{
		ChessPosition chessPos = *it;
		sf::Vector2f position(boardToScreen(sf::Vector2i(chessPos.getX(), chessPos.getY())));
		m_markerContainer->moveMarker(position);
	}
	m_markerContainer->finalize();
}

