#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>

class ThemeMenuState : public State
{
public:
    ThemeMenuState(Game& game, GameData& data);

    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    static const int MAX_THEME_CARDS = 10;

    sf::Font font;
    sf::Text titleText;
    sf::Text helperText;
    sf::Text statusText;
    float animTime;

    sf::RectangleShape cards[MAX_THEME_CARDS];
    sf::Text cardTitles[MAX_THEME_CARDS];
    sf::Text cardDescriptions[MAX_THEME_CARDS];
    int themeIDs[MAX_THEME_CARDS];
    int themeCount;
    int hoveredCard;

    void rebuildCards();
    void applyThemeByIndex(int index);
};
