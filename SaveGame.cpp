#include "SaveGame.h"

#include <ctime>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace
{
    std::string makeFilename(const std::string& saveID)
    {
        return std::string("save_") + saveID + ".txt";
    }

    bool parseEpochLine(const std::string& line, long long& outEpoch)
    {
        if (line.empty())
            return false;
        std::istringstream iss(line);
        long long epoch = 0;
        char extra = 0;
        if (!(iss >> epoch))
            return false;
        if (iss >> extra)
            return false;
        outEpoch = epoch;
        return true;
    }

    long long inferEpochFromSaveID(const std::string& saveID)
    {
        std::size_t pos = saveID.find_last_of('_');
        if (pos == std::string::npos)
            return 0;

        std::string tail = saveID.substr(pos + 1);
        if (tail.size() == 15 && tail[8] == '_')
        {
            std::tm tmValue = {};
            tmValue.tm_year = std::atoi(tail.substr(0, 4).c_str()) - 1900;
            tmValue.tm_mon = std::atoi(tail.substr(4, 2).c_str()) - 1;
            tmValue.tm_mday = std::atoi(tail.substr(6, 2).c_str());
            tmValue.tm_hour = std::atoi(tail.substr(9, 2).c_str());
            tmValue.tm_min = std::atoi(tail.substr(11, 2).c_str());
            tmValue.tm_sec = std::atoi(tail.substr(13, 2).c_str());
            return static_cast<long long>(std::mktime(&tmValue));
        }

        for (char ch : tail)
            if (ch < '0' || ch > '9')
                return 0;
        return std::atoll(tail.c_str());
    }

    bool readSaveHeader(const std::filesystem::path& path, SaveGameMeta& meta)
    {
        std::ifstream in(path.string().c_str());
        if (!in.is_open())
            return false;

        std::getline(in, meta.saveID);
        std::getline(in, meta.username);
        if (meta.saveID.empty() || meta.username.empty())
            return false;

        std::string thirdLine;
        std::getline(in, thirdLine);
        long long epoch = 0;
        if (parseEpochLine(thirdLine, epoch))
        {
            meta.savedAtEpoch = epoch;
            if (!(in >> meta.level))
                return false;
            // rewind to read full gameplay header cleanly
            in.clear();
            in.seekg(0, std::ios::beg);
            std::string throwaway;
            std::getline(in, throwaway);
            std::getline(in, throwaway);
            std::getline(in, throwaway);
            int gameMode = 1, enemyCount = 0, targetCapture = 0;
            if (!(in >> gameMode >> meta.level >> enemyCount >> targetCapture))
                return false;
        }
        else
        {
            meta.savedAtEpoch = inferEpochFromSaveID(meta.saveID);
            std::istringstream iss(thirdLine);
            int gameMode = 1, enemyCount = 0, targetCapture = 0;
            if (!(iss >> gameMode >> meta.level >> enemyCount >> targetCapture))
                return false;
        }

        // score line is always several lines later; easiest is load the full record.
        SaveGameRecord full;
        if (!loadGameFromDisk(meta.saveID, full))
            return false;
        meta.level = full.level;
        meta.score = full.p1Score;
        meta.timestampText = formatSaveTimestamp(full.savedAtEpoch);
        return true;
    }
}

SaveGameRecord::SaveGameRecord()
    : saveID(), username(), savedAtEpoch(0), gameMode(1), level(1), enemyCount(4), targetCapturePercent(75),
      x1(10), y1(0), dx1(0), dy1(0), x2(0), y2(0), dx2(0), dy2(0),
      p1Alive(1), p2Alive(0), p1HasTrail(0), p2HasTrail(0),
      p1Score(0), p1PowerUps(0), p1BonusCount(0), p1BonusThreshold(10), p1NextPowerupScore(50),
      p2Score(0), p2PowerUps(0), p2BonusCount(0), p2BonusThreshold(10), p2NextPowerupScore(50),
      freezeActive(0), freezeTimer(0.f), frozenPlayer(0), delay(0.04f), gameTime(0.f)
{
    for (int i = 0; i < 10; ++i)
        enemyX[i] = enemyY[i] = enemyDx[i] = enemyDy[i] = 0;

    for (int y = 0; y < 25; ++y)
        for (int x = 0; x < 40; ++x)
            grid[y][x] = 0;
}

std::string generateSaveID(const std::string& username)
{
    std::time_t now = std::time(nullptr);
    std::tm localTm = {};
#ifdef _WIN32
    localtime_s(&localTm, &now);
#else
    localTm = *std::localtime(&now);
#endif
    char buffer[32] = {0};
    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", &localTm);
    return username + "_" + buffer;
}

std::string formatSaveTimestamp(long long epochSeconds)
{
    if (epochSeconds <= 0)
        return "Unknown time";

    std::time_t raw = static_cast<std::time_t>(epochSeconds);
    std::tm localTm = {};
#ifdef _WIN32
    localtime_s(&localTm, &raw);
#else
    localTm = *std::localtime(&raw);
#endif
    char buffer[48] = {0};
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTm);
    return buffer;
}

