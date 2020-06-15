#include <algorithm>
#include "ChessRules.h"
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

bool ChessBoard::isValidSelection(const ChessPosition position, const PieceColour playerColour) const
{
	ChessPiece piece = getPiece(position);

	return piece.getColour() == playerColour;
}

void ChessBoard::applyMove(const ChessMove newMove)
{
	//Add special moves...
	int indexFrom = newMove.getPositionFrom().getY() * 8 + newMove.getPositionFrom().getX();
	int indexTo = newMove.getPositionTo().getY() * 8 + newMove.getPositionTo().getX();

	ChessPiece piece = m_currentBoard[indexFrom];
	m_currentBoard[indexFrom] = 0;
	m_currentBoard[indexTo] = piece;
}

bool ChessBoard::inputMove(const ChessMove newMove, const std::vector<ChessPosition>* possiblePositions = nullptr)
{
	if (possiblePositions == nullptr)
		possiblePositions = &getValidPositions(newMove.getPositionFrom());

	for (auto it = possiblePositions->begin(); it != possiblePositions->end(); ++it)
	{
		if (newMove.getPositionTo() != *it)
			continue;
		else {
			applyMove(newMove);
			return true;
		}
	}
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

std::vector<ChessPosition> ChessBoard::getValidPositions(const ChessPosition& selectedPosition) const
{
	return ChessRules::getValidPositions(selectedPosition, *this);
}

