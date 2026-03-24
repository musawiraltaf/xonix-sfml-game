#pragma once

#include <string>

struct SaveGameRecord
{
    std::string saveID;
    std::string username;
    long long savedAtEpoch;

    int gameMode;
    int level;
    int enemyCount;
    int targetCapturePercent;

    int x1, y1, dx1, dy1;
    int x2, y2, dx2, dy2;
    int p1Alive;
    int p2Alive;
    int p1HasTrail;
    int p2HasTrail;

    int p1Score;
    int p1PowerUps;
    int p1BonusCount;
    int p1BonusThreshold;
    int p1NextPowerupScore;

    int p2Score;
    int p2PowerUps;
    int p2BonusCount;
    int p2BonusThreshold;
    int p2NextPowerupScore;

    int freezeActive;
    float freezeTimer;
    int frozenPlayer;
    float delay;
    float gameTime;

    int enemyX[10];
    int enemyY[10];
    int enemyDx[10];
    int enemyDy[10];

    int grid[25][40];

    SaveGameRecord();
};

struct SaveGameMeta
{
    std::string saveID;
    std::string username;
    long long savedAtEpoch;
    std::string timestampText;
    int level;
    int score;

    SaveGameMeta() : saveID(), username(), savedAtEpoch(0), timestampText(), level(1), score(0) {}
};

std::string generateSaveID(const std::string& username);
std::string formatSaveTimestamp(long long epochSeconds);
bool saveGameToDisk(const SaveGameRecord& record);
bool loadGameFromDisk(const std::string& saveID, SaveGameRecord& outRecord);
int listSaveGamesForUser(const std::string& username, SaveGameMeta* outList, int maxCount);
