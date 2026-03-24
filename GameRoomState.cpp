#include "GameRoomState.h"
#include "Game.h"
#include "MainMenuState.h"
#include "GameState.h"
#include "LoginState.h"
#include "UIStyle.h"

#include <sstream>

namespace
{
    const int kModeValues[3] = { 4, 6, 8 };

    void setButtonRect(sf::RectangleShape& box, sf::Text& label, const std::string& text,
                       const sf::FloatRect& rect, unsigned int charSize)
    {
        ui::initializeButton(box, label, ui::bodyFont(), text, rect, charSize);
    }
}

GameRoomState::GameRoomState(Game& game, GameData& data)
    : State(game, data), playerTarget(4), hoveredIndex(-1)
{
    titleText.setFont(ui::headingFont());
    titleText.setCharacterSize(28);
    titleText.setString("Matchmaking Room");
    ui::centerText(titleText, sf::FloatRect(28.f, 22.f, 644.f, 34.f));

    infoText.setFont(ui::bodyFont());
    infoText.setCharacterSize(15);
    infoText.setPosition(54.f, 76.f);
    infoText.setString("Join Queue adds the current signed-in player. Add Another lets you sign in more local players for the 4-player bracket.");
    ui::wrapText(infoText, 380.f);

    bracketText.setFont(ui::bodyFont());
    bracketText.setCharacterSize(13);
    bracketText.setPosition(488.f, 78.f);

    legendText.setFont(ui::bodyFont());
    legendText.setCharacterSize(12);
    legendText.setPosition(54.f, 392.f);
    legendText.setString("Tournament order: Match 1 uses the top two players. Match 2 uses the remaining two players. The final match is between both winners.");
    ui::wrapText(legendText, 610.f);

    queueText.setFont(ui::bodyFont());
    queueText.setCharacterSize(14);
    queueText.setPosition(54.f, 248.f);

    matchText.setFont(ui::bodyFont());
    matchText.setCharacterSize(14);
    matchText.setPosition(412.f, 248.f);

    for (int i = 0; i < 3; ++i)
    {
        setButtonRect(modeButtons[i], modeLabels[i], std::to_string(kModeValues[i]) + " Players",
                      sf::FloatRect(54.f + i * 158.f, 132.f, 146.f, 38.f), 18);
    }

    setButtonRect(joinButton, joinLabel, "Join Queue", sf::FloatRect(54.f, 182.f, 156.f, 42.f), 18);
    setButtonRect(addButton, addLabel, "Add Another", sf::FloatRect(220.f, 182.f, 156.f, 42.f), 18);
    setButtonRect(startButton, startLabel, "Start Bracket", sf::FloatRect(430.f, 182.f, 188.f, 42.f), 18);
    setButtonRect(backButton, backLabel, "Back", sf::FloatRect(54.f, 346.f, 138.f, 38.f), 18);

    rebuildQueueAndMatches();
}

bool GameRoomState::isMouseOver(const sf::RectangleShape& rect, const sf::Vector2f& point) const
{
    return rect.getGlobalBounds().contains(point);
}

