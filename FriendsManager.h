#pragma once

#include <string>
#include "Player.h"

const int FRIENDS_MAX_PLAYERS = 100;
const int USERNAME_HASH_CAPACITY = 211;

struct FriendNode
{
    int userIndex;
    FriendNode* next;

    FriendNode(int idx = -1) : userIndex(idx), next(nullptr) {}
};

class UsernameHashTable
{
public:
    UsernameHashTable();

    void clear();
    bool insert(const std::string& key, int value);
    int find(const std::string& key) const;

private:
    struct Slot
    {
        std::string key;
        int value;
        bool occupied;

        Slot() : key(), value(-1), occupied(false) {}
    };

    Slot slots[USERNAME_HASH_CAPACITY];

    int hash(const std::string& key) const;
};

class FriendsManager
{
public:
    FriendsManager();
    ~FriendsManager();

    void initialize(Player users[], int count);
    void clearAll();
    void rebuildHash(Player users[], int count);

    bool loadFromFile(const std::string& filename, Player users[], int count);
    bool saveToFile(const std::string& filename) const;

    int findUserIndex(const std::string& username) const;

    bool sendRequest(int fromIndex, const std::string& toUsername);
    bool acceptRequest(int targetIndex, int requestPosition);
    bool rejectRequest(int targetIndex, int requestPosition);

    int getFriendCount(int userIndex) const;
    int getPendingCount(int userIndex) const;

    int getFriendIndexAt(int userIndex, int position) const;
    int getPendingIndexAt(int userIndex, int position) const;

    bool areFriends(int a, int b) const;

private:
    FriendNode* friends[FRIENDS_MAX_PLAYERS];
    FriendNode* pending[FRIENDS_MAX_PLAYERS];
    UsernameHashTable userLookup;
    Player* userArray;
    int userCount;

    void clearList(FriendNode*& head);
    bool contains(FriendNode* head, int value) const;
    void appendUnique(FriendNode*& head, int value);
    bool removeAt(FriendNode*& head, int position, int& removedValue);
    bool removeValue(FriendNode*& head, int value);
    int getCount(FriendNode* head) const;
    int getValueAt(FriendNode* head, int position) const;
    bool addFriendship(int a, int b);
};