bool saveGameToDisk(const SaveGameRecord& record)
{
    std::ofstream out(makeFilename(record.saveID).c_str(), std::ios::trunc);
    if (!out.is_open())
        return false;

    out << record.saveID << '\n';
    out << record.username << '\n';
    out << record.savedAtEpoch << '\n';
    out << record.gameMode << ' ' << record.level << ' ' << record.enemyCount << ' ' << record.targetCapturePercent << '\n';
    out << record.x1 << ' ' << record.y1 << ' ' << record.dx1 << ' ' << record.dy1 << ' '
        << record.x2 << ' ' << record.y2 << ' ' << record.dx2 << ' ' << record.dy2 << '\n';
    out << record.p1Alive << ' ' << record.p2Alive << ' ' << record.p1HasTrail << ' ' << record.p2HasTrail << '\n';
    out << record.p1Score << ' ' << record.p1PowerUps << ' ' << record.p1BonusCount << ' ' << record.p1BonusThreshold << ' ' << record.p1NextPowerupScore << '\n';
    out << record.p2Score << ' ' << record.p2PowerUps << ' ' << record.p2BonusCount << ' ' << record.p2BonusThreshold << ' ' << record.p2NextPowerupScore << '\n';
    out << record.freezeActive << ' ' << record.freezeTimer << ' ' << record.frozenPlayer << ' ' << record.delay << ' ' << record.gameTime << '\n';

    for (int i = 0; i < 10; ++i)
        out << record.enemyX[i] << ' ' << record.enemyY[i] << ' ' << record.enemyDx[i] << ' ' << record.enemyDy[i] << '\n';

    for (int y = 0; y < 25; ++y)
    {
        for (int x = 0; x < 40; ++x)
        {
            if (x)
                out << ' ';
            out << record.grid[y][x];
        }
        out << '\n';
    }

    return true;
}

bool loadGameFromDisk(const std::string& saveID, SaveGameRecord& outRecord)
{
    std::ifstream in(makeFilename(saveID).c_str());
    if (!in.is_open())
        return false;

    std::getline(in, outRecord.saveID);
    std::getline(in, outRecord.username);

    std::string thirdLine;
    std::getline(in, thirdLine);

    long long epoch = 0;
    if (parseEpochLine(thirdLine, epoch))
    {
        outRecord.savedAtEpoch = epoch;
        if (!(in >> outRecord.gameMode >> outRecord.level >> outRecord.enemyCount >> outRecord.targetCapturePercent)) return false;
    }
    else
    {
        outRecord.savedAtEpoch = inferEpochFromSaveID(outRecord.saveID);
        std::istringstream iss(thirdLine);
        if (!(iss >> outRecord.gameMode >> outRecord.level >> outRecord.enemyCount >> outRecord.targetCapturePercent)) return false;
    }

    if (!(in >> outRecord.x1 >> outRecord.y1 >> outRecord.dx1 >> outRecord.dy1
          >> outRecord.x2 >> outRecord.y2 >> outRecord.dx2 >> outRecord.dy2)) return false;
    if (!(in >> outRecord.p1Alive >> outRecord.p2Alive >> outRecord.p1HasTrail >> outRecord.p2HasTrail)) return false;
    if (!(in >> outRecord.p1Score >> outRecord.p1PowerUps >> outRecord.p1BonusCount >> outRecord.p1BonusThreshold >> outRecord.p1NextPowerupScore)) return false;
    if (!(in >> outRecord.p2Score >> outRecord.p2PowerUps >> outRecord.p2BonusCount >> outRecord.p2BonusThreshold >> outRecord.p2NextPowerupScore)) return false;
    if (!(in >> outRecord.freezeActive >> outRecord.freezeTimer >> outRecord.frozenPlayer >> outRecord.delay >> outRecord.gameTime)) return false;

    for (int i = 0; i < 10; ++i)
    {
        if (!(in >> outRecord.enemyX[i] >> outRecord.enemyY[i] >> outRecord.enemyDx[i] >> outRecord.enemyDy[i]))
            return false;
    }

    for (int y = 0; y < 25; ++y)
    {
        for (int x = 0; x < 40; ++x)
        {
            if (!(in >> outRecord.grid[y][x]))
                return false;
        }
    }

    return true;
}

int listSaveGamesForUser(const std::string& username, SaveGameMeta* outList, int maxCount)
{
    if (!outList || maxCount <= 0)
        return 0;

    const int internalMax = 128;
    SaveGameMeta temp[internalMax];
    int count = 0;

    std::filesystem::path base = std::filesystem::current_path();
    for (const auto& entry : std::filesystem::directory_iterator(base))
    {
        if (!entry.is_regular_file())
            continue;

        std::string name = entry.path().filename().string();
        if (name.size() <= 9)
            continue;
        if (name.rfind("save_", 0) != 0)
            continue;
        if (entry.path().extension().string() != ".txt")
            continue;

        SaveGameMeta meta;
        if (!readSaveHeader(entry.path(), meta))
            continue;
        if (meta.username != username)
            continue;
        if (count < internalMax)
            temp[count++] = meta;
    }

    for (int i = 0; i < count; ++i)
    {
        for (int j = i + 1; j < count; ++j)
        {
            if (temp[j].savedAtEpoch > temp[i].savedAtEpoch)
            {
                SaveGameMeta swap = temp[i];
                temp[i] = temp[j];
                temp[j] = swap;
            }
        }
    }

    int finalCount = count < maxCount ? count : maxCount;
    for (int i = 0; i < finalCount; ++i)
        outList[i] = temp[i];
    return finalCount;
}
