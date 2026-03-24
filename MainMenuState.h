#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>

struct MenuButton
{
    sf::RectangleShape box;
    sf::Text label;
};

class MainMenuState : public State
{
public:
    MainMenuState(Game& game, GameData& data);

    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text titleText;
    sf::Text subtitleText;
    sf::Text footerText;
    float animTime;

    static const int BUTTON_COUNT = 8;
    MenuButton buttons[BUTTON_COUNT];

    int hoveredIndex;

    void setupButton(int index, const char* text, float x, float y);
    void handleClick(int index);
};
