#pragma once
#include "State.h"

class ChessBoard;

class GameState : public State
{
private:
	ChessBoard* m_board;
	sf::Font m_font;

	void initTextures();
	void initFonts();
	void initGame();

public:
	GameState();
	~GameState();

	virtual void initKeybinds() override;

	virtual void updateInput(const float& deltaTime) override;
	virtual void update(const float& deltaTime) override;
	virtual void render(sf::RenderTarget* target) override;
};

