#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>
#include "Leaderboard.h"

class LeaderboardState : public State
{
public:
    LeaderboardState(Game& game, GameData& data);

    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font  font;
    sf::Text  title;
    sf::Text  info;                 // hint text: "ESC = Back"
    static const int MAX_ROWS = 10; // we only ever show top 10
    sf::Text rows[MAX_ROWS];        // one line of text per player
    int       rowCount;             // how many actually used
};
