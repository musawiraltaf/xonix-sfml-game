#include "GameRoomState.h"
#include "Game.h"
#include "MainMenuState.h"
#include "GameState.h"
#include "LoginState.h"
#include "GameLogic.h"      
#include "MatchResultState.h"
#include <sstream>

GameRoomState::GameRoomState(Game& game, GameData& data)
    : State(game, data),
    playerTarget(4),
    currentInQueue(false)
{
    font.loadFromFile("ariblk.ttf");

    // ----- Title -----
    titleText.setFont(font);
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color::White);
    titleText.setString("Matchmaking Room");
    titleText.setPosition(40.f, 20.f);

    // ----- Info text -----
    infoText.setFont(font);
    infoText.setCharacterSize(16);
    infoText.setFillColor(sf::Color::Yellow);
    infoText.setString("Select 4 / 6 / 8 players, then Join Queue and Start Matches.");
    infoText.setPosition(40.f, 70.f);

    // ----- Mode buttons: 4 / 6 / 8 -----
    const float btnWidth = 150.f;
    const float btnHeight = 35.f;

    float startX = 40.f;
    float y = 110.f;
    int   modes[3] = { 4, 6, 8 };

    for (int i = 0; i < 3; ++i)
    {
        modeButtons[i].setSize(sf::Vector2f(btnWidth, btnHeight));
        modeButtons[i].setFillColor(sf::Color(70, 70, 70));
        modeButtons[i].setOutlineThickness(2.f);
        modeButtons[i].setOutlineColor(sf::Color::White);
        modeButtons[i].setPosition(startX + i * (btnWidth + 10.f), y);

        modeLabels[i].setFont(font);
        modeLabels[i].setCharacterSize(18);
        modeLabels[i].setFillColor(sf::Color::White);

        std::ostringstream oss;
        oss << modes[i] << " Players";
        modeLabels[i].setString(oss.str());

        sf::FloatRect r = modeLabels[i].getLocalBounds();
        float cx = modeButtons[i].getPosition().x + btnWidth / 2.f;
        float cy = modeButtons[i].getPosition().y + btnHeight / 2.f;
        modeLabels[i].setPosition(cx - r.width / 2.f - r.left,
            cy - r.height / 2.f - r.top);
    }

    // ----- Join Queue button -----
    joinButton.setSize(sf::Vector2f(160.f, 40.f));
    joinButton.setFillColor(sf::Color(80, 80, 80));
    joinButton.setOutlineThickness(2.f);
    joinButton.setOutlineColor(sf::Color::White);
    joinButton.setPosition(40.f, 160.f);

    joinLabel.setFont(font);
    joinLabel.setCharacterSize(18);
    joinLabel.setFillColor(sf::Color::White);
    joinLabel.setString("Join Queue");

    {
        sf::FloatRect r = joinLabel.getLocalBounds();
        float cx = joinButton.getPosition().x + joinButton.getSize().x / 2.f;
        float cy = joinButton.getPosition().y + joinButton.getSize().y / 2.f;
        joinLabel.setPosition(cx - r.width / 2.f - r.left,
            cy - r.height / 2.f - r.top);
    }

    // ----- Start Matches button -----
    startButton.setSize(sf::Vector2f(200.f, 40.f));
    startButton.setFillColor(sf::Color(100, 80, 80));
    startButton.setOutlineThickness(2.f);
    startButton.setOutlineColor(sf::Color::White);
    startButton.setPosition(220.f, 160.f);

    startLabel.setFont(font);
    startLabel.setCharacterSize(18);
    startLabel.setFillColor(sf::Color::White);
    startLabel.setString("Start Matches");

    {
        sf::FloatRect r = startLabel.getLocalBounds();
        float cx = startButton.getPosition().x + startButton.getSize().x / 2.f;
        float cy = startButton.getPosition().y + startButton.getSize().y / 2.f;
        startLabel.setPosition(cx - r.width / 2.f - r.left,
            cy - r.height / 2.f - r.top);
    }

    // ----- Back button (ESC equivalent) -----
    backButton.setSize(sf::Vector2f(140.f, 35.f));
    backButton.setFillColor(sf::Color(60, 60, 60));
    backButton.setOutlineThickness(2.f);
    backButton.setOutlineColor(sf::Color::White);
    backButton.setPosition(40.f, static_cast<float>(M * ts) - 60.f);

    backLabel.setFont(font);
    backLabel.setCharacterSize(18);
    backLabel.setFillColor(sf::Color::White);
    backLabel.setString("Back (Esc)");

    {
        sf::FloatRect r = backLabel.getLocalBounds();
        float cx = backButton.getPosition().x + backButton.getSize().x / 2.f;
        float cy = backButton.getPosition().y + backButton.getSize().y / 2.f;
        backLabel.setPosition(cx - r.width / 2.f - r.left,
            cy - r.height / 2.f - r.top);
    }

    // ----- Queue text -----
    queueText.setFont(font);
    queueText.setCharacterSize(16);
    queueText.setFillColor(sf::Color::Cyan);
    queueText.setPosition(40.f, 220.f);

    // ----- Match preview text -----
    matchText.setFont(font);
    matchText.setCharacterSize(16);
    matchText.setFillColor(sf::Color::Green);
    // put it on the right side of the screen
    matchText.setPosition(static_cast<float>(N * ts) / 2.f + 40.f, 220.f);

    rebuildQueueAndMatches();
}

