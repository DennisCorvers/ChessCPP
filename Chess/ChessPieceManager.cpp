#include "pch.h"
#include "ChessPieceManager.h"
#include "ChessPieceEntity.h"
#include "ChessMove.h"
#include "ChessBoard.h"
#include "AnimatorSystem.h"
#include "TextureManager.hpp"

ChessPieceManager::ChessPieceManager(TextureManager& textureManager, int pixelSize, PieceColour orientation)
{
	sf::Texture& boardTexture = *textureManager.getResource(AssetNames::t_board);
	sf::Texture& pieceTexture = *textureManager.getResource(AssetNames::t_pieces);
	pieceTexture.setSmooth(true);

	//Scale board
	auto textureSize = boardTexture.getSize();
	auto textureScale = sf::Vector2f((float)pixelSize / textureSize.x, (float)pixelSize / textureSize.y);

	m_sprite.setTexture(boardTexture);
	setScale(textureScale);

	m_moveAction.reset();
	m_viewOrientation = orientation;

	auto func = std::bind(&ChessPieceManager::animationCallback, this);
	m_animatorSystem = std::make_unique<AnimatorSystem>(func);

	//Scale pieces
	m_squareSize = sf::Vector2f(getGlobalBounds().width / (float)8, getGlobalBounds().height / (float)8);
	auto pieceTSize = pieceTexture.getSize();
	auto pieceScale = m_squareSize / sf::Vector2f(pieceTSize.x / (float)6, pieceTSize.y / (float)2);
	pieceScale = pieceScale * .9f;

	for (char i = 0; i < PIECECOUNT; i++) {
		m_chessPieces[i] = new ChessPieceEntity(PieceColour::Black, PieceType::Pawn, pieceTexture);
		m_chessPieces[i]->setScale(pieceScale);

		m_chessPieces[i]->setActive(false);
	}

	initMarkers();
}

ChessPieceManager::~ChessPieceManager()
{
	m_animatorSystem.reset();

	for (char i = 0; i < 32; i++)
		delete m_chessPieces[i];
}


void ChessPieceManager::flipBoard(const PieceColour orientation, bool forceRefresh)
{
	if (forceRefresh) {
		m_viewOrientation = orientation;
		refreshBoard();
	}
	else if (m_viewOrientation != orientation) {
		m_viewOrientation = orientation;
		refreshBoard();
	}
}

void ChessPieceManager::flipBoard()
{
	m_viewOrientation = m_viewOrientation == PieceColour::Black ? PieceColour::White : PieceColour::Black;
	refreshBoard();
}

void ChessPieceManager::updateBoard(const ChessBoard& board)
{
	m_board = &board;
}

void ChessPieceManager::refreshBoard(const ChessBoard & board)
{
	updateBoard(board);
	refreshBoard();
}

