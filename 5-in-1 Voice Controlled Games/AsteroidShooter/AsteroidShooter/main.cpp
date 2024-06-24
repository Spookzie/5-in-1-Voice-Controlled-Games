#include "Game.h"

#include <time.h>


int main()
{
    srand(static_cast<unsigned>(time(0)));

    Game game;

    while (game.IsRunning())
    {
        game.Update();
        game.Render();
    }

    return 0;
}