#include "FriendsState.h"
#include "Game.h"
#include "GameData.h"
#include "MainMenuState.h"
#include "UserDataIO.h"
#include "UIStyle.h"

#include <sstream>

FriendsState::FriendsState(Game& game, GameData& data)
    : State(game, data), usernameInput(), selectedPending(0)
{
    titleText.setFont(ui::headingFont());
    titleText.setCharacterSize(30);
    titleText.setString("Friends");
    ui::centerText(titleText, sf::FloatRect(28.f, 24.f, 664.f, 36.f));

    friendsTitleText.setFont(ui::bodyFont());
    friendsTitleText.setCharacterSize(18);
    friendsTitleText.setPosition(52.f, 126.f);
    friendsTitleText.setString("Friends List");

    pendingTitleText.setFont(ui::bodyFont());
    pendingTitleText.setCharacterSize(18);
    pendingTitleText.setPosition(378.f, 126.f);
    pendingTitleText.setString("Pending Requests");

    friendsText.setFont(ui::bodyFont());
    friendsText.setCharacterSize(15);
    friendsText.setPosition(52.f, 156.f);

    pendingText.setFont(ui::bodyFont());
    pendingText.setCharacterSize(15);
    pendingText.setPosition(378.f, 156.f);

    inputText.setFont(ui::bodyFont());
    inputText.setCharacterSize(17);
    inputText.setPosition(48.f, 94.f);

    helperText.setFont(ui::bodyFont());
    helperText.setCharacterSize(14);
    helperText.setPosition(44.f, 364.f);
    helperText.setString("Type a username and press Enter to send a request. Up and Down move through pending requests. A accepts, D rejects, and ESC goes back.");
    ui::wrapText(helperText, 620.f);

    statusText.setFont(ui::bodyFont());
    statusText.setCharacterSize(14);
    statusText.setPosition(44.f, 404.f);

    rebuildTexts();
}

void FriendsState::rebuildTexts()
{
    int currentIndex = findPlayerPointerIndex(data, data.currentPlayer);

    std::ostringstream friends;
    std::ostringstream pending;

    if (currentIndex < 0)
    {
        friends << "No player is currently signed in.";
        pending << "";
    }
    else
    {
        friends << "Signed in as " << data.currentPlayer->username << "\n\n";
        int friendCount = data.friends.getFriendCount(currentIndex);
        if (friendCount == 0)
        {
            friends << "No friends added yet.";
        }
        else
        {
            for (int i = 0; i < friendCount; ++i)
            {
                int idx = data.friends.getFriendIndexAt(currentIndex, i);
                if (idx >= 0 && idx < data.userCount)
                    friends << i + 1 << ". " << data.users[idx].username << '\n';
            }
        }

        int pendingCount = data.friends.getPendingCount(currentIndex);
        if (pendingCount == 0)
        {
            pending << "No pending requests.";
            selectedPending = 0;
        }
        else
        {
            if (selectedPending >= pendingCount)
                selectedPending = pendingCount - 1;
            if (selectedPending < 0)
                selectedPending = 0;

            for (int i = 0; i < pendingCount; ++i)
            {
                int idx = data.friends.getPendingIndexAt(currentIndex, i);
                pending << (i == selectedPending ? "> " : "  ");
                if (idx >= 0 && idx < data.userCount)
                    pending << data.users[idx].username;
                else
                    pending << "Unknown";
                if (i + 1 < pendingCount)
                    pending << '\n';
            }
        }
    }

    friendsText.setString(friends.str());
    pendingText.setString(pending.str());
    ui::wrapText(friendsText, 270.f);
    ui::wrapText(pendingText, 250.f);

    inputText.setString("Send request to: " + usernameInput);
    ui::truncateTextToWidth(inputText, 590.f);
}

