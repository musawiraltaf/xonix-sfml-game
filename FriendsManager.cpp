#include "FriendsManager.h"

#include <fstream>
#include <sstream>

UsernameHashTable::UsernameHashTable()
{
    clear();
}

void UsernameHashTable::clear()
{
    for (int i = 0; i < USERNAME_HASH_CAPACITY; ++i)
        slots[i] = Slot();
}

int UsernameHashTable::hash(const std::string& key) const
{
    unsigned int value = 0u;
    for (std::size_t i = 0; i < key.size(); ++i)
        value = value * 131u + static_cast<unsigned int>(static_cast<unsigned char>(key[i]));
    return static_cast<int>(value % USERNAME_HASH_CAPACITY);
}

bool UsernameHashTable::insert(const std::string& key, int value)
{
    int start = hash(key);
    for (int step = 0; step < USERNAME_HASH_CAPACITY; ++step)
    {
        int idx = (start + step) % USERNAME_HASH_CAPACITY;
        if (!slots[idx].occupied || slots[idx].key == key)
        {
            slots[idx].key = key;
            slots[idx].value = value;
            slots[idx].occupied = true;
            return true;
        }
    }
    return false;
}

int UsernameHashTable::find(const std::string& key) const
{
    int start = hash(key);
    for (int step = 0; step < USERNAME_HASH_CAPACITY; ++step)
    {
        int idx = (start + step) % USERNAME_HASH_CAPACITY;
        if (!slots[idx].occupied)
            return -1;
        if (slots[idx].key == key)
            return slots[idx].value;
    }
    return -1;
}

FriendsManager::FriendsManager()
    : userArray(nullptr), userCount(0)
{
    for (int i = 0; i < FRIENDS_MAX_PLAYERS; ++i)
    {
        friends[i] = nullptr;
        pending[i] = nullptr;
    }
}

FriendsManager::~FriendsManager()
{
    clearAll();
}

void FriendsManager::clearList(FriendNode*& head)
{
    while (head)
    {
        FriendNode* next = head->next;
        delete head;
        head = next;
    }
}

void FriendsManager::clearAll()
{
    for (int i = 0; i < FRIENDS_MAX_PLAYERS; ++i)
    {
        clearList(friends[i]);
        clearList(pending[i]);
    }
    userLookup.clear();
}

void FriendsManager::rebuildHash(Player users[], int count)
{
    userArray = users;
    userCount = count;
    userLookup.clear();
    for (int i = 0; i < count; ++i)
        userLookup.insert(users[i].username, i);
}

void FriendsManager::initialize(Player users[], int count)
{
    clearAll();
    rebuildHash(users, count);
}

bool FriendsManager::contains(FriendNode* head, int value) const
{
    for (FriendNode* node = head; node; node = node->next)
    {
        if (node->userIndex == value)
            return true;
    }
    return false;
}

void FriendsManager::appendUnique(FriendNode*& head, int value)
{
    if (contains(head, value))
        return;

    FriendNode* node = new FriendNode(value);
    if (!head)
    {
        head = node;
        return;
    }

    FriendNode* cur = head;
    while (cur->next)
        cur = cur->next;
    cur->next = node;
}

bool FriendsManager::removeAt(FriendNode*& head, int position, int& removedValue)
{
    if (position < 0 || !head)
        return false;

    FriendNode* cur = head;
    FriendNode* prev = nullptr;
    int index = 0;
    while (cur && index < position)
    {
        prev = cur;
        cur = cur->next;
        ++index;
    }

    if (!cur)
        return false;

    removedValue = cur->userIndex;
    if (prev)
        prev->next = cur->next;
    else
        head = cur->next;
    delete cur;
    return true;
}


