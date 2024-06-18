#include "stdafx.h"
#include "Game.h"


int main()
{
	srand(static_cast<unsigned>(time(0)));

	Game game;

	while (game.GetWindow().isOpen())
	{
		game.Update();
		game.Render();
	}

	return 0;
}