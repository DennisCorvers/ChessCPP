#include "pch.h"
#include "Game.h"


int main()
{
	std::unique_ptr<Game> game = std::make_unique<Game>();

	while (game->getWindow().isOpen()) {
		game->update();
		game->render();
		game->lateUpdate();
	}

	return 0;
}


