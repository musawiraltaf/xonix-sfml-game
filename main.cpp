
#include <cstdlib>
#include <ctime>
#include "Game.h"

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr))); // seed RNG once

    Game game;
    game.run();
    return 0;
}