void GameRoomState::handleClickAt(const sf::Vector2f& point)
{
    if (isMouseOver(backButton, point))
    {
        game.changeState(new MainMenuState(game, data));
        return;
    }

    for (int i = 0; i < 3; ++i)
    {
        if (!isMouseOver(modeButtons[i], point))
            continue;

        if (kModeValues[i] != 4)
        {
            infoText.setString("This version currently supports the 4-player bracket only.");
            ui::wrapText(infoText, 380.f);
            return;
        }

        playerTarget = 4;
        infoText.setString("4-player bracket selected. Add the current player first, then sign in three more players and start the tournament.");
        ui::wrapText(infoText, 380.f);
        return;
    }

    if (isMouseOver(joinButton, point))
    {
        Player* session = data.sessionPlayer ? data.sessionPlayer : data.currentPlayer;
        if (!session)
        {
            infoText.setString("Please sign in before joining the queue.");
            ui::wrapText(infoText, 380.f);
            return;
        }
        if (data.matchQueue.containsPlayer(session))
        {
            infoText.setString("The current signed-in player is already in the queue.");
            ui::wrapText(infoText, 380.f);
            return;
        }
        data.matchQueue.push(session, session->totalPoints);
        infoText.setString("Added " + session->username + " to the queue.");
        ui::wrapText(infoText, 380.f);
        rebuildQueueAndMatches();
        return;
    }

    if (isMouseOver(addButton, point))
    {
        game.changeState(new LoginState(game, data, LOGIN_FOR_ROOM));
        return;
    }

    if (isMouseOver(startButton, point))
    {
        if (playerTarget != 4)
        {
            infoText.setString("Only the 4-player bracket is enabled in this version.");
            ui::wrapText(infoText, 380.f);
            return;
        }

        MMEntry snapshot[MatchmakingQueue::MAX_WAITING];
        int count = 0;
        data.matchQueue.getSnapshot(snapshot, count);
        if (count < 4)
        {
            std::ostringstream oss;
            oss << "Four queued players are required before the bracket can start. Current queue: " << count << ".";
            infoText.setString(oss.str());
            ui::wrapText(infoText, 380.f);
            return;
        }

        startTournamentFromQueue();
        return;
    }
}