bool FriendsManager::removeValue(FriendNode*& head, int value)
{
    FriendNode* cur = head;
    FriendNode* prev = nullptr;
    while (cur)
    {
        if (cur->userIndex == value)
        {
            if (prev)
                prev->next = cur->next;
            else
                head = cur->next;
            delete cur;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

int FriendsManager::getCount(FriendNode* head) const
{
    int count = 0;
    for (FriendNode* node = head; node; node = node->next)
        ++count;
    return count;
}

int FriendsManager::getValueAt(FriendNode* head, int position) const
{
    int index = 0;
    for (FriendNode* node = head; node; node = node->next)
    {
        if (index == position)
            return node->userIndex;
        ++index;
    }
    return -1;
}

bool FriendsManager::addFriendship(int a, int b)
{
    if (a < 0 || b < 0 || a >= userCount || b >= userCount || a == b)
        return false;

    appendUnique(friends[a], b);
    appendUnique(friends[b], a);
    return true;
}

bool FriendsManager::loadFromFile(const std::string& filename, Player users[], int count)
{
    initialize(users, count);

    std::ifstream in(filename.c_str());
    if (!in.is_open())
        return false;

    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty())
            continue;

        std::istringstream iss(line);
        char type = '\0';
        int a = -1;
        int b = -1;
        if (!(iss >> type >> a >> b))
            continue;

        if (type == 'F')
            addFriendship(a, b);
        else if (type == 'R')
        {
            if (a >= 0 && a < userCount && b >= 0 && b < userCount)
                appendUnique(pending[a], b);
        }
    }

    return true;
}

bool FriendsManager::saveToFile(const std::string& filename) const
{
    std::ofstream out(filename.c_str(), std::ios::trunc);
    if (!out.is_open())
        return false;

    for (int i = 0; i < userCount; ++i)
    {
        for (FriendNode* node = friends[i]; node; node = node->next)
        {
            if (i < node->userIndex)
                out << 'F' << ' ' << i << ' ' << node->userIndex << '\n';
        }
    }

    for (int i = 0; i < userCount; ++i)
    {
        for (FriendNode* node = pending[i]; node; node = node->next)
            out << 'R' << ' ' << i << ' ' << node->userIndex << '\n';
    }

    return true;
}

int FriendsManager::findUserIndex(const std::string& username) const
{
    return userLookup.find(username);
}

bool FriendsManager::areFriends(int a, int b) const
{
    if (a < 0 || b < 0 || a >= userCount || b >= userCount)
        return false;
    return contains(friends[a], b);
}

bool FriendsManager::sendRequest(int fromIndex, const std::string& toUsername)
{
    int toIndex = findUserIndex(toUsername);
    if (fromIndex < 0 || toIndex < 0)
        return false;
    if (fromIndex == toIndex)
        return false;
    if (areFriends(fromIndex, toIndex))
        return false;
    if (contains(pending[toIndex], fromIndex))
        return false;
    if (contains(pending[fromIndex], toIndex))
    {
        removeValue(pending[fromIndex], toIndex);
        return addFriendship(fromIndex, toIndex);
    }

    appendUnique(pending[toIndex], fromIndex);
    return true;
}

bool FriendsManager::acceptRequest(int targetIndex, int requestPosition)
{
    if (targetIndex < 0 || targetIndex >= userCount)
        return false;

    int senderIndex = -1;
    if (!removeAt(pending[targetIndex], requestPosition, senderIndex))
        return false;

    return addFriendship(targetIndex, senderIndex);
}

bool FriendsManager::rejectRequest(int targetIndex, int requestPosition)
{
    if (targetIndex < 0 || targetIndex >= userCount)
        return false;

    int removedValue = -1;
    return removeAt(pending[targetIndex], requestPosition, removedValue);
}

int FriendsManager::getFriendCount(int userIndex) const
{
    if (userIndex < 0 || userIndex >= userCount)
        return 0;
    return getCount(friends[userIndex]);
}

int FriendsManager::getPendingCount(int userIndex) const
{
    if (userIndex < 0 || userIndex >= userCount)
        return 0;
    return getCount(pending[userIndex]);
}

int FriendsManager::getFriendIndexAt(int userIndex, int position) const
{
    if (userIndex < 0 || userIndex >= userCount)
        return -1;
    return getValueAt(friends[userIndex], position);
}

int FriendsManager::getPendingIndexAt(int userIndex, int position) const
{
    if (userIndex < 0 || userIndex >= userCount)
        return -1;
    return getValueAt(pending[userIndex], position);
}
