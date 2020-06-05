#pragma once
#include "State.h"
#include "ChessBoard.h"

struct MoveAction {
	ChessPosition moveFrom;
	ChessPosition moveTo;
	ChessPiece* movingPiece;

	ChessMove getChessMove() const {
		return ChessMove(moveFrom, moveTo);
	}
};

struct MyEvent;

class GameState : public State
{
private:
	sf::View view;

	ChessBoard* m_board;
	MoveAction m_moveAction;

	sf::Font m_font;

	void initTextures();
	void initFonts();
	void initGame();
	void initView();

	void startDragging(const MyEvent& nextEvent);
	void stopDragging(const MyEvent& nextEvent);

public:
	GameState(StateData* data);
	~GameState();

	void updateView(const float& deltaTime);

	void updateInput(const float& deltaTime) override;
	void update(const float& deltaTime) override;
	void render(sf::RenderTarget* target) override;
};

