#pragma once
#include <string>

struct Player
{
    // Login info
    std::string username;
    std::string password;

    // Persistent stats
    int highScore;    // best score across all games
    int totalPoints;  // sum of all points earned over time
    int wins;
    int losses;

    // Session / run stats
    int score;        // current game score
    int powerUps;     // number of available freeze power-ups

    // --- NEW: bonus + power-up progression state ---
    int bonusCount;       // how many times a "big area" bonus has been triggered
    int bonusThreshold;   // tiles needed for a bonus (starts at 10, then 5)
    int nextPowerupScore; // next score at which this player gets a power-up

    Player();

    // Reset only per session (e.g. when starting a new run)
    void resetPerSession();

    // Reset for a brand new game (if you want slightly stronger reset)
    void resetForNewGame();

    // Add score and update high score / totalPoints, grant power-ups if thresholds reached
    void updateScore(int delta);
};
