#pragma once

#include <map>
#include "SFML/Graphics.hpp"



class State
{
private:

protected:
	std::map<std::string, sf::Texture> textures;


	virtual void initKeybinds() = 0;

public:
	State();
	virtual ~State();

	const bool& getQuit() const;
	const bool getKeytime();

	void endState();

	virtual void updateInput(const float& deltaTime) = 0;
	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget* target) = 0;
};

