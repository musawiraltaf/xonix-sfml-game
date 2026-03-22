#include "LoginState.h"
#include "Game.h"
#include "MainMenuState.h"
#include "GameState.h"
#include "GameRoomState.h"

#include <fstream>
#include <sstream>

static const char* USER_FILE = "users.txt";

// Helper: load users from file into data.users[]
static void loadUsers(GameData& data)
{
    data.userCount = 0;

    std::ifstream in(USER_FILE);
    if (!in)
        return;

    std::string u, p;
    int hs = 0;

    while (in >> u >> p >> hs)
    {
        if (data.userCount >= MAX_PLAYERS)
            break;

        Player& pl = data.users[data.userCount++];
        pl.username = u;
        pl.password = p;
        pl.highScore = hs;

        // Optional safety initialisation (if not done elsewhere):
        // pl.score        = 0;
        // pl.totalPoints  = 0;
        // pl.wins = pl.losses = 0;
        // pl.powerUps     = 0;
        // pl.bonusThreshold = 10;
        // pl.bonusCount     = 0;
    }
}

// Helper: save users back to file
static void saveUsers(const GameData& data)
{
    std::ofstream out(USER_FILE);
    if (!out)
        return;

    for (int i = 0; i < data.userCount; ++i)
    {
        const Player& pl = data.users[i];
        out << pl.username << " " << pl.password << " " << pl.highScore << "\n";
    }
}

// Helper: find existing user by username+password
static int findUser(const GameData& data,
    const std::string& username,
    const std::string& password)
{
    for (int i = 0; i < data.userCount; ++i)
    {
        if (data.users[i].username == username &&
            data.users[i].password == password)
        {
            return i;
        }
    }
    return -1;
}

// Helper: find user by username only
static int findUserByName(const GameData& data,
    const std::string& username)
{
    for (int i = 0; i < data.userCount; ++i)
    {
        if (data.users[i].username == username)
            return i;
    }
    return -1;
}

// ----------------- LoginState methods -----------------

LoginState::LoginState(Game& game, GameData& data, LoginPurpose purpose)
    : State(game, data)
    , purpose(purpose)
    , enteringUsername(true)
{
    font.loadFromFile("ariblk.ttf");

    // Make sure we have users loaded
    loadUsers(this->data);

    // -------- Title --------
    title.setFont(font);
    title.setCharacterSize(32);
    title.setFillColor(sf::Color::White);

    if (purpose == LOGIN_FOR_MAIN)
    {
        title.setString("Login");
    }
    else if (purpose == LOGIN_FOR_COOP_EXTRA)
    {
        title.setString("Login Extra Player");
    }
    else // LOGIN_FOR_ROOM
    {
        title.setString("Login Matchmaking Player");
    }
    title.setPosition(200.f, 50.f);

    // Username label
    usernameLabel.setFont(font);
    usernameLabel.setCharacterSize(20);
    usernameLabel.setFillColor(sf::Color::White);
    usernameLabel.setString("Username:");
    usernameLabel.setPosition(150.f, 150.f);

    usernameText.setFont(font);
    usernameText.setCharacterSize(20);
    usernameText.setFillColor(sf::Color::Yellow);
    usernameText.setPosition(300.f, 150.f);

    // Password label
    passwordLabel.setFont(font);
    passwordLabel.setCharacterSize(20);
    passwordLabel.setFillColor(sf::Color::White);
    passwordLabel.setString("Password:");
    passwordLabel.setPosition(150.f, 200.f);

    passwordText.setFont(font);
    passwordText.setCharacterSize(20);
    passwordText.setFillColor(sf::Color::Yellow);
    passwordText.setPosition(300.f, 200.f);

    // Helper text
    helper.setFont(font);
    helper.setCharacterSize(16);
    helper.setFillColor(sf::Color(200, 200, 200));
    helper.setString("ENTER = Login | R = Register | TAB = Switch field");
    helper.setPosition(150.f, 260.f);

    // Error text
    errorText.setFont(font);
    errorText.setCharacterSize(16);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(150.f, 300.f);

    usernameInput.clear();
    passwordInput.clear();
    rebuildTexts();
}

void LoginState::rebuildTexts()
{
    usernameText.setString(usernameInput);

    // Mask password with '*'
    std::string masked(passwordInput.size(), '*');
    passwordText.setString(masked);
}

void LoginState::showError(const std::string& msg)
{
    errorText.setString(msg);
}

void LoginState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::TextEntered)
    {
        // Only handle printable ASCII
        if (event.text.unicode >= 32 && event.text.unicode < 127)
        {
            char c = static_cast<char>(event.text.unicode);
            if (enteringUsername)
                usernameInput.push_back(c);
            else
                passwordInput.push_back(c);

            rebuildTexts();
        }
    }
    else if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Tab)
        {
            enteringUsername = !enteringUsername;
        }
        else if (event.key.code == sf::Keyboard::Backspace)
        {
            std::string& target = (enteringUsername ? usernameInput : passwordInput);
            if (!target.empty())
            {
                target.pop_back();
                rebuildTexts();
            }
        }
        else if (event.key.code == sf::Keyboard::Enter)
        {
            tryLogin();
        }
        else if (event.key.code == sf::Keyboard::R)
        {
            tryRegister();
        }
        else if (event.key.code == sf::Keyboard::Escape)
        {
            // Cancel back to main menu (only makes sense if main player already logged)
            if (data.currentPlayer)
            {
                game.changeState(new MainMenuState(game, data));
            }
            else
            {
                // no main player yet -> exit game
                game.getWindow().close();
            }
        }
    }
}

void LoginState::update(float /*dt*/)
{
    // no animations
}

void LoginState::render(sf::RenderWindow& window)
{
    window.draw(title);
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

    int idx = findUser(data, usernameInput, passwordInput);
    if (idx < 0)
    {
        showError("User not found or wrong password.");
        return;
    }

    Player* p = &data.users[idx];

    if (purpose == LOGIN_FOR_MAIN)
    {
        // Normal main-player login at game start
        data.currentPlayer = p;
        data.coopPlayer = nullptr;
        data.playerCount = 1;

        // load saved theme for this user
        data.inventory.loadThemePreference(p->username);
        // After main login, go to main menu
        game.changeState(new MainMenuState(game, data));
    }
    else if (purpose == LOGIN_FOR_COOP_EXTRA)
    {
        // Logging in extra player for local 2-player
        if (p == data.currentPlayer)
        {
            showError("Second player must be different from Player 1.");
            return;
        }

        data.coopPlayer = p;
        data.playerCount = 2;

        // After second player logs in, start 2-player game directly
        game.changeState(new GameState(game, data, 2));
    }
    else if (purpose == LOGIN_FOR_ROOM)
    {
        // --- Matchmaking: each login == one player joining the queue ---

        // Push this player into the priority queue.
        // Priority is based on totalPoints (higher = higher priority).
        data.matchQueue.push(p, p->totalPoints);

        // Return to the matchmaking room; it will rebuild the queue display.
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

    if (findUserByName(data, usernameInput) >= 0)
    {
        showError("Username already taken.");
        return;
    }

    Player& pl = data.users[data.userCount++];
    pl.username = usernameInput;
    pl.password = passwordInput;
    pl.highScore = 0;

    saveUsers(data);
    showError("Registered successfully. Press ENTER to login.");
}
