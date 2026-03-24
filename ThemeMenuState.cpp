#include "ThemeMenuState.h"
#include "Game.h"
#include "GameData.h"
#include "MainMenuState.h"
#include "Inventory.h"
#include "UIStyle.h"

namespace
{
    void setTextTopLeft(sf::Text& text, float x, float y)
    {
        text.setPosition(x, y);
    }
}

ThemeMenuState::ThemeMenuState(Game& game, GameData& data)
    : State(game, data), animTime(0.f), themeCount(0), hoveredCard(-1)
{

    titleText.setFont(ui::headingFont());
    titleText.setCharacterSize(30);
    titleText.setString("Themes & Visual Style");
    ui::centerText(titleText, sf::FloatRect(0.f, 22.f, static_cast<float>(game.getWindow().getSize().x), 36.f));

    helperText.setFont(ui::bodyFont());
    helperText.setCharacterSize(15);
    helperText.setString("Click a card or press 1-0. Selected theme changes the menus and the board.");
    ui::centerText(helperText, sf::FloatRect(0.f, 62.f, static_cast<float>(game.getWindow().getSize().x), 20.f));

    statusText.setFont(ui::bodyFont());
    statusText.setCharacterSize(15);
    statusText.setString(data.inventory ? "Current theme: " + data.inventory->getCurrentThemeName() : std::string("Current theme: Classic"));
    ui::centerText(statusText, sf::FloatRect(0.f, 404.f, static_cast<float>(game.getWindow().getSize().x), 20.f));

    rebuildCards();
}

void ThemeMenuState::rebuildCards()
{
    Theme themes[16];
    themeCount = 0;
    if (data.inventory)
        data.inventory->getAllThemes(themes, themeCount);
    if (themeCount > MAX_THEME_CARDS)
        themeCount = MAX_THEME_CARDS;

    const float left = 38.f;
    const float top = 92.f;
    const float cardW = 322.f;
    const float cardH = 58.f;
    const float gapX = 22.f;
    const float gapY = 12.f;

    for (int i = 0; i < themeCount; ++i)
    {
        int col = i % 2;
        int row = i / 2;
        float x = left + col * (cardW + gapX);
        float y = top + row * (cardH + gapY);

        cards[i].setPosition(x, y);
        cards[i].setSize(sf::Vector2f(cardW, cardH));
        cards[i].setOutlineThickness(2.f);

        themeIDs[i] = themes[i].themeID;

        cardTitles[i].setFont(ui::headingFont());
        cardTitles[i].setCharacterSize(17);
        cardTitles[i].setString(std::to_string(themes[i].themeID % 10) + "  " + themes[i].themeName);
        setTextTopLeft(cardTitles[i], x + 16.f, y + 8.f);

        cardDescriptions[i].setFont(ui::bodyFont());
        cardDescriptions[i].setCharacterSize(12);
        cardDescriptions[i].setString(themes[i].description);
        ui::truncateTextToWidth(cardDescriptions[i], cardW - 124.f);
        setTextTopLeft(cardDescriptions[i], x + 16.f, y + 33.f);
    }
}

void ThemeMenuState::applyThemeByIndex(int index)
{
    if (index < 0 || index >= themeCount || !data.inventory)
        return;

    int themeID = themeIDs[index];
    if (data.inventory->applyTheme(themeID))
    {
        if (data.currentPlayer)
            data.inventory->saveThemePreference(data.currentPlayer->username, themeID);
        statusText.setString("Current theme: " + data.inventory->getCurrentThemeName());
        ui::centerText(statusText, sf::FloatRect(0.f, 408.f, static_cast<float>(game.getWindow().getSize().x), 20.f));
    }
}

void ThemeMenuState::handleEvent(sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        game.changeState(new MainMenuState(game, data));
        return;
    }

    if (event.type == sf::Event::MouseMoved)
    {
        sf::Vector2f worldPos = game.getWindow().mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        hoveredCard = -1;
        for (int i = 0; i < themeCount; ++i)
        {
            if (cards[i].getGlobalBounds().contains(worldPos))
            {
                hoveredCard = i;
                break;
            }
        }
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f worldPos = game.getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        for (int i = 0; i < themeCount; ++i)
        {
            if (cards[i].getGlobalBounds().contains(worldPos))
            {
                applyThemeByIndex(i);
                return;
            }
        }
    }

    if (event.type != sf::Event::KeyPressed)
        return;

    int themeID = -1;
    switch (event.key.code)
    {
    case sf::Keyboard::Num1: themeID = 1; break;
    case sf::Keyboard::Num2: themeID = 2; break;
    case sf::Keyboard::Num3: themeID = 3; break;
    case sf::Keyboard::Num4: themeID = 4; break;
    case sf::Keyboard::Num5: themeID = 5; break;
    case sf::Keyboard::Num6: themeID = 6; break;
    case sf::Keyboard::Num7: themeID = 7; break;
    case sf::Keyboard::Num8: themeID = 8; break;
    case sf::Keyboard::Num9: themeID = 9; break;
    case sf::Keyboard::Num0: themeID = 10; break;
    default: break;
    }

    if (themeID != -1)
    {
        for (int i = 0; i < themeCount; ++i)
        {
            if (themeIDs[i] == themeID)
            {
                applyThemeByIndex(i);
                return;
            }
        }
    }
}

void ThemeMenuState::update(float dt)
{
    animTime += dt;
}

void ThemeMenuState::render(sf::RenderWindow& window)
{
    ui::Palette base = ui::getPalette(data.inventory ? data.inventory->getCurrentThemeID() : 1);
    ui::drawBackdrop(window, base, animTime);
    ui::drawPanel(window, sf::FloatRect(18.f, 14.f, 684.f, 422.f), base, true);
    ui::drawHeader(window, titleText, &helperText, base);

    const int activeTheme = data.inventory ? data.inventory->getCurrentThemeID() : 1;

    for (int i = 0; i < themeCount; ++i)
    {
        ui::Palette p = ui::getPalette(themeIDs[i]);
        bool selected = (themeIDs[i] == activeTheme);
        bool hovered = (i == hoveredCard);

        cards[i].setFillColor(hovered ? ui::withAlpha(p.panelFill, 245) : ui::withAlpha(p.panelFill, 228));
        cards[i].setOutlineColor(selected ? p.accent : (hovered ? p.panelOutline : ui::withAlpha(p.panelOutline, 180)));
        cards[i].setOutlineThickness(selected ? 3.f : 2.f);

        window.draw(cards[i]);

        sf::RectangleShape swatch(sf::Vector2f(16.f, 16.f));
        swatch.setPosition(cards[i].getPosition().x + cards[i].getSize().x - 74.f, cards[i].getPosition().y + 9.f);
        swatch.setFillColor(p.tileFilledInner);
        swatch.setOutlineThickness(1.f);
        swatch.setOutlineColor(p.tileBorder);
        window.draw(swatch);
        swatch.move(20.f, 0.f);
        swatch.setFillColor(p.trail1Inner);
        swatch.setOutlineColor(p.trail1Outer);
        window.draw(swatch);
        swatch.move(20.f, 0.f);
        swatch.setFillColor(p.boardBack);
        swatch.setOutlineColor(p.boardGrid);
        window.draw(swatch);

        cardTitles[i].setFillColor(selected ? p.textPrimary : base.textPrimary);
        cardDescriptions[i].setFillColor(selected ? p.textSecondary : base.textSecondary);
        window.draw(cardTitles[i]);
        window.draw(cardDescriptions[i]);
    }

    statusText.setFillColor(base.accent);
    window.draw(statusText);
}
