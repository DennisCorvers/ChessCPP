#pragma once
#include <SFML/Graphics.hpp>

#include "ChessBoard.h"
#include "Window.h"

using namespace Mechanics;

namespace Graphics {

	class SpriteManager {
	private:
		static const int m_spriteSize = 56;
		static const int m_spriteMapSize = 32;
		const sf::Vector2i m_boardOffset = sf::Vector2i(28, 27);

		sf::Texture* m_piecesTexture;
		sf::Texture* m_boardTexture;
		sf::Sprite m_spriteMap[m_spriteMapSize];
		sf::Sprite m_piecesSprite;
		sf::Sprite m_boardSprite;

		bool m_isDirty = false;
		bool m_isFlipped = true;

	public:
		SpriteManager();
		~SpriteManager();

		void loadPiecesSprites(const ChessBoard& board);
		void drawSprites(Window& window);
		void flipBoard(bool isWhiteOrientation);
	};
}
