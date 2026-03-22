#include "GameState.h"
#include "Game.h"
#include "GameData.h"
#include "GameLogic.h"
#include "SinglePlayerMenuState.h"
#include "MultiplayerMenuState.h"
#include "MatchResultState.h"
#include <sstream>

static const int TILE_EMPTY = 0;
static const int TILE_FILLED = 1;
static const int TILE_P1_TRAIL = 2;
static const int TILE_P2_TRAIL = 3;

// Clamp a grid position (gx, gy) into valid range [0..N-1], [0..M-1]
static inline void clampGridPos(int& gx, int& gy)
{
    if (gx < 0)       gx = 0;
    else if (gx > N - 1) gx = N - 1;

    if (gy < 0)       gy = 0;
    else if (gy > M - 1) gy = M - 1;
}

GameState::GameState(Game& game, GameData& data, int mode)
    : State(game, data)
    , gameMode(mode)
    , p1(nullptr)
    , p2(nullptr)
    , enemyCount(4)
    , gameRunning(true)
    , timer(0.f)
    , delay(0.04f)
    , gameTime(0.f)
    , freezeActive(false)
    , freezeTimer(0.f)
    , frozenPlayer(0)
{
    p1 = data.currentPlayer;
    if (gameMode == 2)
        p2 = data.coopPlayer;

    p1Alive = (p1 != nullptr);
    p2Alive = (gameMode == 2 && p2 != nullptr);

    font.loadFromFile("ariblk.ttf");

    p1Hud.setFont(font);
    p1Hud.setCharacterSize(18);
    p1Hud.setFillColor(sf::Color::Yellow);
    p1Hud.setPosition(10.f, 5.f);

    p2Hud.setFont(font);
    p2Hud.setCharacterSize(18);
    p2Hud.setFillColor(sf::Color::Cyan);

    timerText.setFont(font);
    timerText.setCharacterSize(18);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(10.f, 30.f);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(24);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(100.f, 200.f);

    tileTexture.loadFromFile("images/tiles.png");
    enemyTexture.loadFromFile("images/enemy.png");

    tileSprite.setTexture(tileTexture);
    enemySprite.setTexture(enemyTexture);
    enemySprite.setOrigin(
        enemySprite.getLocalBounds().width / 2.f,
        enemySprite.getLocalBounds().height / 2.f
    );

    resetBoard();
    updateHud();
}

/* ====================== SAFE RESET BOARD ======================== */

void GameState::resetBoard()
{
    for (int y = 0; y < M; ++y)
        for (int x = 0; x < N; ++x)
            grid[y][x] = TILE_EMPTY;

    for (int i = 0; i < N; ++i)
        grid[0][i] = grid[M - 1][i] = TILE_FILLED;

    for (int i = 0; i < M; ++i)
        grid[i][0] = grid[i][N - 1] = TILE_FILLED;

    x1 = 10; y1 = 0; dx1 = 0; dy1 = 0;
    x2 = N - 11; y2 = 0; dx2 = 0; dy2 = 0;

    p1Alive = (p1 != nullptr);
    p2Alive = (gameMode == 2 && p2 != nullptr);

    for (int i = 0; i < enemyCount; i++)
        enemies[i] = Enemy();

    freezeActive = false;
    frozenPlayer = 0;
    freezeTimer = 0.f;

    gameRunning = true;
    gameTime = 0.f;
    timer = 0.f;

    if (p1) p1->resetForNewGame();
    if (p2) p2->resetForNewGame();
}

/* ========================= EVENT ========================== */

void GameState::handleEvent(sf::Event& e)
{
    if (e.type == sf::Event::KeyPressed)
    {
        if (e.key.code == sf::Keyboard::Escape)
        {
            if (gameMode == 1)
                game.changeState(new SinglePlayerMenuState(game, data));
            else
                game.changeState(new MultiplayerMenuState(game, data));
            return;
        }
    }
}

/* ========================= SAFE INPUT ========================== */

void GameState::handlePlayerInput()
{
    if (p1Alive && !(freezeActive && frozenPlayer == 1))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  dx1 = -1, dy1 = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dx1 = 1, dy1 = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    dx1 = 0, dy1 = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  dx1 = 0, dy1 = 1;
    }

    if (p2Alive && !(freezeActive && frozenPlayer == 2))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dx2 = -1, dy2 = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dx2 = 1, dy2 = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dx2 = 0, dy2 = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dx2 = 0, dy2 = 1;
    }
}

