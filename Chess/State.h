#pragma once

#include <map>
#include "SFML/Graphics.hpp"

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

	std::map<std::string, sf::Texture> textures;

public:
	State(StateData* data);
	virtual ~State();

	const bool& getQuit() const;

	void endState();

	virtual void updateInput(const float& deltaTime) = 0;
	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget* target) = 0;

};

