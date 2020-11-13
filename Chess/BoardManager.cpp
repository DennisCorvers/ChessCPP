#include "pch.h"
#include "BoardManager.h"
#include "ChessPieceManager.h"
#include "ChessBoard.h"
#include "ChessAction.h"
#include "TextureManager.hpp"
#include "SoundManager.hpp"

BoardManager::BoardManager(TextureManager& textureManager, SoundManager& soundManager, int pixelSize, PieceColour startOrientation) :
	m_soundManager(soundManager),
	m_board(std::make_unique<ChessBoard>())
{
	m_pieceManager = std::make_unique<ChessPieceManager>(textureManager, pixelSize, startOrientation);
}

BoardManager::~BoardManager()
{}

const sf::Vector2f BoardManager::getBoardCenter() {
	return m_pieceManager->getCenter();
}

void BoardManager::resetGame(PieceColour orientation)
{
	m_board->resetBoard(BoardSettings::DEFAULTBOARD);
	m_pieceManager->updateBoard(*m_board);
	m_pieceManager->flipBoard(orientation, true);
}

void BoardManager::resetGame()
{
	m_board->resetBoard(BoardSettings::DEFAULTBOARD);
	m_pieceManager->refreshBoard(*m_board);
}

void BoardManager::flipBoard(PieceColour orientation) {
	m_pieceManager->flipBoard(orientation);
}

void BoardManager::flipBoard() {
	m_pieceManager->flipBoard();
}

bool BoardManager::inputMove(const ChessMove move, bool validateMove, bool animate)
{
	if (validateMove) {
		if (!validateNewMove(move))
			return false;
	}

	ChessBoard nextState;
	ActionType moveResult = m_board->simulateMove(nextState, move, true);
	if (moveResult != ActionType::None)
	{
		m_board = std::make_unique<ChessBoard>(nextState);
		m_pieceManager->inputMove(*m_board, animate);
		handleSound(moveResult, true);
	}
	else {
		m_pieceManager->refreshBoard();
	}

	return true;
}



bool BoardManager::validateNewMove(const ChessMove move) const
{
	auto validPositions = m_board->getValidPositions(move.getPositionFrom());
	for (auto it = validPositions.begin(); it != validPositions.end(); ++it)
		if (move.getPositionTo() == *it) {
			return true;
		}

	return false;
}

void BoardManager::handleSound(const ActionType chessAction, bool playSound)
{
	if (!playSound)
		return;

	//Play from most important to least
	if (chessAction & ActionType::Check || chessAction & ActionType::Checkmate)
		m_soundManager.playSound(AssetNames::s_piece_check);

	else if (chessAction & ActionType::Take)
		m_soundManager.playSound(AssetNames::s_piece_take);

	else if (chessAction & ActionType::Castling)
		m_soundManager.playSound(AssetNames::s_piece_castle);

	else if (chessAction & ActionType::Normal)
		m_soundManager.playSound(AssetNames::s_piece_move);
}

void BoardManager::startSelection(const sf::Vector2f screenPosition, bool forceColour) const
{
	int forceClr = forceColour ? static_cast<int>(m_board->getPlayingColour()) : 0;
	m_pieceManager->startSelection(screenPosition, forceClr);
}

void BoardManager::startSelection(const sf::Vector2f screenPosition, PieceColour validColour) const
{
	m_pieceManager->startSelection(screenPosition, static_cast<int>(validColour));
}

void BoardManager::updateMousePosition(const sf::Vector2f screenPosition) const
{
	m_pieceManager->updateSelection(screenPosition);
}

bool BoardManager::endSelection(const sf::Vector2f screenPosition, ChessMove& outMove) {
	return m_pieceManager->endSelection(screenPosition, outMove);
}

void BoardManager::update(const float & deltaTime)
{
	m_pieceManager->update(deltaTime);
}

void BoardManager::render(sf::RenderTarget& target)
{
	m_pieceManager->render(target);
}

void BoardManager::serializeBoard(sf::Packet & packet, bool isWriting) {
	m_board->netSerialize(packet, isWriting);
}

PieceColour BoardManager::getPlayingColour() const {
	return m_board->getPlayingColour();
}

ActionType BoardManager::getLastAction() const {
	return m_board->getLastAction().actionType;
}

std::string BoardManager::getFENFormat() const {
	return m_board->getFENFormat();
}

