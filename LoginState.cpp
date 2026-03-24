#include "LoginState.h"
#include "Game.h"
#include "MainMenuState.h"
#include "GameState.h"
#include "GameRoomState.h"
#include "UserDataIO.h"
#include "GameLogic.h"
#include "UIStyle.h"

void LoginState::showError(const std::string& msg)
{
    errorText.setString(msg);
}

LoginState::LoginState(Game& game, GameData& data, LoginPurpose purpose)
    : State(game, data), purpose(purpose), animTime(0.f), enteringUsername(true)
{

    loadUsersFromFile(this->data);
    data.friends.loadFromFile("friends.txt", data.users, data.userCount);

    title.setFont(ui::headingFont());
    title.setCharacterSize(30);
    if (purpose == LOGIN_FOR_MAIN)
        title.setString("Login");
    else if (purpose == LOGIN_FOR_COOP_EXTRA)
        title.setString("Login Extra Player");
    else
        title.setString("Queue Player Login");
    ui::centerText(title, sf::FloatRect(0.f, 34.f, static_cast<float>(N * ts), 38.f));

    subtitle.setFont(ui::bodyFont());
    subtitle.setCharacterSize(16);
    if (purpose == LOGIN_FOR_MAIN)
        subtitle.setString("Press CTRL+R to register a new account");
    else if (purpose == LOGIN_FOR_COOP_EXTRA)
        subtitle.setString("Use a different account for Player 2");
    else
        subtitle.setString("Authenticate a player before adding them to queue");
    ui::centerText(subtitle, sf::FloatRect(0.f, 74.f, static_cast<float>(N * ts), 24.f));

    usernameLabel.setFont(ui::bodyFont());
    usernameLabel.setCharacterSize(18);
    usernameLabel.setString("Username");
    usernameLabel.setPosition(170.f, 146.f);

    usernameText.setFont(ui::bodyFont());
    usernameText.setCharacterSize(20);
    usernameText.setPosition(185.f, 180.f);

    passwordLabel.setFont(ui::bodyFont());
    passwordLabel.setCharacterSize(18);
    passwordLabel.setString("Password");
    passwordLabel.setPosition(170.f, 236.f);

    passwordText.setFont(ui::bodyFont());
    passwordText.setCharacterSize(20);
    passwordText.setPosition(185.f, 270.f);

    helper.setFont(ui::bodyFont());
    helper.setCharacterSize(16);
    helper.setString("ENTER = login | TAB = switch field | ESC = back");
    helper.setPosition(170.f, 336.f);

    errorText.setFont(ui::bodyFont());
    errorText.setCharacterSize(15);
    errorText.setPosition(170.f, 366.f);

    usernameInput.clear();
    passwordInput.clear();
    rebuildTexts();
}

void LoginState::rebuildTexts()
{
    usernameText.setString(usernameInput.empty() ? std::string("Type your username...") : usernameInput);
    std::string masked(passwordInput.size(), '*');
    if (masked.empty())
        masked = "Type your password...";
    passwordText.setString(masked);
}

void LoginState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::TextEntered)
    {
        if (event.text.unicode >= 32 && event.text.unicode < 127)
        {
            char c = static_cast<char>(event.text.unicode);
            if (enteringUsername)
                usernameInput.push_back(c);
            else
                passwordInput.push_back(c);
            rebuildTexts();
        }
        return;
    }

    if (event.type != sf::Event::KeyPressed)
        return;

    if (event.key.code == sf::Keyboard::Tab)
        enteringUsername = !enteringUsername;
    else if (event.key.code == sf::Keyboard::Backspace)
    {
        std::string& target = enteringUsername ? usernameInput : passwordInput;
        if (!target.empty())
            target.pop_back();
        rebuildTexts();
    }
    else if (event.key.code == sf::Keyboard::Enter)
        tryLogin();
    else if (event.key.code == sf::Keyboard::R && event.key.control)
        tryRegister();
    else if (event.key.code == sf::Keyboard::Escape)
    {
        if (data.sessionPlayer || data.currentPlayer)
            game.changeState(new MainMenuState(game, data));
        else
            game.getWindow().close();
    }
}

