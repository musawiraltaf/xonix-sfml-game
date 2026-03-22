#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>

struct SPButton
{
    sf::RectangleShape box;
    sf::Text           label;
};

class SinglePlayerMenuState : public State
{
public:
    SinglePlayerMenuState(Game& game, GameData& data);

    virtual void handleEvent(sf::Event& event) override;
    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;

private:
    sf::Font font;

    static const int BUTTON_COUNT = 3;
    SPButton buttons[BUTTON_COUNT];

    int hoveredIndex;

    void setupButton(int index, const char* text, float x, float y);
    void handleClick(int index);
};
