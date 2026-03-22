// Leaderboard.cpp
#include "Leaderboard.h"
#include "Player.h"
#include <fstream>
#include <sstream>

Leaderboard::Leaderboard() : size(0) {}

// Compare by totalPoints first, then highScore
// >0 => a better than b
// <0 => a worse than b
//  0 => same
int Leaderboard::compare(const LBEntry& a, const LBEntry& b) const {
    if (a.totalPoints > b.totalPoints) return 1;
    if (a.totalPoints < b.totalPoints) return -1;

    if (a.highScore > b.highScore) return 1;
    if (a.highScore < b.highScore) return -1;

    return 0;
}

// For min-heap: parent <= child in terms of "worse/better"
bool Leaderboard::isLessOrEqual(const LBEntry& a, const LBEntry& b) const {
    return compare(a, b) <= 0;   // a is worse-or-equal to b
}

void Leaderboard::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        // parent must be worse-or-equal to child in a min-heap
        if (isLessOrEqual(heap[parent], heap[index])) {
            break;
        }
        // swap parent and child
        LBEntry temp = heap[parent];
        heap[parent] = heap[index];
        heap[index] = temp;

        index = parent;
    }
}

void Leaderboard::heapifyDown(int index) {
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        // pick the WORST (smallest) among index, left, right
        if (left < size && !isLessOrEqual(heap[smallest], heap[left])) {
            smallest = left;
        }
        if (right < size && !isLessOrEqual(heap[smallest], heap[right])) {
            smallest = right;
        }

        if (smallest == index) {
            break;
        }

        LBEntry temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;

        index = smallest;
    }
}

void Leaderboard::insertIntoHeap(const LBEntry& entry) {
    if (size < MAX_SIZE) {
        heap[size] = entry;
        heapifyUp(size);
        ++size;
    }
    else {
        // Heap is full: root is the WORST of top-10.
        // Only insert if new entry is BETTER than the root.
        if (compare(entry, heap[0]) > 0) {
            heap[0] = entry;
            heapifyDown(0);
        }
        // else: ignore, not good enough for top-10
    }
}

int Leaderboard::findPlayerIndex(const std::string& username) const {
    for (int i = 0; i < size; ++i) {
        if (heap[i].username == username) {
            return i;
        }
    }
    return -1;
}

void Leaderboard::rebuildHeap() {
    // Build heap in O(n). n <= 10, so this is cheap.
    for (int i = size / 2 - 1; i >= 0; --i) {
        heapifyDown(i);
    }
}

void Leaderboard::loadFromFile(const std::string& filename) {
    size = 0;

    std::ifstream in(filename.c_str());
    if (!in.is_open()) {
        // First run: file may not exist yet, not an error.
        return;
    }

    std::string line;
    while (std::getline(in, line) && size < MAX_SIZE) {
        std::istringstream iss(line);
        LBEntry entry;
        if (!(iss >> entry.username >> entry.totalPoints >> entry.highScore)) {
            continue; // skip invalid line
        }
        heap[size] = entry;
        ++size;
    }

    in.close();
    rebuildHeap();
}

void Leaderboard::saveToFile(const std::string& filename) const {
    std::ofstream out(filename.c_str(), std::ios::trunc);
    if (!out.is_open()) {
        return; // could log if you have logging
    }

    for (int i = 0; i < size; ++i) {
        out << heap[i].username << ' '
            << heap[i].totalPoints << ' '
            << heap[i].highScore << '\n';
    }

    out.close();
}

void Leaderboard::updateWithPlayer(const Player& player) {
    LBEntry entry;
    entry.username = player.username;
    entry.totalPoints = player.totalPoints;
    entry.highScore = player.highScore;

    // If player already exists in leaderboard, update their entry
    int idx = findPlayerIndex(entry.username);
    if (idx != -1) {
        heap[idx] = entry;
        rebuildHeap();
        return;
    }

    // Otherwise, insert into heap (will only keep them if they’re in top-10)
    insertIntoHeap(entry);
}

// Copies sorted (descending) entries into outArray, returns count
int Leaderboard::getEntriesDescending(LBEntry outArray[], int maxOut) const {
    int count = size;
    if (count > maxOut) {
        count = maxOut;
    }

    // Copy current heap elements
    for (int i = 0; i < count; ++i) {
        outArray[i] = heap[i];
    }

    // Selection sort in DESCENDING order using compare()
    for (int i = 0; i < count; ++i) {
        int bestIdx = i;
        for (int j = i + 1; j < count; ++j) {
            if (compare(outArray[j], outArray[bestIdx]) > 0) {
                bestIdx = j;
            }
        }
        if (bestIdx != i) {
            LBEntry tmp = outArray[i];
            outArray[i] = outArray[bestIdx];
            outArray[bestIdx] = tmp;
        }
    }

    return count;
}
