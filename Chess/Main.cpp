#include "pch.h"
#include "Game.h"

int main()
{
	Game game;

	while (game.isRunning()) {
		game.update();
		game.render();
		game.lateUpdate();
	}

	return 0;
}


