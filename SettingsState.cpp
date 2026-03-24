#include "SettingsState.h"
#include "Game.h"
#include "GameData.h"
#include "MainMenuState.h"
#include "UIStyle.h"

#include <sstream>

SettingsState::SettingsState(Game& game, GameData& data)
    : State(game, data)
{
    titleText.setFont(ui::headingFont());
    titleText.setCharacterSize(30);
    titleText.setString("Settings");
    ui::centerText(titleText, sf::FloatRect(82.f, 26.f, 556.f, 38.f));

    sectionText.setFont(ui::bodyFont());
    sectionText.setCharacterSize(21);
    sectionText.setPosition(118.f, 104.f);
    sectionText.setString("Difficulty");

    optionsText.setFont(ui::bodyFont());
    optionsText.setCharacterSize(20);
    optionsText.setPosition(118.f, 146.f);

    detailsText.setFont(ui::bodyFont());
    detailsText.setCharacterSize(17);
    detailsText.setPosition(118.f, 252.f);

    helperText.setFont(ui::bodyFont());
    helperText.setCharacterSize(15);
    helperText.setPosition(118.f, 360.f);
    helperText.setString("Press 1 for Easy, 2 for Normal, 3 for Hard. ESC returns to the main menu.");
    ui::wrapText(helperText, 480.f);

    rebuildText();
}

void SettingsState::rebuildText()
{
    std::ostringstream options;
    options << (data.difficulty == 0 ? "> " : "  ") << "Easy\n";
    options << (data.difficulty == 1 ? "> " : "  ") << "Normal\n";
    options << (data.difficulty == 2 ? "> " : "  ") << "Hard";
    optionsText.setString(options.str());

    std::ostringstream details;
    details << "Easy - fewer enemies and slower progression\n";
    details << "Normal - balanced default mode\n";
    details << "Hard - more enemies and faster movement";
    detailsText.setString(details.str());
    ui::wrapText(detailsText, 480.f);
}

void SettingsState::handleEvent(sf::Event& event)
{
    if (event.type != sf::Event::KeyPressed)
        return;

    if (event.key.code == sf::Keyboard::Escape)
    {
        game.changeState(new MainMenuState(game, data));
        return;
    }
    if (event.key.code == sf::Keyboard::Num1)
        data.difficulty = 0;
    else if (event.key.code == sf::Keyboard::Num2)
        data.difficulty = 1;
    else if (event.key.code == sf::Keyboard::Num3)
        data.difficulty = 2;

    rebuildText();
}

void SettingsState::update(float)
{
}

void SettingsState::render(sf::RenderWindow& window)
{
    ui::Palette palette = ui::getPalette(data.inventory ? data.inventory->getCurrentThemeID() : 1);
    ui::drawBackdrop(window, palette, 0.f);
    ui::drawPanel(window, sf::FloatRect(82.f, 22.f, 556.f, 390.f), palette, true);
    ui::drawHeader(window, titleText, nullptr, palette);

    sectionText.setFillColor(palette.textPrimary);
    optionsText.setFillColor(palette.accent);
    detailsText.setFillColor(palette.textPrimary);
    helperText.setFillColor(palette.textSecondary);

    window.draw(sectionText);
    window.draw(optionsText);
    window.draw(detailsText);
    window.draw(helperText);
}
