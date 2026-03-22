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
    void changeState(State* newState);        // manual pointer-based state switching

    sf::RenderWindow& getWindow();
    GameData& getData() { return data; }

private:
    sf::RenderWindow window;
    GameData         data;

    State* currentState;                      // raw pointer since we aren’t using <memory>
};
