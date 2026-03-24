#pragma once

#include <string>
#include "Leaderboard.h"
#include "MatchmakingQueue.h"
#include "Player.h"
#include "Inventory.h"
#include "FriendsManager.h"

const int MAX_PLAYERS = 100;

struct GameData
{
    Player users[MAX_PLAYERS];
    int userCount;

    int playerCount;
    Player* sessionPlayer;
    Player* currentPlayer;
    Player* coopPlayer;

    int highScore;
    int difficulty;

    Leaderboard leaderboard;
    MatchmakingQueue matchQueue;
    bool matchmakingActive;
    Player* lastWinner;

    Player* roomPlayers[8];
    Player* roomWinners[8];
    int roomPlayerTarget;
    int roomPlayerCount;
    int roomWinnerCount;
    int roomCurrentMatchStart;

    InventoryManager* inventory;
    FriendsManager friends;

    GameData()
        : userCount(0), playerCount(0), sessionPlayer(nullptr), currentPlayer(nullptr), coopPlayer(nullptr),
          highScore(0), difficulty(1), matchmakingActive(false), lastWinner(nullptr),
          roomPlayerTarget(0), roomPlayerCount(0), roomWinnerCount(0), roomCurrentMatchStart(0), inventory(nullptr)
    {
        for (int i = 0; i < 8; ++i)
        {
            roomPlayers[i] = nullptr;
            roomWinners[i] = nullptr;
        }
    }
};
