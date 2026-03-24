#include "MainMenuState.h"
#include "Game.h"
#include "GameData.h"
#include "SinglePlayerMenuState.h"
#include "MultiplayerMenuState.h"
#include "LeaderboardState.h"
#include "LoginState.h"
#include "ProfileState.h"
#include "ThemeMenuState.h"
#include "FriendsState.h"
#include "SettingsState.h"
#include "GameLogic.h"
#include "UIStyle.h"

MainMenuState::MainMenuState(Game& game, GameData& data)
    : State(game, data), animTime(0.f), hoveredIndex(-1)
{
    if (data.sessionPlayer)
    {
        data.currentPlayer = data.sessionPlayer;
        data.coopPlayer = nullptr;
        data.playerCount = 1;
    }
    const float buttonWidth = 392.f;
    const float buttonHeight = 34.f;
    const float gap = 7.f;
    const float winWidth = static_cast<float>(N * ts);
    const float winHeight = static_cast<float>(M * ts);

    titleText.setFont(ui::headingFont());
    titleText.setCharacterSize(30);
    titleText.setString("XONIX");
    ui::centerText(titleText, sf::FloatRect(0.f, 18.f, winWidth, 38.f));

    subtitleText.setFont(ui::bodyFont());
    subtitleText.setCharacterSize(15);
    subtitleText.setString(data.sessionPlayer ? "Logged in as " + data.sessionPlayer->username : std::string("Choose a mode to continue"));
    ui::fitTextToWidth(subtitleText, 320.f, 12);
    ui::centerText(subtitleText, sf::FloatRect(0.f, 62.f, winWidth, 20.f));

    footerText.setFont(ui::bodyFont());
    footerText.setCharacterSize(15);
    footerText.setString("ESC closes the game from this menu");
    ui::centerText(footerText, sf::FloatRect(0.f, winHeight - 30.f, winWidth, 18.f));

    float startY = 92.f;
    float startX = (winWidth - buttonWidth) / 2.f;

    setupButton(0, "Single Player", startX, startY + 0.f * (buttonHeight + gap));
    setupButton(1, "Multiplayer", startX, startY + 1.f * (buttonHeight + gap));
    setupButton(2, "Profile", startX, startY + 2.f * (buttonHeight + gap));
    setupButton(3, "Friends", startX, startY + 3.f * (buttonHeight + gap));
    setupButton(4, "Leaderboard", startX, startY + 4.f * (buttonHeight + gap));
    setupButton(5, "Inventory / Themes", startX, startY + 5.f * (buttonHeight + gap));
    setupButton(6, "Settings", startX, startY + 6.f * (buttonHeight + gap));
    setupButton(7, "Logout", startX, startY + 7.f * (buttonHeight + gap));
}

void MainMenuState::setupButton(int index, const char* text, float x, float y)
{
    ui::initializeButton(buttons[index].box, buttons[index].label, ui::bodyFont(), text,
                         sf::FloatRect(x, y, 392.f, 34.f), 18);
}

void MainMenuState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::MouseMoved)
    {
        sf::Vector2f worldPos = game.getWindow().mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        hoveredIndex = -1;
        for (int i = 0; i < BUTTON_COUNT; ++i)
        {
            if (buttons[i].box.getGlobalBounds().contains(worldPos))
            {
                hoveredIndex = i;
                break;
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f worldPos = game.getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        for (int i = 0; i < BUTTON_COUNT; ++i)
        {
            if (buttons[i].box.getGlobalBounds().contains(worldPos))
            {
                handleClick(i);
                break;
            }
        }
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        game.getWindow().close();
    }
}

void MainMenuState::update(float dt)
{
    animTime += dt;
}

void MainMenuState::render(sf::RenderWindow& window)
{
    ui::Palette palette = ui::getPalette(data.inventory ? data.inventory->getCurrentThemeID() : 1);
    ui::drawBackdrop(window, palette, animTime);
    ui::drawPanel(window, sf::FloatRect(126.f, 14.f, 468.f, 420.f), palette, true);
    ui::drawHeader(window, titleText, &subtitleText, palette);

    footerText.setFillColor(palette.textSecondary);
    window.draw(footerText);

    for (int i = 0; i < BUTTON_COUNT; ++i)
    {
        ui::styleButton(buttons[i].box, buttons[i].label, palette, i == hoveredIndex, i == 7);
        window.draw(buttons[i].box);
        window.draw(buttons[i].label);
    }
}

void MainMenuState::handleClick(int index)
{
    switch (index)
    {
    case 0: game.changeState(new SinglePlayerMenuState(game, data)); break;
    case 1: game.changeState(new MultiplayerMenuState(game, data)); break;
    case 2: game.changeState(new ProfileState(game, data)); break;
    case 3: game.changeState(new FriendsState(game, data)); break;
    case 4: game.changeState(new LeaderboardState(game, data)); break;
    case 5: game.changeState(new ThemeMenuState(game, data)); break;
    case 6: game.changeState(new SettingsState(game, data)); break;
    case 7:
        data.sessionPlayer = nullptr;
        data.currentPlayer = nullptr;
        data.coopPlayer = nullptr;
        data.playerCount = 0;
        game.changeState(new LoginState(game, data));
        break;
    default: break;
    }
}
