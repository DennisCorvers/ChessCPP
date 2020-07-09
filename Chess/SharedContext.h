#pragma once
#include "SFML/Graphics.hpp"
class TextureManager;

struct SharedContext {
	SharedContext() :
		window(nullptr)
	{}

	virtual ~SharedContext() {}

	sf::RenderWindow* window;
	TextureManager* textureManager;
};