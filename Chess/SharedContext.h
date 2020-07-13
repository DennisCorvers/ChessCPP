#pragma once

class sf::RenderWindow;
class TextureManager;
class FontManager;
class AudioManager;
class SoundManager;
class EventManager;

struct SharedContext {
	SharedContext()
	{}

	virtual ~SharedContext() {}

	sf::RenderWindow* window;
	TextureManager* textureManager;
	FontManager* fontManager;
	AudioManager* audioManager;
};