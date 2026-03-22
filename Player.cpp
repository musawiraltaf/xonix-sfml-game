#include "Player.h"

Player::Player()
    : username()
    , password()
    , highScore(0)
    , totalPoints(0)
    , wins(0)
    , losses(0)
    , score(0)
    , powerUps(0)
    , bonusCount(0)
    , bonusThreshold(10)   // start: need >10 tiles for bonus
    , nextPowerupScore(50) // first power-up at 50
{
}

void Player::resetPerSession()
{
    // Clear values that should reset each time you start a new run
    score = 0;
    powerUps = 0;
    bonusCount = 0;
    bonusThreshold = 10;
    nextPowerupScore = 50;
}

void Player::resetForNewGame()
{
    // A "new game" can reuse resetPerSession;
    // you could also zero wins/losses/totalPoints if your design wants that.
    resetPerSession();
    // totalPoints, wins, losses are kept as overall career stats.
}

void Player::updateScore(int delta)
{
    if (delta <= 0) return;

    score += delta;
    totalPoints += delta;

    if (score > highScore)
        highScore = score;

    // -------- Power-up award logic (per-player) ----------
    // Power-ups at scores: 50, 70, 100, 130, then every +30 (160, 190, ...)
    while (nextPowerupScore > 0 && score >= nextPowerupScore)
    {
        powerUps++;

        // Advance to next threshold
        if (nextPowerupScore < 130)
        {
            if (nextPowerupScore == 50)      nextPowerupScore = 70;
            else if (nextPowerupScore == 70) nextPowerupScore = 100;
            else if (nextPowerupScore == 100)nextPowerupScore = 130;
            else                             nextPowerupScore = 160; // fallback
        }
        else
        {
            nextPowerupScore += 30; // 130,160,190,...
        }
    }
}
