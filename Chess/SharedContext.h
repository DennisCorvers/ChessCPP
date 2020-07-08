#pragma once
#include "SFML/Graphics.hpp"
#include "TextureManager.h"

struct SharedContext {
	SharedContext() :
		window(nullptr)
	{}

	sf::RenderWindow* window;
	TextureManager* textureManager;
};