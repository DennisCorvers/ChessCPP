#include "ChessBoard.h"


ChessBoard::ChessBoard(sf::FloatRect boardsize, std::map<std::string, sf::Texture>& textures)
	: textures(textures), m_boardSizes(boardsize)
{
	m_sprite.setTexture(textures["BOARD"]);
	m_pieceManager = new ChessPieceManager(boardsize, textures["PIECES"]);


}

ChessBoard::~ChessBoard()
{
	delete m_pieceManager;
}

bool ChessBoard::hasPieceSelected() const
{
	return m_pieceManager->hasSelection();
}

void ChessBoard::resetBoard()
{
	for (char x = 0; x < 8; x++)
	{
		for (char y = 0; y < 8; y++)
		{
			int index = x * 8 + y;
			m_board[index] = BoardSettings::DEFAULTBOARD[y][x];
		}
	}
	m_pieceManager->reset(m_board);
}

void ChessBoard::update(const float& deltaTime)
{
	m_pieceManager->update(deltaTime);
}

void ChessBoard::render(sf::RenderTarget* const target)
{
	target->draw(m_sprite);

	m_pieceManager->render(target);
}

void ChessBoard::updateMousePosition(const sf::Vector2f screenPosition, const MouseEvent mEvent)
{
	m_pieceManager->updateMousePosition(screenPosition, mEvent);

}

bool ChessBoard::tryMove(const ChessMove move)
{
	return false;
}
