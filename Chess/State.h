#pragma once

#include <map>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "AssetFlags.h"

class EventManager;

class StateData {
public:
	StateData() {};

	sf::RenderWindow* window;
	EventManager* eventManager;
};

class State
{
private:

protected:
	const StateData* stateData;

	std::map<AssetFlags, sf::Texture> textures;
	std::map<AssetFlags, sf::SoundBuffer> sounds;

public:
	State(StateData* data);
	virtual ~State();

	const bool getQuit() const;

	void endState();

	virtual void updateInput(const float& deltaTime) = 0;
	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget* target) = 0;

};

