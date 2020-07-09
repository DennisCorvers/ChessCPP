#pragma once
#include "ResourceManager.h"
#include <SFML/Graphics/Texture.hpp>

class TextureManager : public ResourceManager<TextureManager, sf::Texture>
{
public:
	TextureManager(bool autoSmooth = false)
		:ResourceManager()
	{}

	virtual ~TextureManager() {}

private:
	virtual std::shared_ptr<sf::Texture> loadResource(const std::string & path) const override {
		auto texture = std::make_shared<sf::Texture>();
		if (!texture->loadFromFile(path))
		{
			texture = nullptr;
			throw std::runtime_error("TextureManager::loadResource - Failed to load texture: " + path);
		}
		return texture;
	}
};