#include "ProfileState.h"
#include "Game.h"
#include "GameData.h"
#include "MainMenuState.h"
#include "GameLogic.h"
#include "UserDataIO.h"
#include "UIStyle.h"

#include <sstream>

ProfileState::ProfileState(Game& game, GameData& data)
    : State(game, data)
{
    title.setFont(ui::headingFont());
    title.setCharacterSize(32);
    title.setString("Player Profile");
    ui::centerText(title, sf::FloatRect(42.f, 26.f, 636.f, 42.f));

    leftTitle.setFont(ui::bodyFont());
    leftTitle.setCharacterSize(18);
    leftTitle.setPosition(72.f, 102.f);
    leftTitle.setString("Profile Summary");

    leftBody.setFont(ui::bodyFont());
    leftBody.setCharacterSize(16);
    leftBody.setPosition(72.f, 134.f);

    rightTitle.setFont(ui::bodyFont());
    rightTitle.setCharacterSize(18);
    rightTitle.setPosition(404.f, 102.f);
    rightTitle.setString("Current Session");

    rightBody.setFont(ui::bodyFont());
    rightBody.setCharacterSize(16);
    rightBody.setPosition(404.f, 134.f);

    helper.setFont(ui::bodyFont());
    helper.setCharacterSize(14);
    helper.setString("ESC returns to the main menu.");
    helper.setPosition(72.f, 382.f);

    rebuildText();
}

void ProfileState::rebuildText()
{
    if (!data.currentPlayer)
    {
        leftBody.setString("No player is currently signed in.");
        rightBody.setString("");
        return;
    }

    const Player& p = *data.currentPlayer;
    int idx = findPlayerPointerIndex(data, data.currentPlayer);
    int friendCount = data.friends.getFriendCount(idx);
    int pendingCount = data.friends.getPendingCount(idx);

    std::ostringstream left;
    left << "Username: " << p.username << "\n";
    left << "Total Points: " << p.totalPoints << "\n";
    left << "High Score: " << p.highScore << "\n";
    left << "Wins: " << p.wins << "\n";
    left << "Losses: " << p.losses << "\n";
    left << "Friends: " << friendCount << "\n";
    left << "Pending Requests: " << pendingCount << "\n";
    left << "Theme: " << (data.inventory ? data.inventory->getCurrentThemeName() : std::string("Inkforged"));
    leftBody.setString(left.str());

    std::ostringstream right;
    right << "Score: " << p.score << "\n";
    right << "Power-ups: " << p.powerUps << "\n";
    right << "Bonus Count: " << p.bonusCount << "\n";
    right << "Bonus Threshold: " << p.bonusThreshold << "\n";
    right << "Next Power-up At: " << p.nextPowerupScore;
    rightBody.setString(right.str());

    ui::wrapText(leftBody, 260.f);
    ui::wrapText(rightBody, 200.f);
}

void ProfileState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        game.changeState(new MainMenuState(game, data));
}

void ProfileState::update(float)
{
}

void ProfileState::render(sf::RenderWindow& window)
{
    ui::Palette palette = ui::getPalette(data.inventory ? data.inventory->getCurrentThemeID() : 1);
    ui::drawBackdrop(window, palette, 0.f);
    ui::drawPanel(window, sf::FloatRect(42.f, 20.f, 636.f, 396.f), palette, true);
    ui::drawHeader(window, title, nullptr, palette);

    leftTitle.setFillColor(palette.accent);
    rightTitle.setFillColor(palette.accent);
    leftBody.setFillColor(palette.textPrimary);
    rightBody.setFillColor(palette.textPrimary);
    helper.setFillColor(palette.textSecondary);

    ui::drawPanel(window, sf::FloatRect(58.f, 92.f, 292.f, 270.f), palette, false, 4.f);
    ui::drawPanel(window, sf::FloatRect(388.f, 92.f, 234.f, 168.f), palette, false, 4.f);

    window.draw(leftTitle);
    window.draw(leftBody);
    window.draw(rightTitle);
    window.draw(rightBody);
    window.draw(helper);
}
