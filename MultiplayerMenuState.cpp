#include "MultiplayerMenuState.h"
#include "Game.h"
#include "GameData.h"
#include "MainMenuState.h"
#include "GameRoomState.h"
#include "LoginState.h"
#include "GameLogic.h"
#include "UIStyle.h"

MultiplayerMenuState::MultiplayerMenuState(Game& game, GameData& data)
    : State(game, data), animTime(0.f), hoveredIndex(-1)
{

    const float buttonWidth = 390.f;
    const float buttonHeight = 44.f;
    const float gap = 18.f;
    const float winWidth = static_cast<float>(N * ts);
    const float winHeight = static_cast<float>(M * ts);

    titleText.setFont(ui::headingFont());
    titleText.setCharacterSize(30);
    titleText.setString("Multiplayer");
    ui::centerText(titleText, sf::FloatRect(0.f, 26.f, winWidth, 34.f));

    subtitleText.setFont(ui::bodyFont());
    subtitleText.setCharacterSize(16);
    subtitleText.setString("Play locally or build a queue for the game room");
    ui::centerText(subtitleText, sf::FloatRect(0.f, 64.f, winWidth, 24.f));

    helperText.setFont(ui::bodyFont());
    helperText.setCharacterSize(15);
    helperText.setString("Local 2-player uses two separate accounts. ESC returns.");
    ui::centerText(helperText, sf::FloatRect(0.f, winHeight - 34.f, winWidth, 20.f));

    float totalHeight = BUTTON_COUNT * buttonHeight + (BUTTON_COUNT - 1) * gap;
    float startY = (winHeight - totalHeight) / 2.f;
    float startX = (winWidth - buttonWidth) / 2.f;

    setupButton(0, "Local 2-Player", startX, startY + 0.f * (buttonHeight + gap));
    setupButton(1, "Game Room", startX, startY + 1.f * (buttonHeight + gap));
    setupButton(2, "Back", startX, startY + 2.f * (buttonHeight + gap));
}

void MultiplayerMenuState::setupButton(int index, const char* text, float x, float y)
{
    ui::initializeButton(buttons[index].box, buttons[index].label, ui::bodyFont(), text,
                         sf::FloatRect(x, y, 390.f, 44.f), 18);
}

void MultiplayerMenuState::handleEvent(sf::Event& event)
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
        game.changeState(new MainMenuState(game, data));
    }
}

void MultiplayerMenuState::update(float dt)
{
    animTime += dt;
}

void MultiplayerMenuState::render(sf::RenderWindow& window)
{
    ui::Palette palette = ui::getPalette(data.inventory ? data.inventory->getCurrentThemeID() : 1);
    ui::drawBackdrop(window, palette, animTime);
    ui::drawPanel(window, sf::FloatRect(136.f, 24.f, 448.f, 376.f), palette, true);
    ui::drawHeader(window, titleText, &subtitleText, palette);

    helperText.setFillColor(palette.textSecondary);
    window.draw(helperText);

    for (int i = 0; i < BUTTON_COUNT; ++i)
    {
        ui::styleButton(buttons[i].box, buttons[i].label, palette, i == hoveredIndex, false);
        window.draw(buttons[i].box);
        window.draw(buttons[i].label);
    }
}

void MultiplayerMenuState::handleClick(int index)
{
    switch (index)
    {
    case 0:
        game.changeState(new LoginState(game, data, LOGIN_FOR_COOP_EXTRA));
        break;
    case 1:
        game.changeState(new GameRoomState(game, data));
        break;
    case 2:
        game.changeState(new MainMenuState(game, data));
        break;
    default:
        break;
    }
}
