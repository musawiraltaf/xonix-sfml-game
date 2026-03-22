#include "MultiplayerMenuState.h"
#include "Game.h"
#include "GameData.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "GameRoomState.h"
#include "LoginState.h"
#include "GameLogic.h"    // for N, M, ts

MultiplayerMenuState::MultiplayerMenuState(Game& game, GameData& data)
    : State(game, data),
    hoveredIndex(-1)
{
    font.loadFromFile("ariblk.ttf");

    const float buttonWidth = 400.f;
    const float buttonHeight = 40.f;
    const float gap = 20.f;   // nice spacing

    // Use the logical game size (grid size)
    const float winWidth = static_cast<float>(N * ts);
    const float winHeight = static_cast<float>(M * ts);

    float totalHeight = BUTTON_COUNT * buttonHeight + (BUTTON_COUNT - 1) * gap;

    // Center the stack of buttons
    float startY = (winHeight - totalHeight) / 2.f;
    float startX = (winWidth - buttonWidth) / 2.f;

    setupButton(0, "Local 2-Player", startX, startY + 0 * (buttonHeight + gap));
    setupButton(1, "Game Room", startX, startY + 1 * (buttonHeight + gap));
    setupButton(2, "Back", startX, startY + 2 * (buttonHeight + gap));
}

void MultiplayerMenuState::setupButton(int index, const char* text, float x, float y)
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

    // Center the text inside the button
    sf::FloatRect r = buttons[index].label.getLocalBounds();
    float tx = x + (buttonWidth - r.width) / 2.f - r.left;
    float ty = y + (buttonHeight - r.height) / 2.f - r.top;
    buttons[index].label.setPosition(tx, ty);
}

void MultiplayerMenuState::handleEvent(sf::Event& event)
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
        // ESC from multiplayer menu returns to main menu
        game.changeState(new MainMenuState(game, data));
    }
}

void MultiplayerMenuState::update(float /*dt*/)
{
    // nothing animated yet
}

void MultiplayerMenuState::render(sf::RenderWindow& window)
{
    for (int i = 0; i < BUTTON_COUNT; ++i)
    {
        window.draw(buttons[i].box);
        window.draw(buttons[i].label);
    }
}

void MultiplayerMenuState::handleClick(int index)
{
    switch (index)
    {
    case 0: // Local 2-Player
        if (data.playerCount < 2)
        {
            // Ask an extra player to log in; after login,
            // LoginState will set coopPlayer & playerCount
            // and send us back to the main menu.
            game.changeState(new LoginState(game, data, LOGIN_FOR_COOP_EXTRA));
        }
        else
        {
            // Already have two players -> start the co-op game
            game.changeState(new GameState(game, data, 2));
        }
        break;

    case 1: // Game Room (placeholder matchmaking state)
        game.changeState(new GameRoomState(game, data));
        break;

    case 2: // Back to main menu
        game.changeState(new MainMenuState(game, data));
        break;

    default:
        break;
    }
}
