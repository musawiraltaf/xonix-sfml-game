#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>

class ProfileState : public State
{
public:
    ProfileState(Game& game, GameData& data);

    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;

    sf::Text title;
    sf::Text body;
    sf::Text helper;

    void rebuildText();
};
