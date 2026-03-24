#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>

class MatchResultState : public State
{
public:
    MatchResultState(Game& game, GameData& data, int winnerCode, int finishedMode);

    void handleEvent(sf::Event& e) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    int winnerCode;
    int finishedMode;
    bool bracketHasNextMatch;
    sf::Font font;
    sf::Text titleText;
    sf::Text infoText;
    sf::Text helperText;
};
