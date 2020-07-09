#pragma once

#include "ResourceManager.h"
#include <SFML/Graphics/Font.hpp>

class FontManager : public ResourceManager<FontManager, sf::Font>
{

private:
	virtual std::shared_ptr<sf::Font> loadResource(const std::string & path) const override {
		auto font = std::make_shared<sf::Font>();

		if (!font->loadFromFile(path))
		{
			font = nullptr;
			throw std::runtime_error("FontManager::loadResource - Failed to load font: " + path);
		}
		return font;
	}
};