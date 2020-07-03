#pragma once
#include "SFML/Graphics.hpp"

struct SharedContext {
	SharedContext() :
		window(nullptr)
	{}

	sf::RenderWindow* window;
};