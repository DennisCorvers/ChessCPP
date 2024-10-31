#pragma once
#include "ResourceManager.hpp"
#include <SFML/Audio/SoundBuffer.hpp>

class AudioManager : public ResourceManager<AudioManager, sf::SoundBuffer>
{

private:
	virtual std::shared_ptr<sf::SoundBuffer> loadResource(const std::string & path) const override {
		auto sound = std::make_shared<sf::SoundBuffer>();

		if (!sound->loadFromFile(path))
		{
			sound = nullptr;
			throw std::runtime_error("SoundManager::loadResource - Failed to load sound: " + path);
		}
		return sound;
	}
};