void GameRoomState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        game.changeState(new MainMenuState(game, data));
        return;
    }

    if (event.type == sf::Event::MouseMoved)
    {
        sf::Vector2f worldPos = game.getWindow().mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        hoveredIndex = -1;
        if (isMouseOver(joinButton, worldPos)) hoveredIndex = 10;
        else if (isMouseOver(addButton, worldPos)) hoveredIndex = 11;
        else if (isMouseOver(startButton, worldPos)) hoveredIndex = 12;
        else if (isMouseOver(backButton, worldPos)) hoveredIndex = 13;
        else
        {
            for (int i = 0; i < 3; ++i)
            {
                if (isMouseOver(modeButtons[i], worldPos))
                {
                    hoveredIndex = i;
                    break;
                }
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f worldPos = game.getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        handleClickAt(worldPos);
    }
}

void GameRoomState::update(float)
{
}

void GameRoomState::render(sf::RenderWindow& window)
{
    ui::Palette palette = ui::getPalette(data.inventory ? data.inventory->getCurrentThemeID() : 1);
    ui::drawBackdrop(window, palette, 0.f);
    ui::drawPanel(window, sf::FloatRect(26.f, 18.f, 668.f, 414.f), palette, true);
    ui::drawHeader(window, titleText, nullptr, palette);

    infoText.setFillColor(palette.textSecondary);
    legendText.setFillColor(palette.textSecondary);
    queueText.setFillColor(palette.textPrimary);
    matchText.setFillColor(palette.textPrimary);
    bracketText.setFillColor(palette.accent);

    bracketText.setCharacterSize(13);
    ui::truncateTextToWidth(bracketText, 172.f);

    window.draw(infoText);
    window.draw(legendText);
    window.draw(bracketText);

    for (int i = 0; i < 3; ++i)
    {
        bool selected = (kModeValues[i] == playerTarget);
        bool enabled = (kModeValues[i] == 4);
        modeButtons[i].setFillColor(selected ? palette.buttonHover : palette.buttonFill);
        modeButtons[i].setOutlineColor(selected ? palette.accent : palette.panelOutline);
        if (!enabled)
        {
            modeButtons[i].setFillColor(ui::darken(palette.buttonFill, 8));
            modeButtons[i].setOutlineColor(ui::withAlpha(palette.panelOutline, 120));
            modeLabels[i].setFillColor(ui::withAlpha(palette.textSecondary, 170));
        }
        else
        {
            modeLabels[i].setFillColor(palette.textPrimary);
            if (hoveredIndex == i)
            {
                modeButtons[i].setFillColor(palette.buttonHover);
                modeButtons[i].setOutlineColor(palette.accent);
            }
        }
        window.draw(modeButtons[i]);
        window.draw(modeLabels[i]);
    }

    ui::styleButton(joinButton, joinLabel, palette, hoveredIndex == 10, false);
    ui::styleButton(addButton, addLabel, palette, hoveredIndex == 11, false);
    ui::styleButton(startButton, startLabel, palette, hoveredIndex == 12, false);
    ui::styleButton(backButton, backLabel, palette, hoveredIndex == 13, true);

    window.draw(joinButton); window.draw(joinLabel);
    window.draw(addButton);  window.draw(addLabel);
    window.draw(startButton);window.draw(startLabel);
    window.draw(backButton); window.draw(backLabel);

    ui::drawPanel(window, sf::FloatRect(42.f, 234.f, 320.f, 116.f), palette, false, 5.f);
    ui::drawPanel(window, sf::FloatRect(398.f, 234.f, 266.f, 116.f), palette, false, 5.f);
    window.draw(queueText);
    window.draw(matchText);
}

void GameRoomState::startTournamentFromQueue()
{
    MMEntry snapshot[MatchmakingQueue::MAX_WAITING];
    int count = 0;
    data.matchQueue.getSnapshot(snapshot, count);
    if (count < 4)
        return;

    data.matchmakingActive = true;
    data.roomPlayerTarget = 4;
    data.roomPlayerCount = 4;
    data.roomWinnerCount = 0;
    data.roomCurrentMatchStart = 0;
    data.lastWinner = nullptr;

    for (int i = 0; i < 4; ++i)
    {
        data.roomPlayers[i] = snapshot[i].player;
        data.roomWinners[i] = nullptr;
    }
    for (int i = 4; i < 8; ++i)
    {
        data.roomPlayers[i] = nullptr;
        data.roomWinners[i] = nullptr;
    }

    data.matchQueue.clear();

    data.currentPlayer = data.roomPlayers[0];
    data.coopPlayer = data.roomPlayers[1];
    game.changeState(new GameState(game, data, 2));
}

void GameRoomState::rebuildQueueAndMatches()
{
    MMEntry snapshot[MatchmakingQueue::MAX_WAITING];
    int count = 0;
    data.matchQueue.getSnapshot(snapshot, count);

    std::ostringstream qss;
    qss << "Queue  " << count << "/4\n";
    if (count == 0)
    {
        qss << "[empty]\nJoin Queue adds the signed-in player.";
    }
    else
    {
        for (int i = 0; i < count && i < 4; ++i)
        {
            Player* p = snapshot[i].player;
            if (!p) continue;
            qss << i + 1 << ". " << p->username << " - " << p->totalPoints << " pts\n";
        }
        if (count > 4)
            qss << "+" << (count - 4) << " extra player(s) waiting\n";
    }
    queueText.setString(qss.str());
    ui::wrapText(queueText, 288.f);

    std::ostringstream mss;
    mss << "Bracket Preview\n";
    if (count >= 4)
    {
        Player* p1 = snapshot[0].player;
        Player* p2 = snapshot[1].player;
        Player* p3 = snapshot[2].player;
        Player* p4 = snapshot[3].player;
        mss << "Match 1: " << (p1 ? p1->username : std::string("?")) << " vs "
            << (p2 ? p2->username : std::string("?")) << "\n";
        mss << "Match 2: " << (p3 ? p3->username : std::string("?")) << " vs "
            << (p4 ? p4->username : std::string("?")) << "\n";
        mss << "Final: winners of both matches";
    }
    else
    {
        mss << "Add four queued players to create the bracket.";
    }
    matchText.setString(mss.str());
    ui::wrapText(matchText, 232.f);

    if (!data.matchmakingActive && data.lastWinner && data.roomPlayerCount == 1)
    {
        bracketText.setString("Latest winner: " + data.lastWinner->username);
    }
    else
    {
        Player* session = data.sessionPlayer ? data.sessionPlayer : data.currentPlayer;
        bracketText.setString("Signed in: " + std::string(session ? session->username : "none"));
    }
}
