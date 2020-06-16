#pragma once
#include "State.h"


class BoardManager;

class GameState : public State
{
private:
	sf::View view;

	BoardManager* m_boardManager;

	sf::Font m_font;

	void initTextures();
	void initFonts();
	void initSounds();
	void initGame();
	void initView();

public:
	GameState(StateData* data);
	~GameState();

	void updateView(const float& deltaTime);

	void updateInput(const float& deltaTime) override;
	void update(const float& deltaTime) override;
	void render(sf::RenderTarget* target) override;
};

