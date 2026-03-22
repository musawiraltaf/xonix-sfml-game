#include "Game.h"
#include "LoginState.h"
#include "GameLogic.h"

Game::Game()
    : window(sf::VideoMode(N* ts, M* ts), "Xonix (State System)"),
    currentState(nullptr)
{
    window.setFramerateLimit(60);

    // Fix the logical view to match the grid size (720x450)
    sf::View view(sf::FloatRect(
        0.f, 0.f,
        static_cast<float>(N * ts),
        static_cast<float>(M * ts)
    ));
    window.setView(view);

    data.userCount = 0;
    data.playerCount = 0;
    data.currentPlayer = nullptr;
    data.coopPlayer = nullptr;
    data.highScore = 0;

    // 🔹 (optional) if you have a function that loads all users into data.users:
    // loadUsersFromFile(data);  // or whatever name/signature you used

    // 🔹 Load previous top-10 from file (if it exists)
    data.leaderboard.loadFromFile("leaderboard.txt");
    data.roomPlayerTarget = 0;
    data.roomPlayerCount = 0;
    for (int i = 0; i < 8; ++i)
        data.roomPlayers[i] = nullptr;
    data.matchmakingActive = false;
    data.lastWinner = nullptr;

}




sf::RenderWindow& Game::getWindow()
{
    return window;
}


Game::~Game()
{
    if (currentState)
        delete currentState;
}

void Game::changeState(State* newState)
{
    if (currentState)
        delete currentState;
    currentState = newState;
}

void Game::run()
{
    // start in LoginState
    changeState(new LoginState(*this, data, LOGIN_FOR_MAIN));

    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized)
            {
                // keep the same logical coords even when maximized
                sf::View view(sf::FloatRect(0.f, 0.f,
                    static_cast<float>(N * ts),
                    static_cast<float>(M * ts)));
                window.setView(view);
            }

            if (currentState)
                currentState->handleEvent(event);
        }

        if (currentState)
            currentState->update(dt);

        window.clear(sf::Color::Black);

        if (currentState)
            currentState->render(window);

        window.display();
    }
}
