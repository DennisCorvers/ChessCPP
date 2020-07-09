#pragma once
#include "SFML/Graphics.hpp"

class TextureManager;
class FontManager;

struct SharedContext {
	SharedContext() :
		window(nullptr)
	{}

	virtual ~SharedContext() {}

	sf::RenderWindow* window;
	TextureManager* textureManager;
	FontManager* fontManager;
};