#include "SinglePlayerMenuState.h"
#include "Game.h"
#include "GameData.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "GameLogic.h"
#include "UIStyle.h"

#include <sstream>

SinglePlayerMenuState::SinglePlayerMenuState(Game& game, GameData& data)
    : State(game, data), animTime(0.f), hoveredIndex(-1), showingSaveList(false), saveCount(0), selectedSave(0),
      savePanelRect(162.f, 214.f, 396.f, 138.f)
{
    const float buttonWidth = 360.f;
    const float buttonHeight = 42.f;
    const float gap = 14.f;
    const float winWidth = static_cast<float>(N * ts);
    const float winHeight = static_cast<float>(M * ts);

    titleText.setFont(ui::headingFont());
    titleText.setCharacterSize(30);
    titleText.setString("Single Player");
    ui::centerText(titleText, sf::FloatRect(0.f, 26.f, winWidth, 36.f));

    subtitleText.setFont(ui::bodyFont());
    subtitleText.setCharacterSize(16);
    subtitleText.setString("Start a new run or pick one of your saved games");
    ui::centerText(subtitleText, sf::FloatRect(0.f, 64.f, winWidth, 24.f));

    float startY = 108.f;
    float startX = (winWidth - buttonWidth) / 2.f;

    setupButton(0, "Start New Game", startX, startY + 0.f * (buttonHeight + gap));
    setupButton(1, "Load Saved Game", startX, startY + 1.f * (buttonHeight + gap));
    setupButton(2, "Back", startX, startY + 2.f * (buttonHeight + gap));

    promptText.setFont(ui::bodyFont());
    promptText.setCharacterSize(15);
    promptText.setPosition(startX, 312.f);
    promptText.setString("Press F5 during gameplay to save. Load Saved Game opens your recent saves.");
    ui::wrapText(promptText, buttonWidth);

    statusText.setFont(ui::bodyFont());
    statusText.setCharacterSize(15);
    statusText.setPosition(startX, 346.f);
    statusText.setString("");

    saveListText.setFont(ui::bodyFont());
    saveListText.setCharacterSize(14);
    saveListText.setPosition(savePanelRect.left + 14.f, savePanelRect.top + 14.f);

    for (int i = 0; i < MAX_VISIBLE_SAVES; ++i)
        saveRowRects[i] = sf::FloatRect(savePanelRect.left + 10.f, savePanelRect.top + 10.f + i * 24.f, savePanelRect.width - 20.f, 22.f);
}

void SinglePlayerMenuState::setupButton(int index, const char* text, float x, float y)
{
    ui::initializeButton(buttons[index].box, buttons[index].label, ui::bodyFont(), text,
                         sf::FloatRect(x, y, 360.f, 42.f), 18);
}

void SinglePlayerMenuState::refreshSaveList()
{
    saveCount = 0;
    selectedSave = 0;

    if (!data.currentPlayer)
    {
        showingSaveList = false;
        statusText.setFillColor(sf::Color(255, 120, 120));
        statusText.setString("Please sign in before loading a saved game.");
        return;
    }

    saveCount = listSaveGamesForUser(data.currentPlayer->username, availableSaves, MAX_VISIBLE_SAVES);
    showingSaveList = true;
    promptText.setString("Saved games for " + data.currentPlayer->username);
    statusText.setFillColor(sf::Color::White);
    statusText.setString(saveCount > 0
        ? "Use Up/Down or click a save, then press Enter. ESC closes the list."
        : "No saved games were found for this player yet.");
    rebuildSaveListText();
}

void SinglePlayerMenuState::rebuildSaveListText()
{
    std::ostringstream oss;
    if (saveCount <= 0)
    {
        oss << "No saved games available.";
        saveListText.setString(oss.str());
        return;
    }

    for (int i = 0; i < saveCount; ++i)
    {
        const SaveGameMeta& meta = availableSaves[i];
        oss << (i == selectedSave ? "> " : "  ");
        oss << meta.timestampText << " | Level " << meta.level << " | Score " << meta.score;
        if (i + 1 < saveCount)
            oss << '\n';
    }
    saveListText.setString(oss.str());
}

