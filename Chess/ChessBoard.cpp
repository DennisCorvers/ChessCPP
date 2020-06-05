#include "ChessBoard.h"
#include "ChessPiece.h"

ChessBoard::ChessBoard(BoardSizes boardsize, std::map<std::string, sf::Texture>& textures)
	: textures(textures), m_boardSizes(boardsize)
{
	m_sprite.setTexture(textures["BOARD"]);

	initPieces();
	initBoard();
}

ChessBoard::~ChessBoard()
{
	for (char i = 0; i < 32; i++)
		delete m_pieces[i];
}

void ChessBoard::initPieces()
{
	for (char i = 0; i < BoardSettings::PIECECOUNT; i++)
		m_pieces[i] = new ChessPiece(PieceColour::Black, PieceType::Pawn, textures["PIECES"]);
}

void ChessBoard::initBoard()
{
	int pieceCount = 0;
	for (char x = 0; x < 8; x++)
	{
		for (char y = 0; y < 8; y++)
		{
			char pieceValue = BoardSettings::DEFAULTBOARD[y][x];
			m_board[x * 8 + y] = pieceValue;
			if (pieceValue == 0)
				continue;

			ChessPiece* piece = m_pieces[pieceCount];
			PieceColour colour = pieceValue < 0 ? PieceColour::Black : PieceColour::White;
			PieceType type = PieceType(abs(pieceValue));

			piece->transform(colour, type);

			sf::Vector2f pos = boardToScreen(sf::Vector2i(x, y));
			piece->setCenter(pos.x, pos.y);

			pieceCount++;
		}
	}
}

void ChessBoard::resetBoard()
{
	initBoard();
}

void ChessBoard::update()
{

}

void ChessBoard::render(sf::RenderTarget* target)
{
	target->draw(m_sprite);

	for (char i = 0; i < BoardSettings::PIECECOUNT; i++)
		m_pieces[i]->render(target);
}

void ChessBoard::snapPieceToBoard(const ChessPosition newPosition, ChessPiece * piece)
{
	sf::Vector2i boardPos(newPosition.X(), newPosition.Y());
	sf::Vector2f newPos(boardToScreen(boardPos));

	piece->setCenter(newPos.x, newPos.y);
}

ChessPiece* ChessBoard::getClickedPiece(const sf::Vector2f clickPosition) const
{
	for (char i = 0; i < BoardSettings::PIECECOUNT; i++)
	{
		ChessPiece* piece = m_pieces[i];
		if (!piece->isEnabled) continue;
		if (piece->boundsContains(clickPosition.x, clickPosition.y))
			return piece;
	}
	return NULL;
}

bool ChessBoard::tryMove(const ChessMove move)
{
	return false;
}

bool ChessBoard::tryScreenToBoard(const sf::Vector2f mousePosition, sf::Vector2i* const boardPosition) const
{
	*boardPosition = sf::Vector2i(-1, -1);
	if (mousePosition.x < m_boardSizes.leftOffset || mousePosition.y < m_boardSizes.topOffset)
		return false;

	int xPos = (int)((mousePosition.x - m_boardSizes.leftOffset) / m_boardSizes.fieldXSize);
	int yPos = (int)((mousePosition.y - m_boardSizes.topOffset) / m_boardSizes.fieldYSize);

	if (xPos > 7 || yPos > 7)
		return false;

	*boardPosition = sf::Vector2i(xPos, yPos);
	return true;
}

sf::Vector2f ChessBoard::boardToScreen(const sf::Vector2i boardPosition) const
{
	sf::Vector2f pos(
		m_boardSizes.leftOffset + m_boardSizes.fieldXSize / 2 + m_boardSizes.fieldXSize * boardPosition.x,
		m_boardSizes.topOffset + m_boardSizes.fieldYSize / 2 + m_boardSizes.fieldYSize * boardPosition.y
	);
	return pos;
}
