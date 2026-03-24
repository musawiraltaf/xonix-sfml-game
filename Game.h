#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "GameData.h"

class Game
{
public:
    Game();
    ~Game();

    void run();
    void changeState(State* newState);

    sf::RenderWindow& getWindow();
    GameData& getData() { return data; }

private:
    void applyPendingStateChange();

    sf::RenderWindow window;
    GameData data;
    State* currentState;
    State* pendingState;
};