void ChessPieceManager::refreshBoard()
{
	m_moveAction.reset();

	char pieceCount = 0;
	for (char x = 0; x < 8; x++)
	{
		for (char y = 0; y < 8; y++)
		{
			ChessPiece val = m_board->getPiece(x, y);
			if (val.isEmpty())
				continue;

			ChessPieceEntity* piece = m_chessPieces[pieceCount];
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

	if (m_board->getMoveNumber() == 0)
		m_moveMarkers.reset();
}


void ChessPieceManager::inputMove(const ChessBoard& board, bool animate)
{
	updateBoard(board);
	inputMove(board.getLastAction(), animate);
}

void ChessPieceManager::inputMove(const ChessAction& newAction, bool animate)
{
	if (animate) {
		sf::Vector2i boardPosition(newAction.moveTo.x(), newAction.moveTo.y());
		sf::Vector2f newPosition = boardToScreen(boardPosition);
		auto pieceFrom = getClickedPiece(newAction.moveFrom);
		ASSERT(pieceFrom != nullptr, "No piece at " << newAction.moveFrom);

		auto animation = new AnimatorComponent(*pieceFrom, newPosition);

		//Castling moves 2 pieces...
		if (newAction.actionType & ActionType::Castling) {
			bool castleLeft = newAction.moveTo.x() < 5;
			char yPos = newAction.moveFrom.y();
			char xPos = castleLeft ? 0 : 7;
			char xMod = castleLeft ? 1 : -1;

			auto rookPiece = getClickedPiece(ChessPosition(xPos, yPos));
			sf::Vector2i rookBoardPosition(newAction.moveTo.x() + xMod, yPos);
			sf::Vector2f newRookPosition = boardToScreen(rookBoardPosition);

			auto rookAnimation = new AnimatorComponent(*rookPiece, newRookPosition);
			m_animatorSystem->queueAnimation(rookAnimation);
		}
		m_animatorSystem->queueAnimation(animation);

	}
	else {
		animationCallback();
	}

	sf::Vector2i moveFrom(newAction.moveFrom.x(), newAction.moveFrom.y());
	sf::Vector2i moveTo(newAction.moveTo.x(), newAction.moveTo.y());
	m_moveMarkers.setMarkers(boardToScreen(moveFrom), boardToScreen(moveTo));
}

void ChessPieceManager::initMarkers()
{
	m_selectionMarker.setFillColor(sf::Color(200, 200, 121, 150));
	m_selectionMarker.setSize(m_squareSize);

	m_moveMarkers.setup(sf::Color(210, 205, 150, 150), m_squareSize);

	sf::CircleShape moveMarker;
	moveMarker.setFillColor(sf::Color(0, 200, 0, 100));
	moveMarker.setRadius(m_squareSize.x / 2);
	moveMarker.setScale(.45f, .45f);
	m_markerContainer = std::make_unique<SelectionMarkerContainer>(moveMarker);
}


void ChessPieceManager::animationCallback()
{
	refreshBoard();
}


void ChessPieceManager::update(const float & deltaTime)
{
	m_animatorSystem->update(deltaTime);
}

void ChessPieceManager::render(sf::RenderTarget& target)
{
	target.draw(m_sprite);

	m_moveMarkers.render(target);

	//Render overlays for possible moves etc...
	if (m_moveAction.hasSelection())
	{
		target.draw(m_selectionMarker);
		m_markerContainer->render(target);
	}

	//Rendering of all pieces
	for (char i = 0; i < PIECECOUNT; i++)
		m_chessPieces[i]->render(target);

	//Render again to ensure it's always on top.
	if (m_moveAction.isMoving())
		m_moveAction.movingPiece->render(target);

	m_animatorSystem->render(target);
}


void ChessPieceManager::startSelection(const sf::Vector2f screenPosition, int forceColour)
{
	if (!boundsContains(screenPosition.x, screenPosition.y))
		return;

	if (m_animatorSystem->isAnimating())
		return;

	ChessPieceEntity* piece = getClickedPiece(screenPosition);

	if (forceColour && piece != nullptr) {
		PieceColour clr = piece->getColour();
		if (clr != m_board->getPlayingColour())
			return;
		if (static_cast<int>(clr) != forceColour)
			return;
	}

	sf::Vector2i pos = screenToBoard(screenPosition);
	ChessPosition newChessPos(pos.x, pos.y);

	//Prevents double checking and double selections
	if (m_moveAction.moveFrom == newChessPos && m_moveAction.hasSelection()) {
		m_moveAction.setMoving(true);
		return;
	}

	if (piece) {
		if (piece->xPos != pos.x || piece->yPos != pos.y)
			return;

		if (m_moveAction.hasSelection() && piece->getColour() != m_moveAction.movingPiece->getColour())
			return;

		m_moveAction.newSelection(newChessPos, piece);
		m_moveAction.setMoving(true);
		m_moveAction.validPositions = m_board->getValidPositions(m_moveAction.moveFrom);

		selectChessPiece(m_moveAction);
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

	snapPieceToBoard(m_moveAction.moveFrom, *m_moveAction.movingPiece);
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


void ChessPieceManager::snapPieceToBoard(const ChessPosition newPosition, ChessPieceEntity& piece)
{
	sf::Vector2i boardPos(newPosition.x(), newPosition.y());
	sf::Vector2f newPos(boardToScreen(boardPos));

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

	int xPos = (int)((mousePos.x) / m_squareSize.x);
	int yPos = (int)((mousePos.y) / m_squareSize.y);

	sf::Vector2i clamped(Math::min(xPos, 7), Math::min(yPos, 7));
	return transformVector(clamped);
}

sf::Vector2f ChessPieceManager::boardToScreen(const sf::Vector2i boardPosition) const
{
	sf::Vector2i boardPos = transformVector(boardPosition);
	sf::Vector2f pos(
		m_squareSize.x / 2 + m_squareSize.x * boardPos.x,
		m_squareSize.y / 2 + m_squareSize.y * boardPos.y
	);
	return pos;
}

sf::Vector2f ChessPieceManager::clampToBoard(const sf::Vector2f mousePosition) const
{
	return sf::Vector2f(
		Math::clamp<float>(mousePosition.x, 0, m_squareSize.x * 8),
		Math::clamp<float>(mousePosition.y, 0, m_squareSize.y * 8)
	);
}


void ChessPieceManager::selectChessPiece(const MoveAction & moveData)
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

