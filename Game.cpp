#include "Game.h"
#include "LoginState.h"
#include "GameLogic.h"
#include "UIStyle.h"

Game::Game()
    : window(sf::VideoMode(N * ts, M * ts), "Xonix (State System)"),
      currentState(nullptr), pendingState(nullptr)
{
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    sf::View view(sf::FloatRect(0.f, 0.f, static_cast<float>(N * ts), static_cast<float>(M * ts)));
    window.setView(view);

    data.inventory = new InventoryManager();
    data.leaderboard.loadFromFile("leaderboard.txt");
}

Game::~Game()
{
    if (pendingState)
        delete pendingState;
    if (currentState)
        delete currentState;
    delete data.inventory;
    data.inventory = nullptr;
}

sf::RenderWindow& Game::getWindow()
{
    return window;
}

void Game::changeState(State* newState)
{
    if (pendingState)
        delete pendingState;
    pendingState = newState;
}

void Game::applyPendingStateChange()
{
    if (!pendingState)
        return;

    if (currentState)
        delete currentState;
    currentState = pendingState;
    pendingState = nullptr;
}

void Game::run()
{
    changeState(new LoginState(*this, data, LOGIN_FOR_MAIN));
    applyPendingStateChange();

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
                sf::View view(sf::FloatRect(0.f, 0.f,
                    static_cast<float>(N * ts),
                    static_cast<float>(M * ts)));
                window.setView(view);
            }

            if (currentState)
                currentState->handleEvent(event);

            applyPendingStateChange();
        }

        if (currentState)
            currentState->update(dt);

        applyPendingStateChange();

        window.clear(sf::Color::Black);
        if (currentState)
            currentState->render(window);

        ui::Palette palette = ui::getPalette(data.inventory ? data.inventory->getCurrentThemeID() : 1);
        ui::drawCursor(window, palette, false);
        window.display();
    }
}