void LoginState::update(float dt)
{
    animTime += dt;
}

void LoginState::render(sf::RenderWindow& window)
{
    ui::Palette palette = ui::getPalette(data.inventory ? data.inventory->getCurrentThemeID() : 1);
    ui::drawBackdrop(window, palette, animTime);
    ui::drawPanel(window, sf::FloatRect(140.f, 24.f, 440.f, 398.f), palette, true);
    ui::drawHeader(window, title, &subtitle, palette);

    usernameLabel.setFillColor(palette.textSecondary);
    passwordLabel.setFillColor(palette.textSecondary);
    helper.setFillColor(palette.textSecondary);
    errorText.setFillColor(errorText.getString().isEmpty() ? palette.textSecondary : palette.danger);

    sf::RectangleShape inputBox(sf::Vector2f(370.f, 42.f));
    inputBox.setPosition(170.f, 172.f);
    inputBox.setFillColor(ui::withAlpha(palette.buttonFill, 220));
    inputBox.setOutlineThickness(2.f);
    inputBox.setOutlineColor(enteringUsername ? palette.accent : palette.panelOutline);
    window.draw(inputBox);

    inputBox.setPosition(170.f, 262.f);
    inputBox.setOutlineColor(!enteringUsername ? palette.accent : palette.panelOutline);
    window.draw(inputBox);

    usernameText.setFillColor(usernameInput.empty() ? palette.textSecondary : palette.textPrimary);
    passwordText.setFillColor(passwordInput.empty() ? palette.textSecondary : palette.textPrimary);

    window.draw(usernameLabel);
    window.draw(usernameText);
    window.draw(passwordLabel);
    window.draw(passwordText);
    window.draw(helper);
    window.draw(errorText);
}

void LoginState::tryLogin()
{
    if (usernameInput.empty() || passwordInput.empty())
    {
        showError("Please enter both username and password.");
        return;
    }

    int idx = findUserIndex(data, usernameInput, passwordInput);
    if (idx < 0)
    {
        showError("User not found or wrong password.");
        return;
    }

    Player* p = &data.users[idx];

    if (purpose == LOGIN_FOR_MAIN)
    {
        data.sessionPlayer = p;
        data.currentPlayer = p;
        data.coopPlayer = nullptr;
        data.playerCount = 1;
        if (data.inventory)
            data.inventory->loadThemePreference(p->username);
        game.changeState(new MainMenuState(game, data));
    }
    else if (purpose == LOGIN_FOR_COOP_EXTRA)
    {
        if (p == data.currentPlayer)
        {
            showError("Second player must be different from Player 1.");
            return;
        }
        data.coopPlayer = p;
        data.playerCount = 2;
        game.changeState(new GameState(game, data, 2));
    }
    else
    {
        if (data.matchQueue.containsPlayer(p))
        {
            showError("That player is already in the queue.");
            return;
        }
        data.matchQueue.push(p, p->totalPoints);
        game.changeState(new GameRoomState(game, data));
    }
}

void LoginState::tryRegister()
{
    if (usernameInput.empty() || passwordInput.empty())
    {
        showError("Username and password cannot be empty.");
        return;
    }
    if (usernameInput.size() < 3)
    {
        showError("Username too short (min 3).");
        return;
    }
    if (passwordInput.size() < 4)
    {
        showError("Password too short (min 4).");
        return;
    }
    if (data.userCount >= MAX_PLAYERS)
    {
        showError("User limit reached.");
        return;
    }
    if (findUserIndexByName(data, usernameInput) >= 0)
    {
        showError("Username already taken.");
        return;
    }

    Player& pl = data.users[data.userCount++];
    pl = Player();
    pl.username = usernameInput;
    pl.password = passwordInput;

    data.friends.rebuildHash(data.users, data.userCount);
    saveUsersToFile(data);
    data.friends.saveToFile("friends.txt");
    showError("Registered successfully. Press ENTER to login.");
}
