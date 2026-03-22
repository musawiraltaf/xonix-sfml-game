#pragma once
#include <SFML/Graphics.hpp>

class Game;
struct GameData;

class State
{
protected:
    Game& game;
    GameData& data;

public:
    State(Game& game, GameData& data);   // declaration only
    virtual ~State();                    // we'll define it in .cpp

    virtual void handleEvent(sf::Event& event) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};
