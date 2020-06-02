#include "SpriteManager.h"
#include "Utilities.h"

namespace Graphics {
	SpriteManager::SpriteManager()
	{
		m_piecesTexture = ResourceLoader::loadTexture("Assets\\Sprites\\Pieces.png");
		m_boardTexture = ResourceLoader::loadTexture("Assets\\Sprites\\Board.png");

		if (m_piecesTexture) { m_piecesSprite = sf::Sprite(*m_piecesTexture); }
		if (m_boardTexture) { m_boardSprite = sf::Sprite(*m_boardTexture); }

		for (int i = 0; i < m_spriteMapSize; i++)
			m_spriteMap[i].setTexture(*m_piecesTexture);
	}

	SpriteManager::~SpriteManager()
	{
		delete m_boardTexture;
		delete m_piecesTexture;
	}

	void SpriteManager::loadPiecesSprites(const ChessBoard& board)
	{
		int k = 0;
		for (char x = 0; x < 8;x++) {
			for (char y = 0; y < 8;y++) {
				ChessPiece piece = board.getBoard()[x * 8 + y];
				if (piece.getType() == PieceType::None) continue;
				char type = (char)piece.getType();

				int xPos = abs(type) - 1;
				int yPos = piece.getColour() == PieceColour::White ? 1 : 0; //Flip White/Black to flip the pieces on the board.
				m_spriteMap[k].setTextureRect(sf::IntRect(m_spriteSize * xPos, m_spriteSize * yPos, m_spriteSize, m_spriteSize));
				m_spriteMap[k].setPosition(
					m_spriteSize * y + m_boardOffset.x,
					m_spriteSize * x + m_boardOffset.y);
				k++;
			}
		}
	}

	void SpriteManager::drawSprites(Window& window) const
	{
		window.draw(m_boardSprite);
		for (int i = 0; i < m_spriteMapSize; i++)
		{
			window.draw(m_spriteMap[i]);
		}
	}

	void SpriteManager::flipBoard(bool isWhiteOrientation)
	{

	}

	void SpriteManager::dragPiece(int posX, int posY) {

		for (int i = 0; i < m_spriteMapSize; i++)
		{
			if (m_spriteMap[i].getGlobalBounds().contains(posX, posY))
			{

			}
		}
	}

	sf::Vector2i SpriteManager::toBoardCoords(int posX, int posY) {
		return sf::Vector2i(1, 1);
	}
}