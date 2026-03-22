#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>

struct MPButton
{
    sf::RectangleShape box;
    sf::Text           label;
};

class MultiplayerMenuState : public State
{
public:
    MultiplayerMenuState(Game& game, GameData& data);

    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;

    static const int BUTTON_COUNT = 3;
    MPButton buttons[BUTTON_COUNT];

    int hoveredIndex;

    void setupButton(int index, const char* text, float x, float y);
    void handleClick(int index);
};
