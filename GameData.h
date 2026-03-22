#pragma once
#include "Leaderboard.h"
#include "MatchmakingQueue.h"   
#include "Player.h"
#include "Inventory.h" 

const int MAX_PLAYERS = 100;

struct GameData
{
    // All registered users (loaded from file)
    Player users[MAX_PLAYERS];
    int    userCount = 0;

    // "Active" players for current game/session
    int     playerCount = 0;   // 1 for single player, 2 for local co-op, etc.
    Player* currentPlayer = nullptr; // main logged-in player
    Player* coopPlayer = nullptr; // second player in co-op (optional)

    int     highScore = 0;
    Leaderboard leaderboard;
    MatchmakingQueue matchQueue;
    bool matchmakingActive = false;    // we’re running a series of ranked matches
    Player* lastWinner = nullptr;  // winner of the most recent match

    // --- NEW: room-based matchmaking session ---
    Player* roomPlayers[8];   // we only allow up to 8 as per project
    int     roomPlayerTarget; // 4, 6, or 8
    int     roomPlayerCount;  // how many have logged in so far
    InventoryManager inventory;
};
