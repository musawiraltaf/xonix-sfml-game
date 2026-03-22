#include "MainMenuState.h"
#include "Game.h"
#include "GameData.h"
#include "SinglePlayerMenuState.h"
#include "MultiplayerMenuState.h"
#include "LeaderboardState.h"
#include "LoginState.h"
#include "GameState.h"
#include "GameRoomState.h"
#include "ProfileState.h"
#include "ThemeMenuState.h"



MainMenuState::MainMenuState(Game& game, GameData& data)
    : State(game, data),
    hoveredIndex(-1)
{
    font.loadFromFile("ariblk.ttf");

    const float buttonWidth = 400.f;
    const float buttonHeight = 40.f;
    const float gap = 10.f;   // small gap so 8 buttons fit

    const float winWidth = static_cast<float>(N * ts);
    const float winHeight = static_cast<float>(M * ts);

    float totalHeight = BUTTON_COUNT * buttonHeight + (BUTTON_COUNT - 1) * gap;

    float startY = (winHeight - totalHeight) / 2.f;
    float startX = (winWidth - buttonWidth) / 2.f;

    setupButton(0, "Single Player", startX, startY + 0 * (buttonHeight + gap));
    setupButton(1, "Multiplayer", startX, startY + 1 * (buttonHeight + gap));
    setupButton(2, "Profile", startX, startY + 2 * (buttonHeight + gap));
    setupButton(3, "Friends", startX, startY + 3 * (buttonHeight + gap));
    setupButton(4, "Leaderboard", startX, startY + 4 * (buttonHeight + gap));
    setupButton(5, "Inventory / Themes", startX, startY + 5 * (buttonHeight + gap));
    setupButton(6, "Settings", startX, startY + 6 * (buttonHeight + gap));
    setupButton(7, "Logout", startX, startY + 7 * (buttonHeight + gap));
}

void MainMenuState::setupButton(int index, const char* text, float x, float y)
{
    const float buttonWidth = 400.f;
    const float buttonHeight = 40.f;

    buttons[index].box.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    buttons[index].box.setFillColor(sf::Color(80, 80, 80));
    buttons[index].box.setPosition(x, y);

    buttons[index].label.setFont(font);
    buttons[index].label.setCharacterSize(20);
    buttons[index].label.setFillColor(sf::Color::White);
    buttons[index].label.setString(text);

    // Center text inside the rectangle
    sf::FloatRect textRect = buttons[index].label.getLocalBounds();
    float textX = x + (buttonWidth - textRect.width) / 2.f - textRect.left;
    float textY = y + (buttonHeight - textRect.height) / 2.f - textRect.top;
    buttons[index].label.setPosition(textX, textY);
}


void MainMenuState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::MouseMoved)
    {
        sf::Vector2i pixelPos(event.mouseMove.x, event.mouseMove.y);
        sf::Vector2f worldPos = game.getWindow().mapPixelToCoords(pixelPos);

        hoveredIndex = -1;
        for (int i = 0; i < BUTTON_COUNT; ++i)
        {
            if (buttons[i].box.getGlobalBounds().contains(worldPos))
            {
                hoveredIndex = i;
                break;
            }
        }

        for (int i = 0; i < BUTTON_COUNT; ++i)
        {
            if (i == hoveredIndex)
                buttons[i].box.setFillColor(sf::Color(120, 120, 120));
            else
                buttons[i].box.setFillColor(sf::Color(80, 80, 80));
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f worldPos = game.getWindow().mapPixelToCoords(pixelPos);

        for (int i = 0; i < BUTTON_COUNT; ++i)
        {
            if (buttons[i].box.getGlobalBounds().contains(worldPos))
            {
                handleClick(i);
                break;
            }
        }
    }
    else if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape)
    {
        // ESC from main menu exits the game
        game.getWindow().close();
    }
}

void MainMenuState::update(float /*dt*/)
{
    // no animations yet
}

void MainMenuState::render(sf::RenderWindow& window)
{
    for (int i = 0; i < BUTTON_COUNT; ++i)
    {
        window.draw(buttons[i].box);
        window.draw(buttons[i].label);
    }
}

void MainMenuState::handleClick(int index)
{
    switch (index)
    {
    case 0: // "Single Player"
        // go to the Single Player submenu (Start New Game / Load / Back)
        game.changeState(new SinglePlayerMenuState(game, data));
        break;

    case 1: // "Multiplayer"
        // go to Multiplayer submenu (Local 2-Player / Game Room / Back)
        game.changeState(new MultiplayerMenuState(game, data));
        break;

    case 2: // "Profile"
        game.changeState(new ProfileState(game, data));
        break;

    case 3: // "Friends"
        // TODO: implement later
        break;

    case 4: // "Leaderboard"
        // 🔹 THIS is what makes clicking Leaderboard open the leaderboard screen
        game.changeState(new LeaderboardState(game, data));
        break;

    case 5: // "Inventory / Themes"
        game.changeState(new ThemeMenuState(game, data));

        break;

    case 6: // "Settings"
        // TODO: implement later
        break;

    case 7: // "Logout"
        // Clear current player and go back to Login
        data.currentPlayer = nullptr;
        data.coopPlayer = nullptr;
        data.playerCount = 0;
        game.changeState(new LoginState(game, data));
        break;

    default:
        break;
    }
}