/* ============================ UPDATE ============================= */

void GameState::update(float dt)
{
    if (!gameRunning)
        return;

    gameTime += dt;
    timer += dt;

    if (freezeActive)
    {
        freezeTimer -= dt;
        if (freezeTimer <= 0.f)
        {
            freezeActive = false;
            frozenPlayer = 0;
        }
    }

    handlePlayerInput();

    if (timer > delay)
    {
        stepSimulation();
        timer = 0.f;
    }

    updateHud();
}

/* ============================================================= */

inline void clampPos(int& x, int max)
{
    if (x < 0) x = 0;
    if (x >= max) x = max - 1;
}

/* ====================== STEP SIMULATION ====================== */

void GameState::stepSimulation()
{
    movePlayers();
    checkPlayerCollisions();

    if (!freezeActive)
    {
        moveEnemies();
        checkEnemyTrailCollisions();
    }

    checkEnemyPlayerCollisions();
    markTrails();
    checkGameEnd();
}

/* ===================== MOVE ========================== */

void GameState::movePlayers()
{
    if (p1Alive)
    {
        x1 += dx1; y1 += dy1;
        clampPos(x1, N);
        clampPos(y1, M);
    }

    if (p2Alive)
    {
        x2 += dx2; y2 += dy2;
        clampPos(x2, N);
        clampPos(y2, M);
    }
}

void GameState::moveEnemies()
{
    for (int i = 0; i < enemyCount; i++)
        enemies[i].move();
}

/* ====================== SAFE CHECKS ======================= */

//void GameState::checkEnemyPlayerCollisions()
//{
//    for (int i = 0; i < enemyCount; i++)
//    {
//        int gx = enemies[i].x / ts;
//        int gy = enemies[i].y / ts;
//        clampPos(gx, N);
//        clampPos(gy, M);
//
//        if (p1Alive && gx == x1 && gy == y1)
//            p1Alive = false;
//        if (p2Alive && gx == x2 && gy == y2)
//            p2Alive = false;
//    }
//}

void GameState::checkEnemyPlayerCollisions()
{
    for (int i = 0; i < enemyCount; ++i)
    {
        int gx = enemies[i].x / ts;
        int gy = enemies[i].y / ts;

        // Make absolutely sure we never go out of bounds
        clampGridPos(gx, gy);

        // Just compare with player tile positions
        if (p1Alive && gx == x1 && gy == y1)
            p1Alive = false;

        if (gameMode == 2 && p2Alive && gx == x2 && gy == y2)
            p2Alive = false;
    }
}


void GameState::checkEnemyTrailCollisions()
{
    for (int i = 0; i < enemyCount; i++)
    {
        int gx = enemies[i].x / ts;
        int gy = enemies[i].y / ts;
        clampPos(gx, N);
        clampPos(gy, M);

        int tile = grid[gy][gx];
        if (tile == TILE_P1_TRAIL)
            p1Alive = false;
        else if (tile == TILE_P2_TRAIL)
            p2Alive = false;
    }
}

/* ====================== PLAYER-PLAYER ====================== */

void GameState::checkPlayerCollisions()
{
    int t1 = p1Alive ? grid[y1][x1] : TILE_FILLED;
    int t2 = p2Alive ? grid[y2][x2] : TILE_FILLED;

    if (gameMode == 2 && p1Alive && p2Alive && x1 == x2 && y1 == y2)
    {
        if (t1 == TILE_EMPTY && t2 == TILE_EMPTY)
            p1Alive = p2Alive = false;
        else if (t1 == TILE_EMPTY)
            p1Alive = false;
        else if (t2 == TILE_EMPTY)
            p2Alive = false;
    }

    if (p1Alive && (t1 == TILE_P1_TRAIL || t1 == TILE_P2_TRAIL))
        p1Alive = false;
    if (p2Alive && (t2 == TILE_P1_TRAIL || t2 == TILE_P2_TRAIL))
        p2Alive = false;
}

/* ================== TRAILS ================== */

void GameState::markTrails()
{
    if (p1Alive && grid[y1][x1] == TILE_EMPTY)
        grid[y1][x1] = TILE_P1_TRAIL;

    if (p2Alive && grid[y2][x2] == TILE_EMPTY)
        grid[y2][x2] = TILE_P2_TRAIL;
}

/* ===================== GAME END ======================= */

