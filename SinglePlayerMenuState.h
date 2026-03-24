#pragma once

#include "State.h"
#include "SaveGame.h"
#include <SFML/Graphics.hpp>
#include <string>

struct SPButton
{
    sf::RectangleShape box;
    sf::Text label;
};

class SinglePlayerMenuState : public State
{
public:
    SinglePlayerMenuState(Game& game, GameData& data);

    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text titleText;
    sf::Text subtitleText;
    sf::Text promptText;
    sf::Text statusText;
    sf::Text saveListText;
    float animTime;

    static const int BUTTON_COUNT = 3;
    static const int MAX_VISIBLE_SAVES = 10;
    SPButton buttons[BUTTON_COUNT];
    int hoveredIndex;

    bool showingSaveList;
    SaveGameMeta availableSaves[MAX_VISIBLE_SAVES];
    int saveCount;
    int selectedSave;
    sf::FloatRect savePanelRect;
    sf::FloatRect saveRowRects[MAX_VISIBLE_SAVES];

    void setupButton(int index, const char* text, float x, float y);
    void handleClick(int index);
    void refreshSaveList();
    void rebuildSaveListText();
    void tryLoadSelectedSave();
};
