#pragma once

#include "Player.h"

// A matchmaking entry: wraps a Player* plus its priority/score.
struct MMEntry
{
    Player* player;
    int     priority;     // used by the priority queue (higher = better)
    int     totalPoints;  // used only for display; same as priority
};

class MatchmakingQueue
{
public:
    static const int MAX_WAITING = 16;

    MatchmakingQueue();

    void clear();
    bool isEmpty() const;
    bool canCreateMatch() const;
    bool containsPlayer(Player* player) const;

    void push(Player* player, int priority);
    bool popTopTwo(MMEntry& a, MMEntry& b);
    void getSnapshot(MMEntry outArray[], int& count) const;

private:
    MMEntry arr[MAX_WAITING];
    int     size;

    void heapifyUp(int index);
    void heapifyDown(int index);
};
