#include "MatchResultState.h"
#include "Game.h"
#include "GameData.h"
#include "GameRoomState.h"
#include "SinglePlayerMenuState.h"
#include "MainMenuState.h"
#include "GameState.h"
#include "UIStyle.h"

#include <sstream>

namespace
{
    void continueBracket(Game& game, GameData& data)
    {
        if (data.lastWinner && data.roomWinnerCount < 8)
            data.roomWinners[data.roomWinnerCount++] = data.lastWinner;

        data.roomCurrentMatchStart += 2;

        if (data.roomCurrentMatchStart < data.roomPlayerCount)
        {
            data.currentPlayer = data.roomPlayers[data.roomCurrentMatchStart];
            data.coopPlayer = data.roomPlayers[data.roomCurrentMatchStart + 1];
            game.changeState(new GameState(game, data, 2));
            return;
        }

        if (data.roomWinnerCount > 1)
        {
            for (int i = 0; i < data.roomWinnerCount; ++i)
            {
                data.roomPlayers[i] = data.roomWinners[i];
                data.roomWinners[i] = nullptr;
            }
            for (int i = data.roomWinnerCount; i < 8; ++i)
                data.roomPlayers[i] = nullptr;

            data.roomPlayerCount = data.roomWinnerCount;
            data.roomWinnerCount = 0;
            data.roomCurrentMatchStart = 0;
            data.currentPlayer = data.roomPlayers[0];
            data.coopPlayer = data.roomPlayers[1];
            game.changeState(new GameState(game, data, 2));
            return;
        }

        if (data.roomWinnerCount == 1)
        {
            data.roomPlayers[0] = data.roomWinners[0];
            data.lastWinner = data.roomWinners[0];
        }
        data.roomPlayerCount = data.roomWinnerCount;
        data.roomWinnerCount = 0;
        data.roomCurrentMatchStart = 0;
        data.matchmakingActive = false;
        game.changeState(new GameRoomState(game, data));
    }
}

MatchResultState::MatchResultState(Game& game, GameData& data, int winnerCode, int finishedMode)
    : State(game, data), winnerCode(winnerCode), finishedMode(finishedMode), bracketHasNextMatch(false)
{
    titleText.setFont(ui::headingFont());
    titleText.setCharacterSize(30);
    titleText.setPosition(90.f, 66.f);

    infoText.setFont(ui::bodyFont());
    infoText.setCharacterSize(19);
    infoText.setPosition(92.f, 150.f);

    helperText.setFont(ui::bodyFont());
    helperText.setCharacterSize(16);
    helperText.setPosition(92.f, 322.f);

    if (finishedMode != 1 && data.matchmakingActive)
    {
        const int projectedWinnerCount = data.roomWinnerCount + (data.lastWinner ? 1 : 0);
        const int projectedNextStart = data.roomCurrentMatchStart + 2;
        bracketHasNextMatch = (projectedNextStart < data.roomPlayerCount) || (projectedWinnerCount > 1);
    }

    std::ostringstream oss;
    if (finishedMode == 1)
    {
        titleText.setString("Game Over");
        int score = data.currentPlayer ? data.currentPlayer->score : 0;
        int highScore = data.currentPlayer ? data.currentPlayer->highScore : 0;
        oss << "Final score: " << score << "\n"
            << "High score: " << highScore;
        helperText.setString("R = restart single player | M / ESC = back to the single-player menu");
    }
    else
    {
        if (data.matchmakingActive)
            titleText.setString(bracketHasNextMatch ? "Match Complete" : "Tournament Winner");
        else
            titleText.setString("Match Finished");

        if (winnerCode == 1)
            oss << "Winner: " << (data.currentPlayer ? data.currentPlayer->username : std::string("Player 1")) << "\n";
        else if (winnerCode == 2)
            oss << "Winner: " << (data.coopPlayer ? data.coopPlayer->username : std::string("Player 2")) << "\n";
        else
            oss << "Result: Tie\n";

        oss << "P1 score: " << (data.currentPlayer ? data.currentPlayer->score : 0) << "\n"
            << "P2 score: " << (data.coopPlayer ? data.coopPlayer->score : 0);

        if (data.matchmakingActive)
        {
            oss << "\n\nBracket order:\n"
                << "1) Match 1: two highest-scoring players\n"
                << "2) Match 2: remaining two players\n"
                << "3) Final: winners of both matches";
            helperText.setString(bracketHasNextMatch
                ? "ENTER / SPACE = continue bracket | ESC = return to the main menu"
                : "ENTER / SPACE / M = return to the matchmaking room | ESC = main menu");
        }
        else
        {
            helperText.setString("R = rematch | M = matchmaking room | ESC = main menu");
        }
    }

    infoText.setString(oss.str());
    ui::wrapText(infoText, 500.f);
    ui::wrapText(helperText, 500.f);
}

void MatchResultState::handleEvent(sf::Event& e)
{
    if (e.type != sf::Event::KeyPressed)
        return;

    if (finishedMode == 1)
    {
        if (e.key.code == sf::Keyboard::R)
        {
            game.changeState(new GameState(game, data, 1));
            return;
        }
        if (e.key.code == sf::Keyboard::M || e.key.code == sf::Keyboard::Escape)
        {
            game.changeState(new SinglePlayerMenuState(game, data));
            return;
        }
    }
    else
    {
        if (data.matchmakingActive)
        {
            if (e.key.code == sf::Keyboard::Enter || e.key.code == sf::Keyboard::Space || (!bracketHasNextMatch && e.key.code == sf::Keyboard::M))
            {
                continueBracket(game, data);
                return;
            }
            if (e.key.code == sf::Keyboard::Escape)
            {
                data.matchmakingActive = false;
                game.changeState(new MainMenuState(game, data));
                return;
            }
        }
        else
        {
            if (e.key.code == sf::Keyboard::R)
            {
                game.changeState(new GameState(game, data, 2));
                return;
            }
            if (e.key.code == sf::Keyboard::M)
            {
                game.changeState(new GameRoomState(game, data));
                return;
            }
            if (e.key.code == sf::Keyboard::Escape)
            {
                game.changeState(new MainMenuState(game, data));
                return;
            }
        }
    }
}

void MatchResultState::update(float)
{
}

void MatchResultState::render(sf::RenderWindow& window)
{
    ui::Palette palette = ui::getPalette(data.inventory ? data.inventory->getCurrentThemeID() : 1);
    ui::drawBackdrop(window, palette, 0.f);
    ui::drawPanel(window, sf::FloatRect(76.f, 44.f, 560.f, 312.f), palette, true);
    ui::drawHeader(window, titleText, nullptr, palette);

    infoText.setFillColor(palette.warning);
    helperText.setFillColor(palette.textSecondary);

    window.draw(infoText);
    window.draw(helperText);
}
