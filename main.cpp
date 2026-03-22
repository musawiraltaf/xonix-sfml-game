
#include <cstdlib>
#include <ctime>
#include "Game.h"

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr))); // seed RNG once

    Game game;
    game.run();
    return 0;
}


//#include <SFML/Graphics.hpp>
//#include <time.h>
//#include <fstream>
//#include <iostream>
//#include <string>
//#include "Player.h"  
//#include "Enemy.h" 
//#include "Auth.h"
//#include "GameLogic.h"
//
//using namespace sf;
//
//
//
//
//const std::string highScoreFile = "highscore.txt"; 
//
//const std::string usersFile = "users.txt";
//const int MAX_USERS = 100;
//
//
//
//
//enum ScreenState {
//    LOGIN_SCREEN,
//    MAIN_MENU,
//    GAME_RUNNING,
//    GAME_OVER_SCREEN
//};
//
//
//int loadHighScore(const std::string& filename)
//{
//    std::ifstream in(filename);
//    int hs = 0;
//    if (in >> hs) return hs;
//    return 0;
//}
//
//void saveHighScore(const std::string& filename, int hs)
//{
//    std::ofstream out(filename);
//    if (out) out << hs;
//}
//
//
//
//int main()
//{
//    srand(time(0));
//
//    // --- load users and login/register ---
//    PlayerArray users;
//    loadUsers(usersFile, users);
//    Player player;
//
//    int highScore = 0;
//    {
//        std::ifstream in("highscore.txt");
//        if (in)
//        {
//            in >> highScore;
//        }
//    }
//
//    RenderWindow window(VideoMode(N * ts, M * ts), "Xonix Game!");
//
//    window.setFramerateLimit(60);
//    ScreenState currentScreen = LOGIN_SCREEN;
//
//    Texture t1, t2, t3;
//    t1.loadFromFile("images/tiles.png");
//    t2.loadFromFile("images/gameover.png");
//    t3.loadFromFile("images/enemy.png");
//
//    Font font;
//    if (!font.loadFromFile("ariblk.ttf"))
//    {
//        return -1; // font not found
//    }
//    // For login screen
//    std::string usernameInput = "";
//    std::string passwordInput = "";
//    bool enteringUsername = true;      // toggle between username & password
//    bool showLoginError = false;
//    std::string loginErrorMessage = "";
//
//    Text loginTitle;
//    loginTitle.setFont(font);
//    loginTitle.setCharacterSize(30);
//    loginTitle.setFillColor(Color::White);
//    loginTitle.setPosition(60, 40);
//    loginTitle.setString("XONIX LOGIN");
//
//    Text usernameLabel;
//    usernameLabel.setFont(font);
//    usernameLabel.setCharacterSize(20);
//    usernameLabel.setFillColor(Color::White);
//    usernameLabel.setPosition(60, 120);
//    usernameLabel.setString("Username:");
//
//    Text passwordLabel;
//    passwordLabel.setFont(font);
//    passwordLabel.setCharacterSize(20);
//    passwordLabel.setFillColor(Color::White);
//    passwordLabel.setPosition(60, 180);
//    passwordLabel.setString("Password:");
//
//    Text usernameText;
//    usernameText.setFont(font);
//    usernameText.setCharacterSize(20);
//    usernameText.setFillColor(Color::Yellow);
//    usernameText.setPosition(200, 120);
//
//    Text passwordText;
//    passwordText.setFont(font);
//    passwordText.setCharacterSize(20);
//    passwordText.setFillColor(Color::Yellow);
//    passwordText.setPosition(200, 180);
//
//    Text helperText;
//    helperText.setFont(font);
//    helperText.setCharacterSize(18);
//    helperText.setFillColor(Color::White);
//    helperText.setPosition(60, 240);
//    helperText.setString("Enter username, press TAB for password.\nPress ENTER to Login, R to Register.");
//
//    Text errorText;
//    errorText.setFont(font);
//    errorText.setCharacterSize(18);
//    errorText.setFillColor(Color::Red);
//    errorText.setPosition(60, 300);
//
//
//    Text scoreText;
//    scoreText.setFont(font);
//    scoreText.setCharacterSize(20);
//    scoreText.setFillColor(Color::White);
//    scoreText.setPosition(10, 10);
//
//    Text mainMenuText;
//    mainMenuText.setFont(font);
//    mainMenuText.setCharacterSize(24);
//    mainMenuText.setFillColor(Color::White);
//    mainMenuText.setPosition(60, 60);
//
//    Text gameOverText;
//    gameOverText.setFont(font);
//    gameOverText.setCharacterSize(24);
//    gameOverText.setFillColor(Color::White);
//    gameOverText.setPosition(120, 220);
//
//    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
//    sGameover.setPosition(100, 80);
//    sEnemy.setOrigin(20, 20);
//
//    int enemyCount = 4;
//    Enemy a[10];
//
//    
//
//    bool Game = true;
//    int gameMode = 1; // 1 = single player, 2 = multiplayer (co-op)
//
//    // Player 1
//    int x1 = 10, y1 = 0, dx1 = 0, dy1 = 0;
//    // Player 2
//    int x2 = N - 11, y2 = 0, dx2 = 0, dy2 = 0;
//
//    int tilesCapturedThisMove = 0;
//
//    float timer = 0.f, delay = 0.07f;
//    Clock clock;
//
//    bool freezeActive = false;
//    float freezeTimer = 0.0f;
//
//    // Initialize grid boundaries
//    for (int i = 0; i < M; i++)
//        for (int j = 0; j < N; j++)
//            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
//                grid[i][j] = 1;
//
//    while (window.isOpen())
//    {
//        float time = clock.getElapsedTime().asSeconds();
//        clock.restart();
//        timer += time;
//
//        if (freezeActive)
//        {
//            freezeTimer -= time;
//            if (freezeTimer <= 0)
//                freezeActive = false;
//        }
//
//        Event e;
//        while (window.pollEvent(e))
//        {
//            if (e.type == Event::Closed)
//                window.close();
//
//            // ======================= LOGIN SCREEN INPUT =======================
//            if (currentScreen == LOGIN_SCREEN)
//            {
//                // Handle text typing
//                if (e.type == Event::TextEntered)
//                {
//                    if (e.text.unicode == 8)  // backspace
//                    {
//                        std::string& target = enteringUsername ? usernameInput : passwordInput;
//                        if (!target.empty())
//                            target.pop_back();
//                    }
//                    else if (e.text.unicode >= 32 && e.text.unicode < 127)
//                    {
//                        char c = static_cast<char>(e.text.unicode);
//                        std::string& target = enteringUsername ? usernameInput : passwordInput;
//                        if (target.size() < 20)
//                            target.push_back(c);
//                    }
//                }
//
//                if (e.type == Event::KeyPressed)
//                {
//                    if (e.key.code == Keyboard::Tab)
//                    {
//                        enteringUsername = !enteringUsername;
//                    }
//                    else if (e.key.code == Keyboard::Enter)
//                    {
//                        int idx = findUserIndex(users, usernameInput);
//
//                        if (idx == -1)
//                        {
//                            showLoginError = true;
//                            loginErrorMessage = "User not found. Press R to Register.";
//                        }
//                        else if (users[idx].password != passwordInput)
//                        {
//                            showLoginError = true;
//                            loginErrorMessage = "Wrong password.";
//                        }
//                        else
//                        {
//                            player = users[idx];
//                            player.resetForNewGame();
//                            showLoginError = false;
//                            currentScreen = MAIN_MENU;
//                        }
//                    }
//                    else if (e.key.code == Keyboard::R)
//                    {
//                        if (usernameInput.empty() || passwordInput.empty())
//                        {
//                            showLoginError = true;
//                            loginErrorMessage = "Enter username and password first.";
//                        }
//                        else if (findUserIndex(users, usernameInput) != -1)
//                        {
//                            showLoginError = true;
//                            loginErrorMessage = "Username already exists.";
//                        }
//                        else
//                        {
//                            Player p;
//                            p.username = usernameInput;
//                            p.password = passwordInput;
//                            p.totalPoints = 0;
//                            p.wins = 0;
//                            p.losses = 0;
//                            p.resetForNewGame();
//                            users.pushBack(p);
//                            player = p;
//
//                            saveUsers(usersFile, users);
//
//
//                            showLoginError = false;
//                            currentScreen = MAIN_MENU;
//                        }
//                    }
//                }
//
//                // update displayed text
//                usernameText.setString(usernameInput);
//                passwordText.setString(std::string(passwordInput.size(), '*'));
//                if (showLoginError)
//                    errorText.setString(loginErrorMessage);
//                else
//                    errorText.setString("");
//
//                continue; // prevent game logic from running
//            }
//
//            if (e.type == Event::KeyPressed)
//            {
//                // MAIN MENU INPUT
//                if (currentScreen == MAIN_MENU)
//                {
//                    if (e.key.code == Keyboard::Num1 || e.key.code == Keyboard::Numpad1)
//                    {
//                        gameMode = 1; // single player
//
//                        for (int i = 1; i < M - 1; i++)
//                            for (int j = 1; j < N - 1; j++)
//                                grid[i][j] = 0;
//
//                        x1 = 10;  y1 = 0;  dx1 = dy1 = 0;
//                        x2 = N - 11; y2 = 0; dx2 = dy2 = 0; // ignored in single
//
//                        player.resetForNewGame();
//                        Game = true;
//                        freezeActive = false;
//                        freezeTimer = 0.0f;
//                        for (int i = 0; i < enemyCount; i++) a[i] = Enemy();
//
//                        currentScreen = GAME_RUNNING;
//                    }
//                    else if (e.key.code == Keyboard::Num2 || e.key.code == Keyboard::Numpad2)
//                    {
//                        gameMode = 2; // multiplayer co-op
//
//                        for (int i = 1; i < M - 1; i++)
//                            for (int j = 1; j < N - 1; j++)
//                                grid[i][j] = 0;
//
//                        x1 = 10;      y1 = 0;  dx1 = dy1 = 0;
//                        x2 = N - 11;  y2 = 0;  dx2 = dy2 = 0;
//
//                        player.resetForNewGame();
//                        Game = true;
//                        freezeActive = false;
//                        freezeTimer = 0.0f;
//                        for (int i = 0; i < enemyCount; i++) a[i] = Enemy();
//
//                        currentScreen = GAME_RUNNING;
//                    }
//                    else if (e.key.code == Keyboard::Escape)
//                    {
//                        window.close();
//                    }
//                }
//                // GAME RUNNING INPUT
//                else if (currentScreen == GAME_RUNNING)
//                {
//                    if (e.key.code == Keyboard::Escape)
//                    {
//                        // restart current mode
//                        for (int i = 1; i < M - 1; i++)
//                            for (int j = 1; j < N - 1; j++)
//                                grid[i][j] = 0;
//
//                        if (gameMode == 1)
//                        {
//                            x1 = 10; y1 = 0; dx1 = dy1 = 0;
//                        }
//                        else
//                        {
//                            x1 = 10;      y1 = 0;  dx1 = dy1 = 0;
//                            x2 = N - 11;  y2 = 0;  dx2 = dy2 = 0;
//                        }
//
//                        player.resetForNewGame();
//                        Game = true;
//                        freezeActive = false;
//                        freezeTimer = 0.0f;
//                        for (int i = 0; i < enemyCount; i++) a[i] = Enemy();
//                    }
//
//                    // Activate power-up: freeze for 3 seconds
//                    if (e.key.code == Keyboard::Space)
//                    {
//                        if (player.powerUps > 0 && !freezeActive)
//                        {
//                            freezeActive = true;
//                            freezeTimer = 3.0f;
//                            player.powerUps--;
//                        }
//                    }
//                }
//                // GAME OVER INPUT
//                else if (currentScreen == GAME_OVER_SCREEN)
//                {
//                    if (e.key.code == Keyboard::R)
//                    {
//                        for (int i = 1; i < M - 1; i++)
//                            for (int j = 1; j < N - 1; j++)
//                                grid[i][j] = 0;
//
//                        if (gameMode == 1)
//                        {
//                            x1 = 10; y1 = 0; dx1 = dy1 = 0;
//                        }
//                        else
//                        {
//                            x1 = 10;      y1 = 0;  dx1 = dy1 = 0;
//                            x2 = N - 11;  y2 = 0;  dx2 = dy2 = 0;
//                        }
//
//                        player.resetForNewGame();
//                        Game = true;
//                        freezeActive = false;
//                        freezeTimer = 0.0f;
//                        for (int i = 0; i < enemyCount; i++) a[i] = Enemy();
//
//                        currentScreen = GAME_RUNNING;
//                    }
//                    else if (e.key.code == Keyboard::M)
//                    {
//                        currentScreen = MAIN_MENU;
//                    }
//                    else if (e.key.code == Keyboard::Escape)
//                    {
//                        window.close();
//                    }
//                }
//            }
//        }
//
//        // MAIN MENU DRAW
//        // ======================= LOGIN SCREEN DRAW =======================
//        if (currentScreen == LOGIN_SCREEN)
//        {
//            window.clear();
//
//            window.draw(loginTitle);
//            window.draw(usernameLabel);
//            window.draw(passwordLabel);
//            window.draw(usernameText);
//            window.draw(passwordText);
//            window.draw(helperText);
//
//            if (showLoginError)
//                window.draw(errorText);
//
//            window.display();
//            continue;
//        }
//
//        if (currentScreen == MAIN_MENU)
//        {
//            mainMenuText.setString(
//                "XONIX GAME\n\n"
//                "High Score: " + std::to_string(highScore) + "\n\n"
//                "Press 1 - Single Player\n"
//                "Press 2 - Multiplayer (Co-Op)\n\n"
//                "Press ESC to Exit"
//            );
//
//            window.clear();
//            window.draw(mainMenuText);
//            window.display();
//            continue;
//        }
//
//        // GAME LOGIC WHEN RUNNING
//        if (currentScreen == GAME_RUNNING)
//        {
//            // Player 1 (Arrow keys)
//            if (Keyboard::isKeyPressed(Keyboard::Left)) { dx1 = -1; dy1 = 0; }
//            if (Keyboard::isKeyPressed(Keyboard::Right)) { dx1 = 1; dy1 = 0; }
//            if (Keyboard::isKeyPressed(Keyboard::Up)) { dx1 = 0; dy1 = -1; }
//            if (Keyboard::isKeyPressed(Keyboard::Down)) { dx1 = 0; dy1 = 1; }
//
//            // Player 2 (WASD) only in multiplayer
//            if (gameMode == 2)
//            {
//                if (Keyboard::isKeyPressed(Keyboard::A)) { dx2 = -1; dy2 = 0; }
//                if (Keyboard::isKeyPressed(Keyboard::D)) { dx2 = 1; dy2 = 0; }
//                if (Keyboard::isKeyPressed(Keyboard::W)) { dx2 = 0; dy2 = -1; }
//                if (Keyboard::isKeyPressed(Keyboard::S)) { dx2 = 0; dy2 = 1; }
//            }
//
//            if (Game && timer > delay)
//            {
//                // MOVE PLAYER 1
//                x1 += dx1;
//                y1 += dy1;
//
//                if (x1 < 0) x1 = 0;
//                if (x1 > N - 1) x1 = N - 1;
//                if (y1 < 0) y1 = 0;
//                if (y1 > M - 1) y1 = M - 1;
//
//                if (grid[y1][x1] == 2)
//                {
//                    Game = false;
//                    currentScreen = GAME_OVER_SCREEN;
//                    if (player.score > highScore)
//                    {
//                        highScore = player.score;
//                        saveHighScore(highScoreFile, highScore);
//                    }
//                    player.totalPoints += player.score;
//                    updateUserInArray(users, player);
//                    saveUsers(usersFile, users);
//
//                }
//
//                if (grid[y1][x1] == 0) grid[y1][x1] = 2;
//
//                // MOVE PLAYER 2 (if multiplayer)
//                if (gameMode == 2)
//                {
//                    x2 += dx2;
//                    y2 += dy2;
//
//                    if (x2 < 0) x2 = 0;
//                    if (x2 > N - 1) x2 = N - 1;
//                    if (y2 < 0) y2 = 0;
//                    if (y2 > M - 1) y2 = M - 1;
//
//                    if (grid[y2][x2] == 2)
//                    {
//                        Game = false;
//                        currentScreen = GAME_OVER_SCREEN;
//                        if (player.score > highScore)
//                        {
//                            highScore = player.score;
//                            saveHighScore(highScoreFile, highScore);
//                        }
//                        player.totalPoints += player.score;
//                        updateUserInArray(users, player);
//                        saveUsers(usersFile, users);
//
//                    }
//
//                    if (grid[y2][x2] == 0) grid[y2][x2] = 2;
//                }
//
//                timer = 0;
//            }
//
//            // move enemies only when not frozen
//            if (Game && !freezeActive)
//            {
//                for (int i = 0; i < enemyCount; i++)
//                    a[i].move();
//            }
//
//            // region closure when any player hits filled area
//            if (Game && (grid[y1][x1] == 1 || (gameMode == 2 && grid[y2][x2] == 1)))
//            {
//                dx1 = dy1 = 0;
//                dx2 = dy2 = 0;
//
//                int beforeCount = 0;
//                for (int i = 0; i < M; i++)
//                    for (int j = 0; j < N; j++)
//                        if (grid[i][j] == 1)
//                            beforeCount++;
//
//                // flood-fill from each enemy
//                for (int i = 0; i < enemyCount; i++)
//                    drop(a[i].y / ts, a[i].x / ts);
//
//                // fill everything else
//                for (int i = 0; i < M; i++)
//                    for (int j = 0; j < N; j++)
//                    {
//                        if (grid[i][j] == -1)
//                            grid[i][j] = 0;
//                        else
//                            grid[i][j] = 1;
//                    }
//
//                int afterCount = 0;
//                for (int i = 0; i < M; i++)
//                    for (int j = 0; j < N; j++)
//                        if (grid[i][j] == 1)
//                            afterCount++;
//
//                tilesCapturedThisMove = afterCount - beforeCount;
//                if (tilesCapturedThisMove > 0)
//                    player.updateScore(tilesCapturedThisMove);
//            }
//
//            // enemies hitting trail
//            if (Game)
//            {
//                for (int i = 0; i < enemyCount; i++)
//                {
//                    if (grid[a[i].y / ts][a[i].x / ts] == 2)
//                    {
//                        Game = false;
//                        currentScreen = GAME_OVER_SCREEN;
//                        if (player.score > highScore)
//                        {
//                            highScore = player.score;
//                            saveHighScore(highScoreFile, highScore);
//                        }
//
//                        player.totalPoints += player.score;
//                        updateUserInArray(users, player);
//                        saveUsers(usersFile, users);
//                    }
//                }
//            }
//        }
//
//        // GAME OVER TEXT
//        if (currentScreen == GAME_OVER_SCREEN)
//        {
//            gameOverText.setString(
//                "GAME OVER\n\n"
//                "Score: " + std::to_string(player.score) + "\n" +
//                "High Score: " + std::to_string(highScore) + "\n\n"
//                "R - Restart\n"
//                "M - Main Menu\n"
//                "ESC - Exit"
//            );
//        }
//
//        scoreText.setString("Score: " + std::to_string(player.score));
//
//        // DRAW
//        window.clear();
//
//        // draw grid
//        for (int i = 0; i < M; i++)
//            for (int j = 0; j < N; j++)
//            {
//                if (grid[i][j] == 0) continue;
//                if (grid[i][j] == 1) sTile.setTextureRect(IntRect(0, 0, ts, ts));
//                if (grid[i][j] == 2) sTile.setTextureRect(IntRect(54, 0, ts, ts));
//                sTile.setPosition(j * ts, i * ts);
//                sTile.setColor(Color::White);
//                window.draw(sTile);
//            }
//
//        // draw player 1
//        sTile.setTextureRect(IntRect(36, 0, ts, ts));
//        sTile.setColor(Color::Yellow);
//        sTile.setPosition(x1 * ts, y1 * ts);
//        window.draw(sTile);
//
//        // draw player 2 (if multiplayer)
//        if (gameMode == 2)
//        {
//            sTile.setColor(Color::Cyan);
//            sTile.setPosition(x2 * ts, y2 * ts);
//            window.draw(sTile);
//        }
//
//        // draw enemies
//        sEnemy.rotate(10);
//        for (int i = 0; i < enemyCount; i++)
//        {
//            sEnemy.setPosition(a[i].x, a[i].y);
//            window.draw(sEnemy);
//        }
//
//        if (!Game && currentScreen == GAME_OVER_SCREEN)
//            window.draw(sGameover);
//
//        window.draw(scoreText);
//
//        if (currentScreen == GAME_RUNNING)
//        {
//            Text freezeText;
//            freezeText.setFont(font);
//            freezeText.setCharacterSize(18);
//            freezeText.setFillColor(Color::Cyan);
//            freezeText.setPosition(10, 40);
//
//            if (freezeActive)
//                freezeText.setString("Freeze: ACTIVE");
//            else
//                freezeText.setString("Freeze Ready: " + std::to_string(player.powerUps));
//
//            window.draw(freezeText);
//
//            Text modeText;
//            modeText.setFont(font);
//            modeText.setCharacterSize(18);
//            modeText.setFillColor(Color::White);
//            modeText.setPosition(10, 70);
//            modeText.setString(gameMode == 1 ? "Mode: Single Player" : "Mode: Co-Op Multiplayer");
//            window.draw(modeText);
//        }
//
//        if (currentScreen == GAME_OVER_SCREEN)
//        {
//            window.draw(gameOverText);
//        }
//
//        window.display();
//    }
//
//    return 0;
//}