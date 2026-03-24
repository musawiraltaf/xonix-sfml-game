#pragma once

#include <string>

struct GameData;
struct Player;

bool loadUsersFromFile(GameData& data, const std::string& filename = "users.txt");
bool saveUsersToFile(const GameData& data, const std::string& filename = "users.txt");
int findUserIndex(const GameData& data, const std::string& username, const std::string& password);
int findUserIndexByName(const GameData& data, const std::string& username);
int findPlayerPointerIndex(const GameData& data, const Player* player);
