#include "pch.h"
#include "ChessPieceManager.h"
#include "ChessPieceEntity.h"
#include "ChessMove.h"
#include "ChessBoard.h"

ChessPieceManager::ChessPieceManager(const sf::FloatRect boardSizes, std::map<AssetFlags, sf::Texture>& textures, PieceColour orientation)
{
	m_boardSizes = boardSizes;
	m_sprite.setTexture(textures[AssetFlags::t_board]);

	m_moveAction.reset();
	m_viewOrientation = orientation;

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

void ChessPieceManager::flipBoard(const ChessBoard& board, const PieceColour orientation)
{
	if (m_viewOrientation != orientation) {
		m_viewOrientation = orientation;
		refreshBoard(board);
	}
}

void ChessPieceManager::flipBoard(const ChessBoard & board)
{
	m_viewOrientation = m_viewOrientation == PieceColour::Black ? PieceColour::White : PieceColour::Black;
	refreshBoard(board);
}

void ChessPieceManager::refreshBoard(const ChessBoard& board)
{
	m_moveAction.reset();

	char pieceCount = 0;
	for (char x = 0; x < 8; x++)
	{
		for (char y = 0; y < 8; y++)
		{
			ChessPiece val = board.getPiece(x, y);
			if (val.isEmpty())
				continue;

			ChessPieceEntity* piece = m_chessPieces[pieceCount];
			piece->stopAnimator();
			piece->setActive(true);
			piece->transform(val.getColour(), val.getType());
			piece->xPos = x;
			piece->yPos = y;

			sf::Vector2f pos = boardToScreen(sf::Vector2i(x, y));
			piece->setCenter(pos.x, pos.y);

			pieceCount++;
		}
	}

	for (; pieceCount < PIECECOUNT; pieceCount++)
		m_chessPieces[pieceCount]->setActive(false);
}

void ChessPieceManager::inputMove(const ChessAction & newAction, bool animate)
{
	//TODO
	//- Start Animations for affected pieces
	//- Once animations are done (via vector<> ?) play relevant sounds
	//- Refresh the entire board.

	auto pieceFrom = getClickedPiece(newAction.moveFrom);
	auto pieceTo = getClickedPiece(newAction.moveTo);
	switch (newAction.actionType) {

	case ActionType::EnPassant:
	{

		break;
	}
	case ActionType::Castling:
	{
		break;
	}
	case ActionType::Promotion:
	{
		pieceFrom->transform(pieceFrom->getColour(), PieceType::Queen);
	}
	default:
	{
		if (pieceTo)
			pieceTo->setActive(false);

		pieceFrom->xPos = newAction.moveTo.x();
		pieceFrom->yPos = newAction.moveTo.y();
		snapPieceToBoard(newAction.moveTo, *pieceFrom, animate);

		break;
	}
	}
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
	for (char i = 0; i < PIECECOUNT; i++)
		m_chessPieces[i]->update(deltaTime);
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
	if (m_moveAction.isMoving())
		m_moveAction.movingPiece->render(target);
}

void ChessPieceManager::startSelection(const sf::Vector2f screenPosition, ChessBoard& board)
{
	if (!m_boardCollider.contains(screenPosition.x, screenPosition.y))
		return;

	ChessPieceEntity* piece = getClickedPiece(screenPosition);
	sf::Vector2i pos = screenToBoard(screenPosition);
	ChessPosition newChessPos(pos.x, pos.y);

	//Prevents double checking and double selections
	if (m_moveAction.moveFrom == newChessPos && m_moveAction.hasSelection()) {
		m_moveAction.setMoving(true);
		return;
	}

	if (piece) {
		if (m_moveAction.hasSelection() && piece->getColour() != m_moveAction.movingPiece->getColour())
			return;

		m_moveAction.newSelection(newChessPos, piece);
		m_moveAction.setMoving(true);
		m_moveAction.validPositions = board.getValidPositions(m_moveAction.moveFrom);

		selectChessPiece(m_moveAction, board);
	}
	else {
		if (!m_moveAction.hasSelection())
			m_moveAction.reset();
	}
}

void ChessPieceManager::updateSelection(const sf::Vector2f screenPosition)
{
	if (Math::distance(screenPosition, m_lastScreenPosition) > 0.15f) {
		if (m_moveAction.isMoving())
		{
			auto clampedMouse = clampToBoard(screenPosition);
			m_moveAction.movingPiece->setCenter(clampedMouse.x, clampedMouse.y);
		}
	}
	m_lastScreenPosition = screenPosition;
}

bool ChessPieceManager::endSelection(const sf::Vector2f screenPosition, ChessMove& outMove)
{
	if (!m_moveAction.hasSelection())
		return false;

	sf::Vector2i pos = screenToBoard(screenPosition);
	m_moveAction.moveTo = ChessPosition(pos.x, pos.y);
	m_moveAction.setMoving(false);

	snapPieceToBoard(m_moveAction.moveFrom, *m_moveAction.movingPiece, false);
	//Reset entity if the same position is selected.
	if (m_moveAction.moveFrom == m_moveAction.moveTo) {
		m_moveAction.selectCount++;
		if (m_moveAction.selectCount > 1)
			m_moveAction.reset();

		return false;
	}

	bool isValidMove = false;
	for (auto it = m_moveAction.validPositions.begin(); it != m_moveAction.validPositions.end(); ++it)
	{
		if (m_moveAction.moveTo == *it) {
			isValidMove = true;
			break;
		}
	}

	//Reset selection if an invalid position is selected.
	if (!isValidMove) {
		m_moveAction.moveTo = m_moveAction.moveFrom;
		m_moveAction.reset();
		return false;
	}

	outMove = ChessMove(m_moveAction.moveFrom, m_moveAction.moveTo);
	m_moveAction.reset();
	return true;
}

void ChessPieceManager::snapPieceToBoard(const ChessPosition newPosition, ChessPieceEntity& piece, bool animate)
{
	sf::Vector2i boardPos(newPosition.x(), newPosition.y());
	sf::Vector2f newPos(boardToScreen(boardPos));

	if (animate)
		piece.move(newPos);
	else
		piece.setCenter(newPos);
}

void ChessPieceManager::snapMarkerToBoard(const ChessPosition newPosition, sf::Shape& marker)
{
	sf::Vector2i boardPos(newPosition.x(), newPosition.y());
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

ChessPieceEntity* ChessPieceManager::getClickedPiece(const ChessPosition chessPosition) const
{
	for (char i = 0; i < PIECECOUNT; i++)
	{
		ChessPieceEntity* piece = m_chessPieces[i];
		if (!piece->isActive()) continue;
		if (piece->xPos == chessPosition.x() && piece->yPos == chessPosition.y())
			return piece;
	}
	return nullptr;
}

sf::Vector2i ChessPieceManager::screenToBoard(const sf::Vector2f mousePosition) const
{
	auto mousePos = clampToBoard(mousePosition);

	int xPos = (int)((mousePos.x - m_boardSizes.left) / m_boardSizes.width);
	int yPos = (int)((mousePos.y - m_boardSizes.top) / m_boardSizes.height);

	sf::Vector2i clamped(Math::limit(xPos, 7), Math::limit(yPos, 7));
	return transformVector(clamped);
}

sf::Vector2f ChessPieceManager::boardToScreen(const sf::Vector2i boardPosition) const
{
	sf::Vector2i boardPos = transformVector(boardPosition);
	sf::Vector2f pos(
		m_boardSizes.left + m_boardSizes.width / 2 + m_boardSizes.width * boardPos.x,
		m_boardSizes.top + m_boardSizes.height / 2 + m_boardSizes.height * boardPos.y
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
		sf::Vector2f position(boardToScreen(sf::Vector2i(chessPos.x(), chessPos.y())));
		m_markerContainer->moveMarker(position);
	}
	m_markerContainer->finalize();
}

