// MainMenuState.h
#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>

struct MenuButton
{
    sf::RectangleShape box;
    sf::Text           label;
};

class MainMenuState : public State
{
public:
    MainMenuState(Game& game, GameData& data);

    virtual void handleEvent(sf::Event& event) override;
    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;

private:
    sf::Font font;

    static const int BUTTON_COUNT = 8;
    MenuButton buttons[BUTTON_COUNT];

    int hoveredIndex;

    void setupButton(int index, const char* text, float x, float y);
    void handleClick(int index);
};
