#pragma once
#include "ResourceManagers.hpp"
#include "SoundManager.hpp"

class AudioManager;
class EventManager;

struct SharedContext {

	sf::RenderWindow* window;
	TextureManager* textureManager;
	FontManager* fontManager;
	SoundManager* soundManager;
	ThemeManager* themeManager;

	void changeState(States state) const {
		soundManager->switchState(state);
	}

	void removeState(States state) const {
		soundManager->removeState(state);

		textureManager->releaseResource(state);
		fontManager->releaseResource(state);
		themeManager->releaseResource(state);
	}
};