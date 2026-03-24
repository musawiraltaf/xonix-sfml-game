#include "UserDataIO.h"
#include "GameData.h"
#include "Player.h"

#include <fstream>
#include <sstream>

bool loadUsersFromFile(GameData& data, const std::string& filename)
{
    data.userCount = 0;

    std::ifstream in(filename.c_str());
    if (!in.is_open())
        return false;

    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty())
            continue;

        if (data.userCount >= MAX_PLAYERS)
            break;

        std::istringstream iss(line);
        std::string username;
        std::string password;
        int highScore = 0;
        int totalPoints = 0;
        int wins = 0;
        int losses = 0;

        if (!(iss >> username >> password >> highScore))
            continue;

        if (!(iss >> totalPoints)) totalPoints = 0;
        if (!(iss >> wins)) wins = 0;
        if (!(iss >> losses)) losses = 0;

        Player& player = data.users[data.userCount++];
        player = Player();
        player.username = username;
        player.password = password;
        player.highScore = highScore;
        player.totalPoints = totalPoints;
        player.wins = wins;
        player.losses = losses;
        player.resetPerSession();
    }

    return true;
}

bool saveUsersToFile(const GameData& data, const std::string& filename)
{
    std::ofstream out(filename.c_str(), std::ios::trunc);
    if (!out.is_open())
        return false;

    for (int i = 0; i < data.userCount; ++i)
    {
        const Player& player = data.users[i];
        out << player.username << ' '
            << player.password << ' '
            << player.highScore << ' '
            << player.totalPoints << ' '
            << player.wins << ' '
            << player.losses << '\n';
    }

    return true;
}

int findUserIndex(const GameData& data, const std::string& username, const std::string& password)
{
    for (int i = 0; i < data.userCount; ++i)
    {
        if (data.users[i].username == username && data.users[i].password == password)
            return i;
    }
    return -1;
}

int findUserIndexByName(const GameData& data, const std::string& username)
{
    for (int i = 0; i < data.userCount; ++i)
    {
        if (data.users[i].username == username)
            return i;
    }
    return -1;
}

int findPlayerPointerIndex(const GameData& data, const Player* player)
{
    if (!player)
        return -1;

    for (int i = 0; i < data.userCount; ++i)
    {
        if (&data.users[i] == player)
            return i;
    }
    return -1;
}
