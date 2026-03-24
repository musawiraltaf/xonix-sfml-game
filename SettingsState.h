#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>

class SettingsState : public State
{
public:
    SettingsState(Game& game, GameData& data);

    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text titleText;
    sf::Text sectionText;
    sf::Text optionsText;
    sf::Text detailsText;
    sf::Text helperText;

    void rebuildText();
};
