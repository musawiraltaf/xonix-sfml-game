#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "GameLogic.h"
#include "Player.h"

class GameState : public State
{
public:
    // mode: 1 = single player, 2 = local co-op
    GameState(Game& game, GameData& data, int mode);

    virtual void handleEvent(sf::Event& event) override;
    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;
private:
    // --------- mode / players ----------
    int     gameMode;      // 1 = single, 2 = co-op
    Player* p1;            // data.currentPlayer
    Player* p2;            // data.coopPlayer (only if co-op)

    // --------- player positions / movement ----------
    int x1, y1, dx1, dy1;  // P1 (grid coords)
    int x2, y2, dx2, dy2;  // P2 (grid coords)

    bool p1Alive;
    bool p2Alive;

    // --------- enemies ----------
    Enemy enemies[10];
    int   enemyCount;

    // --------- game flow ----------
    bool  gameRunning;     // false when game over
    float timer;           // step timer
    float delay;           // step interval (e.g. 0.07s)
    float gameTime;        // total elapsed time

    // --------- power-up / freeze ----------
    bool  freezeActive;
    float freezeTimer;
    // 0 = none frozen, 1 = P1 frozen, 2 = P2 frozen
    int   frozenPlayer;

    int   tilesCapturedThisMove;

    // --------- UI / HUD ----------
    sf::Font font;
    sf::Text p1Hud;
    sf::Text p2Hud;
    sf::Text timerText;
    sf::Text gameOverText;

    // --------- graphics: textures & sprites ----------
    sf::Texture tileTexture;
    sf::Texture enemyTexture;
    sf::Texture gameOverTexture;

    sf::Sprite  tileSprite;
    sf::Sprite  enemySprite;
    sf::Sprite  gameOverSprite;

    float enemySpinAngle;  // for spinning enemies


private:
    void resetBoard();
    void updateHud();

    void handlePowerUpEvent(const sf::Event& event);
    void handlePlayerInput();

    void stepSimulation();
    void movePlayers();
    void moveEnemies();

    void handleRegionClosureIfNeeded();
    void applyRegionClosure(bool p1Closed, bool p2Closed);

    void checkPlayerCollisions();
    void checkEnemyTrailCollisions();

    // NEW
    bool p1HasTrail;
    bool p2HasTrail;

    void checkEnemyPlayerCollisions();          // <--- we'll define this
    void awardTilesToPlayer(Player* player, int tiles);

    void markTrails();
    void checkGameEnd();
};

