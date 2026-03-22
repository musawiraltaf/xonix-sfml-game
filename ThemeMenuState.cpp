#include "ThemeMenuState.h"
#include "Game.h"
#include "MainMenuState.h"
#include "Inventory.h"      // for InventoryManager in GameData
#include <sstream>

ThemeMenuState::ThemeMenuState(Game& game, GameData& data)
    : State(game, data)
{
    // Load same font you use elsewhere
    font.loadFromFile("ariblk.ttf");

    // ---- Title ----
    titleText.setFont(font);
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color::White);
    titleText.setString("Theme / Inventory");
    titleText.setPosition(60.f, 30.f);

    // ---- Theme list text ----
    themeListText.setFont(font);
    themeListText.setCharacterSize(20);
    themeListText.setFillColor(sf::Color::Yellow);
    themeListText.setPosition(60.f, 90.f);

    // We hardcode the themes to match your Inventory defaults
    std::ostringstream oss;
    oss << "Press a number key to select a theme:\n\n";
    oss << "1 - Classic\n";
    oss << "2 - Dark Mode\n";
    oss << "3 - Ocean\n";
    oss << "4 - Forest\n";
    oss << "5 - Sunset\n";
    oss << "6 - Neon\n";
    oss << "7 - Retro\n";
    oss << "8 - Monochrome\n";
    oss << "9 - Fire\n";
    oss << "0 - Ice (Theme ID 10)\n\n";

    oss << "Current theme: " << data.inventory.getCurrentThemeName() << "\n";

    themeListText.setString(oss.str());

    // ---- Helper text ----
    helperText.setFont(font);
    helperText.setCharacterSize(18);
    helperText.setFillColor(sf::Color::Cyan);
    helperText.setString("ESC = Back to Main Menu");
    helperText.setPosition(60.f, 420.f);
}

void ThemeMenuState::handleEvent(sf::Event& event)
{
    // ESC -> return to main menu
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape)
    {
        game.changeState(new MainMenuState(game, data));
        return;
    }

    // Number keys -> apply theme
    if (event.type == sf::Event::KeyPressed)
    {
        int themeID = -1;

        switch (event.key.code)
        {
        case sf::Keyboard::Num1: themeID = 1; break;
        case sf::Keyboard::Num2: themeID = 2; break;
        case sf::Keyboard::Num3: themeID = 3; break;
        case sf::Keyboard::Num4: themeID = 4; break;
        case sf::Keyboard::Num5: themeID = 5; break;
        case sf::Keyboard::Num6: themeID = 6; break;
        case sf::Keyboard::Num7: themeID = 7; break;
        case sf::Keyboard::Num8: themeID = 8; break;
        case sf::Keyboard::Num9: themeID = 9; break;
        case sf::Keyboard::Num0: themeID = 10; break; // 0 -> Ice
        default:
            break;
        }

        if (themeID != -1)
        {
            // Apply theme using your InventoryManager
            if (data.inventory.applyTheme(themeID))
            {
                // Save player preference if someone is logged in
                if (data.currentPlayer)
                {
                    data.inventory.saveThemePreference(
                        data.currentPlayer->username,
                        themeID
                    );
                }

                // Go back to main menu after selecting
                game.changeState(new MainMenuState(game, data));
                return;
            }
            // If invalid themeID, do nothing (stay on screen)
        }
    }
}

void ThemeMenuState::update(float /*dt*/)
{
    // No animations for now
}

void ThemeMenuState::render(sf::RenderWindow& window)
{
    window.draw(titleText);
    window.draw(themeListText);
    window.draw(helperText);
}
