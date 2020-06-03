#include "ChessBoard.h"
#include "ChessPiece.h"

const char ChessBoard::m_defaultBoard[8][8] = {
	{ -2, -3, -4, -5, -6, -4, -3, -2},
	{ -1, -1, -1, -1, -1, -1, -1, -1 },
	{  0,  0,  0,  0,  0,  0,  0,  0 },
	{  0,  0,  0,  0,  0,  0,  0,  0 },
	{  0,  0,  0,  0,  0,  0,  0,  0 },
	{  0,  0,  0,  0,  0,  0,  0,  0 },
	{  1,  1,  1,  1,  1,  1,  1,  1 },
	{  2,  3,  4,  5,  6,  4,  3,  2 },
};


ChessBoard::ChessBoard(sf::Vector2f boardOffset, std::map<std::string, sf::Texture>& textures)
	: textures(textures)
{
	m_sprite.setTexture(textures["BOARD"]);
	m_boardOffset = boardOffset;

	initBoard();
}

ChessBoard::~ChessBoard()
{
	for (char i = 0; i < 64; i++)
		delete m_board[i];
}

void ChessBoard::initBoard()
{
	for (char x = 0; x < 8; x++)
	{
		for (char y = 0; y < 8; y++)
		{
			int index = x * 8 + y;
			char pieceValue = m_defaultBoard[x][y];
			if (pieceValue == 0)
			{
				m_board[index] = NULL;
				continue;
			}
			PieceColour colour = pieceValue < 0 ? PieceColour::Black : PieceColour::White;
			createPiece(colour, PieceType(abs(pieceValue)), sf::Vector2f(x, y));
		}
	}
}

void ChessBoard::createPiece(PieceColour colour, PieceType pieceType, const sf::Vector2f position)
{
	int index = position.x * 8 + position.y;
	ChessPiece* piece = new ChessPiece(colour, pieceType, textures["PIECES"]);

	//Calculate size of squares and offets etc...
	piece->setPosition(
		56 * position.y + m_boardOffset.x,
		56 * position.x + m_boardOffset.y);

	m_board[index] = piece;
}


void ChessBoard::resetBoard()
{
	for (char i = 0; i < 64; i++)
		delete m_board[i];

	initBoard();
}

void ChessBoard::update()
{

	//Do drag stuff??
}

void ChessBoard::render(sf::RenderTarget* target)
{
	target->draw(m_sprite);

	//Render "children"
	for (char i = 0; i < 64; i++)
	{
		ChessPiece* piece = m_board[i];
		if (!piece) { continue; }

		piece->render(target);
	}
}

