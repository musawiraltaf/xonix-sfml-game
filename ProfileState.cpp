#include "ProfileState.h"
#include "Game.h"
#include "GameData.h"
#include "MainMenuState.h"
#include "GameLogic.h"     // for N, M, ts
#include <sstream>

ProfileState::ProfileState(Game& game, GameData& data)
    : State(game, data)
{
    font.loadFromFile("ariblk.ttf");

    // --- Title ---
    title.setFont(font);
    title.setCharacterSize(32);
    title.setFillColor(sf::Color::White);
    title.setString("Player Profile");

    // Center title horizontally
    float winWidth = static_cast<float>(N * ts);
    sf::FloatRect tr = title.getLocalBounds();
    title.setPosition(
        (winWidth - tr.width) / 2.f - tr.left,
        40.f
    );

    // --- Body text (stats) ---
    body.setFont(font);
    body.setCharacterSize(22);
    body.setFillColor(sf::Color(220, 220, 220));
    body.setPosition(80.f, 120.f);

    // --- Helper text at bottom ---
    helper.setFont(font);
    helper.setCharacterSize(18);
    helper.setFillColor(sf::Color(180, 180, 180));
    helper.setString("ESC = Back to Main Menu");
    helper.setPosition(80.f, static_cast<float>(M * ts) - 50.f);

    rebuildText();
}

void ProfileState::rebuildText()
{
    std::ostringstream oss;

    if (!data.currentPlayer)
    {
        oss << "No player is currently logged in.\n\n"
            << "Login to see your profile statistics.";
    }
    else
    {
        const Player& p = *data.currentPlayer;

        oss << "Username: " << p.username << "\n"
            << "Total Points: " << p.totalPoints << "\n"
            << "High Score:  " << p.highScore << "\n"
            << "Wins:        " << p.wins << "\n"
            << "Losses:      " << p.losses << "\n";

        // Session info (current run)
        oss << "Current Session:\n"
            << "  Score:    " << p.score << "\n"
            << "  Power-ups: " << p.powerUps << "\n";
    }

    body.setString(oss.str());
}

void ProfileState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape)
    {
        // Back to main menu
        game.changeState(new MainMenuState(game, data));
    }
}

void ProfileState::update(float /*dt*/)
{
    // Nothing animated for now
}

void ProfileState::render(sf::RenderWindow& window)
{
    window.draw(title);
    window.draw(body);
    window.draw(helper);
}
