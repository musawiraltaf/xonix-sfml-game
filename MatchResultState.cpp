#include "MatchResultState.h"
#include "Game.h"
#include "GameRoomState.h"
#include <sstream>

MatchResultState::MatchResultState(Game& game, GameData& data, int w)
    : State(game, data)
    , winnerCode(w)
{
    font.loadFromFile("ariblk.ttf");

    // --- Title ---
    titleText.setFont(font);
    titleText.setCharacterSize(32);
    titleText.setFillColor(sf::Color::White);
    titleText.setString("Match Finished");
    titleText.setPosition(80.f, 80.f);

    // --- Winner text (NO player pointers, super safe) ---
    infoText.setFont(font);
    infoText.setCharacterSize(24);
    infoText.setFillColor(sf::Color::Yellow);

    std::ostringstream oss;
    if (winnerCode == 1)
        oss << "Winner: Player 1";
    else if (winnerCode == 2)
        oss << "Winner: Player 2";
    else
        oss << "Result: Tie";

    infoText.setString(oss.str());
    infoText.setPosition(80.f, 140.f);

    // --- Helper text ---
    helperText.setFont(font);
    helperText.setCharacterSize(18);
    helperText.setFillColor(sf::Color::Cyan);
    helperText.setString("Press ENTER or ESC to return to Matchmaking Room");
    helperText.setPosition(80.f, 220.f);
}

void MatchResultState::handleEvent(sf::Event& e)
{
    if (e.type == sf::Event::KeyPressed)
    {
        if (e.key.code == sf::Keyboard::Enter ||
            e.key.code == sf::Keyboard::Escape)
        {
            // Go back to matchmaking room
            game.changeState(new GameRoomState(game, data));
            return;
        }
    }
}

void MatchResultState::update(float)
{
    // no animations
}

void MatchResultState::render(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
    window.draw(titleText);
    window.draw(infoText);
    window.draw(helperText);
}
