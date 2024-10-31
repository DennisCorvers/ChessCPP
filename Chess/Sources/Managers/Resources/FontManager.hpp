#pragma once
#include "ResourceManager.hpp"
#include <SFML/Graphics/Font.hpp>

class FontManager : public ResourceManager<FontManager, sf::Font>
{
public:
	virtual std::shared_ptr<sf::Font> loadResource(const std::string & path) const override {
		auto font = std::make_shared<sf::Font>();

		if (!font->loadFromFile(path))
		{
			font = nullptr;
			throw std::runtime_error("FontManager::loadResource - Failed to load font: " + path);
		}
		return font;
	}

	static void setSmoothing(sf::Font& font, unsigned int fontSize, bool isSmooth) {
		sf::Texture& texture = const_cast<sf::Texture&>(font.getTexture(fontSize));
		texture.setSmooth(isSmooth);
	}
};