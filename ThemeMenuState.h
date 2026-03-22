#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>

// Simple menu to choose a theme (inventory) using number keys.
class ThemeMenuState : public State
{
public:
    ThemeMenuState(Game& game, GameData& data);

    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text titleText;
    sf::Text themeListText;
    sf::Text helperText;
};
