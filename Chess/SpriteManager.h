#pragma once
#include <SFML/Graphics.hpp>
#include "Window.h"

#include "Enums.h"

class ChessPiece;

namespace Graphics {

	class SpriteManager {
	private:
		static const int m_spriteSize = 56;
		static const int m_spriteMapSize = 32;
		const sf::Vector2i m_boardOffset = sf::Vector2i(28, 27);

	public:

		void loadPiecesSprites(ChessPiece* board);

	};
}
