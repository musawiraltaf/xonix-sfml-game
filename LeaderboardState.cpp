#include "LeaderboardState.h"
#include "Game.h"
#include "GameData.h"
#include "MainMenuState.h"
#include "GameLogic.h"     //  for N, M, ts
#include <sstream>         // for std::ostringstream

LeaderboardState::LeaderboardState(Game& game, GameData& data)
    : State(game, data), rowCount(0)
{
    // Use your existing font
    if (!font.loadFromFile("ariblk.ttf"))
    {
        // if it fails, we just won’t see text
    }

    // ----- Title -----
    title.setFont(font);
    title.setCharacterSize(32);
    title.setFillColor(sf::Color::White);
    title.setString("Leaderboard (Top 10)");

    // center horizontally
    float winWidth = static_cast<float>(N * ts);
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition(
        (winWidth - tb.width) / 2.f - tb.left,
        30.f
    );

    // ----- Hint text -----
    info.setFont(font);
    info.setCharacterSize(18);
    info.setFillColor(sf::Color(200, 200, 200));
    info.setString("Press ESC to return to Main Menu");

    sf::FloatRect ib = info.getLocalBounds();
    info.setPosition(
        (winWidth - ib.width) / 2.f - ib.left,
        70.f
    );

    // ----- Build rows from leaderboard -----
    LBEntry entries[MAX_ROWS];
    int count = data.leaderboard.getEntriesDescending(entries, MAX_ROWS);
    rowCount = count;

    float startY = 110.f;  // first row
    float lineHeight = 30.f;   // vertical spacing

    for (int i = 0; i < count; ++i)
    {
        sf::Text& t = rows[i];
        t.setFont(font);
        t.setCharacterSize(20);
        t.setFillColor(sf::Color::White);

        // Format:  "1) username   Total: X   High: Y"
        std::ostringstream oss;
        oss << (i + 1) << ")  "
            << entries[i].username
            << "   Total: " << entries[i].totalPoints
            << "   High: " << entries[i].highScore;

        t.setString(oss.str());

        // small left padding
        t.setPosition(60.f, startY + i * lineHeight);
    }
}

void LeaderboardState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape)
    {
        // ESC -> back to main menu
        game.changeState(new MainMenuState(game, data));
    }

    // (Optional: if you want mouse-click to also go back, add:)
    /*
    else if (event.type == sf::Event::MouseButtonPressed &&
             event.mouseButton.button == sf::Mouse::Left)
    {
        game.changeState(new MainMenuState(game, data));
    }
    */
}

void LeaderboardState::update(float /*dt*/)
{
    // nothing animated yet
}

void LeaderboardState::render(sf::RenderWindow& window)
{
    window.draw(title);
    window.draw(info);

    for (int i = 0; i < rowCount; ++i)
    {
        window.draw(rows[i]);
    }
}