void SinglePlayerMenuState::tryLoadSelectedSave()
{
    if (saveCount <= 0 || selectedSave < 0 || selectedSave >= saveCount)
        return;

    SaveGameRecord record;
    if (!loadGameFromDisk(availableSaves[selectedSave].saveID, record))
    {
        statusText.setFillColor(sf::Color(255, 120, 120));
        statusText.setString("Could not load the selected save.");
        return;
    }
    if (!data.currentPlayer || record.username != data.currentPlayer->username)
    {
        statusText.setFillColor(sf::Color(255, 120, 120));
        statusText.setString("That save belongs to a different player.");
        return;
    }

    game.changeState(new GameState(game, data, 1, &record));
}

void SinglePlayerMenuState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::MouseMoved)
    {
        sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        hoveredIndex = -1;
        for (int i = 0; i < BUTTON_COUNT; ++i)
        {
            if (buttons[i].box.getGlobalBounds().contains(mousePos))
            {
                hoveredIndex = i;
                break;
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        if (showingSaveList)
        {
            for (int i = 0; i < saveCount; ++i)
            {
                if (saveRowRects[i].contains(mousePos))
                {
                    selectedSave = i;
                    rebuildSaveListText();
                    return;
                }
            }
        }

        for (int i = 0; i < BUTTON_COUNT; ++i)
        {
            if (buttons[i].box.getGlobalBounds().contains(mousePos))
            {
                handleClick(i);
                break;
            }
        }
    }
    else if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            if (showingSaveList)
            {
                showingSaveList = false;
                promptText.setString("Press F5 during gameplay to save. Load Saved Game opens your recent saves.");
                statusText.setString("");
            }
            else
            {
                game.changeState(new MainMenuState(game, data));
            }
            return;
        }

        if (showingSaveList)
        {
            if (event.key.code == sf::Keyboard::Up && saveCount > 0 && selectedSave > 0)
            {
                --selectedSave;
                rebuildSaveListText();
            }
            else if (event.key.code == sf::Keyboard::Down && saveCount > 0 && selectedSave + 1 < saveCount)
            {
                ++selectedSave;
                rebuildSaveListText();
            }
            else if (event.key.code == sf::Keyboard::Enter)
            {
                tryLoadSelectedSave();
            }
        }
    }
}

void SinglePlayerMenuState::update(float dt)
{
    animTime += dt;
}

void SinglePlayerMenuState::render(sf::RenderWindow& window)
{
    ui::Palette palette = ui::getPalette(data.inventory ? data.inventory->getCurrentThemeID() : 1);
    ui::drawBackdrop(window, palette, animTime);
    ui::drawPanel(window, sf::FloatRect(132.f, 22.f, 456.f, 386.f), palette, true);
    ui::drawHeader(window, titleText, &subtitleText, palette);

    promptText.setFillColor(palette.textSecondary);
    if (statusText.getString().isEmpty())
        statusText.setFillColor(palette.textSecondary);
    saveListText.setFillColor(palette.textPrimary);

    for (int i = 0; i < BUTTON_COUNT; ++i)
    {
        ui::styleButton(buttons[i].box, buttons[i].label, palette, i == hoveredIndex, i == 2);
        window.draw(buttons[i].box);
        window.draw(buttons[i].label);
    }

    window.draw(promptText);
    window.draw(statusText);

    if (showingSaveList)
    {
        ui::drawPanel(window, savePanelRect, palette, false, 5.f);
        for (int i = 0; i < saveCount; ++i)
        {
            sf::RectangleShape row(sf::Vector2f(saveRowRects[i].width, saveRowRects[i].height));
            row.setPosition(saveRowRects[i].left, saveRowRects[i].top);
            row.setFillColor(i == selectedSave ? ui::withAlpha(palette.buttonHover, 120) : sf::Color::Transparent);
            row.setOutlineThickness(i == selectedSave ? 1.f : 0.f);
            row.setOutlineColor(palette.accent);
            window.draw(row);
        }
        window.draw(saveListText);
    }
}

void SinglePlayerMenuState::handleClick(int index)
{
    switch (index)
    {
    case 0:
        game.changeState(new GameState(game, data, 1));
        break;
    case 1:
        refreshSaveList();
        break;
    case 2:
        game.changeState(new MainMenuState(game, data));
        break;
    default:
        break;
    }
}
