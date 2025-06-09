#include "stdafx.h"
#include "GameEngine.h"

int main()
{
	srand(static_cast<unsigned>(time(0)));

	GameEngine game;
	while (game.getWindow().isOpen())
	{
		game.update();
		game.render();
	}
	return 0;
}