bool GameRoomState::isMouseOver(const sf::RectangleShape& rect,
    const sf::Vector2f& point) const
{
    return rect.getGlobalBounds().contains(point);
}

void GameRoomState::handleClickAt(const sf::Vector2f& point)
{
    // ----- Back -----
    if (isMouseOver(backButton, point))
    {
        game.changeState(new MainMenuState(game, data));
        return;
    }

    // ----- Mode buttons: choose 4 / 6 / 8 -----
    int modes[3] = { 4, 6, 8 };
    for (int i = 0; i < 3; ++i)
    {
        if (isMouseOver(modeButtons[i], point))
        {
            playerTarget = modes[i];

            std::ostringstream oss;
            oss << "Target: " << playerTarget
                << " players. Join Queue and then Start Matches.";
            infoText.setString(oss.str());
            return;
        }
    }

    // ----- Join Queue -----
    // ----- Join Queue -----
    if (isMouseOver(joinButton, point))
    {
        // Must pick 4 / 6 / 8 first
        if (playerTarget == 0)
        {
            infoText.setString("Please select 4, 6 or 8 players first.");
            return;
        }

        // How many players are already in the queue?
        MMEntry snapshot[MatchmakingQueue::MAX_WAITING];
        int count = 0;
        data.matchQueue.getSnapshot(snapshot, count);

        if (count >= playerTarget)
        {
            std::ostringstream oss;
            oss << "Queue already has " << playerTarget
                << " players.";
            infoText.setString(oss.str());
            return;
        }

        // We still need more players -> go to LoginState in ROOM mode.
        // LOGIN_FOR_ROOM will:
        //   * ask for username/password
        //   * enqueue that player into data.matchQueue
        //   * return to a fresh GameRoomState
        game.changeState(new LoginState(game, data, LOGIN_FOR_ROOM));
        return;
    }

    // ----- Start Matches -----
    if (isMouseOver(startButton, point))
    {
        // Check if we have enough players in queue
        MMEntry snapshot[MatchmakingQueue::MAX_WAITING];
        int count = 0;
        data.matchQueue.getSnapshot(snapshot, count);

        if (count < playerTarget)
        {
            std::ostringstream oss;
            oss << "Need " << playerTarget << " players in queue. Currently: "
                << count << ".";
            infoText.setString(oss.str());
            return;
        }
        // We are starting a matchmaking series now
        data.matchmakingActive = true;
        data.lastWinner = nullptr;

        // Take the TOP TWO players and start a match (local 2P in your game)
        MMEntry a, b;
        if (!data.matchQueue.popTopTwo(a, b))
        {
            infoText.setString("Not enough players to create a match.");
            rebuildQueueAndMatches();
            return;
        }

        // Start a 2-player game between these two
        data.currentPlayer = a.player;
        data.coopPlayer = b.player;

        // When game finishes and returns to menus, the queue still contains
        // the remaining players, so you can come back to GameRoom and
        // start the next pair.
        game.changeState(new GameState(game, data, 2));
        return;
    }
}

void GameRoomState::handleEvent(sf::Event& event)
{
    // ESC -> back to main menu
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape)
    {
        game.changeState(new MainMenuState(game, data));
        return;
    }

    // Handle left mouse click (both press or release, to be safe)
    if ((event.type == sf::Event::MouseButtonPressed ||
        event.type == sf::Event::MouseButtonReleased) &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::RenderWindow& window = game.getWindow();

        // Convert from window pixels to in-game coordinates
        sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

        handleClickAt(worldPos);
    }
}

void GameRoomState::update(float /*dt*/)
{
    // No animations for now.
}

void GameRoomState::render(sf::RenderWindow& window)
{
    window.draw(titleText);
    window.draw(infoText);

    for (int i = 0; i < 3; ++i)
    {
        window.draw(modeButtons[i]);
        window.draw(modeLabels[i]);
    }

    window.draw(joinButton);
    window.draw(joinLabel);

    window.draw(startButton);
    window.draw(startLabel);

    window.draw(backButton);
    window.draw(backLabel);

    window.draw(queueText);
    window.draw(matchText);
}

void GameRoomState::rebuildQueueAndMatches()
{
    // Build queue display and "next match" preview from the priority queue.
    MMEntry snapshot[MatchmakingQueue::MAX_WAITING];
    int count = 0;
    data.matchQueue.getSnapshot(snapshot, count);

    // ----- Queue text -----
    std::ostringstream qss;
    qss << "Queue (by priority / total points):\n";
    if (count == 0)
    {
        qss << "  [empty]";
    }
    else
    {
        for (int i = 0; i < count; ++i)
        {
            Player* p = snapshot[i].player;
            if (!p) continue;

            qss << "  " << (i + 1) << ". " << p->username
                << "  (totalPoints=" << p->totalPoints << ")\n";
        }
    }
    queueText.setString(qss.str());

    // ----- Match preview -----
    std::ostringstream mss;
    mss << "Next match (top 2 in queue):\n";

    if (count >= 2)
    {
        Player* p1 = snapshot[0].player;
        Player* p2 = snapshot[1].player;
        if (p1 && p2)
        {
            mss << "  " << p1->username << "  vs  " << p2->username << "\n";
        }
        else
        {
            mss << "  [invalid players]\n";
        }
    }
    else
    {
        mss << "  [waiting for more players...]\n";
    }

    matchText.setString(mss.str());
}