void GameState::checkGameEnd()
{
    // Single-player ends when P1 dies.
    // Multiplayer ends when BOTH die.
    if (!p1Alive && (gameMode == 1 || !p2Alive))
    {
        gameRunning = false;

        // ---- Decide winner code: 0 = tie, 1 = P1, 2 = P2 ----
        int winner = 0;

        if (gameMode == 2 && p1 && p2)
        {
            if (p1->score > p2->score)      winner = 1;
            else if (p2->score > p1->score) winner = 2;
            else                             winner = 0;
        }
        else
        {
            // single player: treat P1 as "winner"
            winner = 1;
        }

        // ---- Update stats safely (no strings here) ----
        if (p1) p1->totalPoints += p1->score;
        if (p2) p2->totalPoints += p2->score;

        // update highScore
        if (p1 && p1->score > data.highScore) data.highScore = p1->score;
        if (p2 && p2->score > data.highScore) data.highScore = p2->score;

        // leaderboard (top-10)
        if (p1) data.leaderboard.updateWithPlayer(*p1);
        if (p2) data.leaderboard.updateWithPlayer(*p2);
        data.leaderboard.saveToFile("leaderboard.txt");

        // ---- Leave this state and go to MatchResultState ----
        // VERY IMPORTANT: do this *last* and then immediately return.
        game.changeState(new MatchResultState(game, data, winner));
        return;
    }
}


/* ===================== HUD ======================= */

void GameState::updateHud()
{
    std::ostringstream ss;

    if (p1)
        ss << "P1: " << p1->username << "  Score: " << p1->score;
    else
        ss << "P1: N/A";

    p1Hud.setString(ss.str());

    if (gameMode == 2)
    {
        std::ostringstream ss2;
        if (p2)
            ss2 << "P2: " << p2->username << "  Score: " << p2->score;
        else
            ss2 << "P2: N/A";

        p2Hud.setString(ss2.str());
        float width = N * ts;
        auto r = p2Hud.getLocalBounds();
        p2Hud.setPosition(width - 10 - r.width, 5);
    }

    std::ostringstream tss;
    tss << "Time: " << int(gameTime);
    timerText.setString(tss.str());
}

/* ==================== RENDER =============================== */

void GameState::render(sf::RenderWindow& window)
{
    // ----- draw grid using tiles.png -----
    for (int y = 0; y < M; ++y)
    {
        for (int x = 0; x < N; ++x)
        {
            int v = grid[y][x];
            if (v == TILE_EMPTY)
                continue;

            // Always reset colour first so old tint doesn’t leak
            tileSprite.setColor(sf::Color::White);

            if (v == TILE_FILLED)
            {
                // normal land tile
                tileSprite.setTextureRect(sf::IntRect(0, 0, ts, ts));
            }
            else if (v == TILE_P1_TRAIL)
            {
                tileSprite.setTextureRect(sf::IntRect(54, 0, ts, ts));
                tileSprite.setColor(sf::Color::Yellow);   // P1 trail
            }
            else if (v == TILE_P2_TRAIL)
            {
                tileSprite.setTextureRect(sf::IntRect(54, 0, ts, ts));
                tileSprite.setColor(sf::Color::Cyan);     // P2 trail
            }

            tileSprite.setPosition(static_cast<float>(x * ts),
                static_cast<float>(y * ts));
            window.draw(tileSprite);
        }
    }

    // ----- draw players as coloured tiles -----
    tileSprite.setTextureRect(sf::IntRect(36, 0, ts, ts)); // player tile

    if (p1Alive)
    {
        tileSprite.setColor(sf::Color::Yellow);
        tileSprite.setPosition(static_cast<float>(x1 * ts),
            static_cast<float>(y1 * ts));
        window.draw(tileSprite);
    }

    if (gameMode == 2 && p2Alive)
    {
        tileSprite.setColor(sf::Color::Green);
        tileSprite.setPosition(static_cast<float>(x2 * ts),
            static_cast<float>(y2 * ts));
        window.draw(tileSprite);
    }

    // ----- draw enemies using enemy.png -----
    for (int i = 0; i < enemyCount; ++i)
    {
        enemySprite.setPosition(static_cast<float>(enemies[i].x),
            static_cast<float>(enemies[i].y));
        window.draw(enemySprite);
    }

    // ----- HUD -----
    window.draw(p1Hud);
    if (gameMode == 2)
        window.draw(p2Hud);
    window.draw(timerText);

    // (If you want a text game-over overlay, you can re-enable it later)
    // if (!gameRunning)
    // {
    //     window.draw(gameOverSprite);
    //     window.draw(gameOverText);
    // }
}
