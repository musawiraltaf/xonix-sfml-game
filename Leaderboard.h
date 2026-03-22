// Leaderboard.h
#pragma once
#include <string>

struct Player; // forward declaration of your existing Player struct

struct LBEntry {
    std::string username;
    int totalPoints;
    int highScore;

    LBEntry() : username(), totalPoints(0), highScore(0) {}  
};


class Leaderboard {
public:
    static const int MAX_SIZE = 10;

    Leaderboard();

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;

    // Update leaderboard with a finished player's stats
    void updateWithPlayer(const Player& player);

    // Copy best entries (descending) into outArray.
    // Returns number of entries copied.
    int getEntriesDescending(LBEntry outArray[], int maxOut) const;

    int getSize() const { return size; }

private:
    LBEntry heap[MAX_SIZE]; // min-heap: root = WORST of top-10
    int size;

    // Comparison: >0 if a is better, <0 if a is worse, 0 if equal
    int compare(const LBEntry& a, const LBEntry& b) const;
    bool isLessOrEqual(const LBEntry& a, const LBEntry& b) const;

    void heapifyUp(int index);
    void heapifyDown(int index);
    void insertIntoHeap(const LBEntry& entry);

    int findPlayerIndex(const std::string& username) const;
    void rebuildHeap();
};
