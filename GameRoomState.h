#pragma once

#include "State.h"
#include "GameData.h"
#include "MatchmakingQueue.h"

#include <SFML/Graphics.hpp>
#include <string>

class GameRoomState : public State
{
public:
    GameRoomState(Game& game, GameData& data);

    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    // UI
    sf::Font font;

    sf::Text titleText;
    sf::Text infoText;
    sf::Text queueText;
    sf::Text matchText;

    sf::RectangleShape backButton;
    sf::Text           backLabel;

    sf::RectangleShape joinButton;
    sf::Text           joinLabel;

    sf::RectangleShape startButton;
    sf::Text           startLabel;

    sf::RectangleShape modeButtons[3];   // 4 / 6 / 8
    sf::Text           modeLabels[3];

    int  playerTarget;      // 4, 6, or 8
    bool currentInQueue;    // has currentPlayer been added already?

    // Helpers
    bool isMouseOver(const sf::RectangleShape& rect,
        const sf::Vector2f& point) const;

    void handleClickAt(const sf::Vector2f& point);

    void rebuildQueueAndMatches();
};
