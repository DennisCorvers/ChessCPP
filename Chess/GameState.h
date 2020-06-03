#pragma once
#include "State.h"

class ChessBoard;

class GameState : public State
{
private:
	sf::View view;

	ChessBoard* m_board;
	sf::Font m_font;

	void initTextures();
	void initFonts();
	void initGame();
	void initView();

public:
	GameState(StateData* data);
	~GameState();

	virtual void initKeybinds() override;

	void updateView(const float& deltaTime);

	void updateInput(const float& deltaTime) override;
	void update(const float& deltaTime) override;
	void render(sf::RenderTarget* target) override;
};

