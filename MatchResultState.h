#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

class MatchResultState : public State
{
public:
    // winnerCode: 0 = tie, 1 = P1, 2 = P2
    MatchResultState(Game& game, GameData& data, int winnerCode);

    void handleEvent(sf::Event& e) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    int winnerCode;
    sf::Font font;
    sf::Text titleText;
    sf::Text infoText;
    sf::Text helperText;
};