void FriendsState::handleEvent(sf::Event& event)
{
    int currentIndex = findPlayerPointerIndex(data, data.currentPlayer);

    if (event.type == sf::Event::TextEntered)
    {
        if (event.text.unicode >= 32 && event.text.unicode < 127)
        {
            usernameInput.push_back(static_cast<char>(event.text.unicode));
            rebuildTexts();
        }
    }
    else if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            game.changeState(new MainMenuState(game, data));
            return;
        }
        if (event.key.code == sf::Keyboard::Backspace)
        {
            if (!usernameInput.empty())
                usernameInput.pop_back();
        }
        else if (event.key.code == sf::Keyboard::Enter)
        {
            if (currentIndex < 0 || usernameInput.empty())
            {
                statusText.setFillColor(sf::Color::Red);
                statusText.setString("Enter a username first.");
            }
            else if (data.friends.sendRequest(currentIndex, usernameInput))
            {
                data.friends.saveToFile("friends.txt");
                statusText.setFillColor(sf::Color::Green);
                statusText.setString("Friend request sent.");
                usernameInput.clear();
            }
            else
            {
                statusText.setFillColor(sf::Color::Red);
                statusText.setString("Could not send request. Check the username and try again.");
            }
        }
        else if (event.key.code == sf::Keyboard::Up)
        {
            if (selectedPending > 0)
                --selectedPending;
        }
        else if (event.key.code == sf::Keyboard::Down)
        {
            int pendingCount = (currentIndex >= 0) ? data.friends.getPendingCount(currentIndex) : 0;
            if (selectedPending + 1 < pendingCount)
                ++selectedPending;
        }
        else if (event.key.code == sf::Keyboard::A)
        {
            if (currentIndex >= 0 && data.friends.acceptRequest(currentIndex, selectedPending))
            {
                data.friends.saveToFile("friends.txt");
                statusText.setFillColor(sf::Color::Green);
                statusText.setString("Friend request accepted.");
            }
            else
            {
                statusText.setFillColor(sf::Color::Red);
                statusText.setString("No pending request selected.");
            }
        }
        else if (event.key.code == sf::Keyboard::D)
        {
            if (currentIndex >= 0 && data.friends.rejectRequest(currentIndex, selectedPending))
            {
                data.friends.saveToFile("friends.txt");
                statusText.setFillColor(sf::Color::Yellow);
                statusText.setString("Friend request rejected.");
            }
            else
            {
                statusText.setFillColor(sf::Color::Red);
                statusText.setString("No pending request selected.");
            }
        }

        rebuildTexts();
    }
}

void FriendsState::update(float)
{
}

void FriendsState::render(sf::RenderWindow& window)
{
    ui::Palette palette = ui::getPalette(data.inventory ? data.inventory->getCurrentThemeID() : 1);
    ui::drawBackdrop(window, palette, 0.f);
    ui::drawPanel(window, sf::FloatRect(28.f, 16.f, 664.f, 418.f), palette, true);

    sf::RectangleShape inputBox(sf::Vector2f(616.f, 34.f));
    inputBox.setPosition(42.f, 86.f);
    inputBox.setFillColor(ui::withAlpha(palette.buttonFill, 232));
    inputBox.setOutlineThickness(2.f);
    inputBox.setOutlineColor(palette.accent);

    ui::drawPanel(window, sf::FloatRect(42.f, 118.f, 286.f, 222.f), palette, false, 4.f);
    ui::drawPanel(window, sf::FloatRect(368.f, 118.f, 290.f, 222.f), palette, false, 4.f);
    ui::drawHeader(window, titleText, nullptr, palette);

    friendsTitleText.setFillColor(palette.accent);
    pendingTitleText.setFillColor(palette.accent);
    friendsText.setFillColor(palette.textPrimary);
    pendingText.setFillColor(palette.textPrimary);
    inputText.setFillColor(palette.textPrimary);
    helperText.setFillColor(palette.textSecondary);

    if (statusText.getString().isEmpty())
        statusText.setFillColor(palette.textSecondary);

    window.draw(inputBox);
    window.draw(inputText);
    window.draw(friendsTitleText);
    window.draw(pendingTitleText);
    window.draw(friendsText);
    window.draw(pendingText);
    window.draw(helperText);
    window.draw(statusText);
}
