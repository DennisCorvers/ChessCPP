#include "pch.h"
#include "BoardManager.h"
#include "ChessPieceManager.h"
#include "ChessBoard.h"
#include "ChessAction.h"
#include "TextureManager.h"

BoardManager::BoardManager(
	TextureManager& textureManager,
	std::map<AssetFlags, sf::SoundBuffer>& sounds,
	PieceColour startOrientation)
{
	m_board = std::make_unique<ChessBoard>();

	sf::Texture& boardTexture = *textureManager.getResource(AssetFlags::t_board);
	sf::Texture& pieceTexture = *textureManager.getResource(AssetFlags::t_pieces);
	m_pieceManager = std::make_unique<ChessPieceManager>(boardTexture, pieceTexture, startOrientation);

	m_soundMap[ActionType::Normal] = sf::Sound(sounds[AssetFlags::s_piece_move]);
	m_soundMap[ActionType::Check] = sf::Sound(sounds[AssetFlags::s_piece_check]);
	m_soundMap[ActionType::Take] = sf::Sound(sounds[AssetFlags::s_piece_take]);
	m_soundMap[ActionType::Castling] = sf::Sound(sounds[AssetFlags::s_piece_castle]);
}

BoardManager::~BoardManager()
{

}

sf::Vector2f BoardManager::getBoardCenter()
{
	return m_pieceManager->getCenter();
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

bool BoardManager::inputMove(const ChessMove move, bool animate)
{
	if (!m_hasCachedMove) {
		std::cout << "No cached move found!" << std::endl;

		if (!validateMove(move))
			return false;
	}
	else
		m_hasCachedMove = false;

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

	if (moveResult & ActionType::Checkmate)
		std::cout << "Checkmate\n";

	if (moveResult & ActionType::Check)
		std::cout << "Check\n";

	if (moveResult & ActionType::Stalemate)
		std::cout << "Stalemate\n";

	if (moveResult & ActionType::Draw)
		std::cout << "Draw\n";

	return true;
}

bool BoardManager::validateMove(const ChessMove move) const
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
		m_soundMap[ActionType::Check].play();

	else if (chessAction & ActionType::Take)
		m_soundMap[ActionType::Take].play();

	else if (chessAction & ActionType::Castling)
		m_soundMap[ActionType::Castling].play();

	else if (chessAction & ActionType::Normal)
		m_soundMap[ActionType::Normal].play();
}

void BoardManager::startSelection(const sf::Vector2f screenPosition, PieceColour playerColour) const
{
	m_pieceManager->startSelection(screenPosition);
}

void BoardManager::updateMousePosition(const sf::Vector2f screenPosition) const
{
	m_pieceManager->updateSelection(screenPosition);
}

bool BoardManager::endSelection(const sf::Vector2f screenPosition, ChessMove& outMove)
{
	bool result = m_pieceManager->endSelection(screenPosition, outMove);
	if (result) {
		m_hasCachedMove = true;
		m_cachedMove = outMove;
	}

	return result;
}

void BoardManager::update(const float & deltaTime)
{
	m_pieceManager->update(deltaTime);
}

void BoardManager::render(sf::RenderTarget& target)
{
	m_pieceManager->render(target);
}
