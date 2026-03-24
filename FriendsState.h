#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>
#include <string>

class FriendsState : public State
{
public:
    FriendsState(Game& game, GameData& data);

    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text titleText;
    sf::Text friendsTitleText;
    sf::Text pendingTitleText;
    sf::Text friendsText;
    sf::Text pendingText;
    sf::Text inputText;
    sf::Text helperText;
    sf::Text statusText;

    std::string usernameInput;
    int selectedPending;

    void rebuildTexts();
};
