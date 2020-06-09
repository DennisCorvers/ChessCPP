#include "ChessBoard.h"
#include "Enums.h"

ChessBoard::ChessBoard(const sf::Texture & boardTexture, const char(&boardData)[SIZE])
	: m_boardTexture(boardTexture)
{
	m_sprite.setTexture(boardTexture);
	m_defaultBoard = boardData;
}

ChessBoard::~ChessBoard()
{	}

const char * ChessBoard::getBoard() const
{
	return m_currentBoard;
}

bool ChessBoard::isValidSelection(const ChessPosition position, const PieceColour playerColour) const
{
	char piece = m_currentBoard[position.getY() * 8 + position.getX()];

	if (playerColour == PieceColour::Black) {
		return piece < 0;
	}
	else {
		return piece > 1;
	}

	return false;
}

bool ChessBoard::inputMove(const ChessMove newMove)
{
	return false;
}

void ChessBoard::resetBoard()
{
	for (char x = 0; x < 8; x++)
	{
		for (char y = 0; y < 8; y++)
		{
			int index = x * 8 + y;
			m_currentBoard[index] = m_defaultBoard[index];
		}
	}
}

void ChessBoard::update(const float& deltaTime)
{
}

void ChessBoard::render(sf::RenderTarget* const target)
{
	target->draw(m_sprite);
}

std::vector<ChessPosition> ChessBoard::getValidPositions(const ChessPosition & selectedPosition) const
{
	return std::vector<ChessPosition>();
}

