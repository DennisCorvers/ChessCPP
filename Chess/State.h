#pragma once

#include <map>
#include "SFML/Graphics.hpp"

class StateData {
public:
	StateData() {};

	sf::RenderWindow* window;
};

class State
{
private:

protected:
	StateData* stateData;
	sf::RenderWindow* window;

	std::map<std::string, sf::Texture> textures;

	virtual void initKeybinds() = 0;

public:
	State(StateData* data);
	virtual ~State();

	const bool& getQuit() const;
	const bool getKeytime();

	void endState();

	virtual void updateInput(const float& deltaTime) = 0;
	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget* target) = 0;
};

