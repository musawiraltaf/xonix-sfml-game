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
    sf::Font font; // kept for compatibility; static UI fonts are used instead.

    sf::Text titleText;
    sf::Text infoText;
    sf::Text queueText;
    sf::Text matchText;
    sf::Text bracketText;
    sf::Text legendText;

    sf::RectangleShape backButton;
    sf::Text           backLabel;

    sf::RectangleShape joinButton;
    sf::Text           joinLabel;

    sf::RectangleShape addButton;
    sf::Text           addLabel;

    sf::RectangleShape startButton;
    sf::Text           startLabel;

    sf::RectangleShape modeButtons[3];
    sf::Text           modeLabels[3];

    int  playerTarget;
    int  hoveredIndex;

    bool isMouseOver(const sf::RectangleShape& rect, const sf::Vector2f& point) const;
    void handleClickAt(const sf::Vector2f& point);
    void startTournamentFromQueue();
    void rebuildQueueAndMatches();
};
