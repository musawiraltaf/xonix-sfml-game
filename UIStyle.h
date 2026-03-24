#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include <string>
#include <vector>

namespace ui
{
    struct Palette
    {
        sf::Color bgTop;
        sf::Color bgBottom;
        sf::Color panelFill;
        sf::Color panelOutline;
        sf::Color panelInner;
        sf::Color buttonFill;
        sf::Color buttonHover;
        sf::Color buttonDanger;
        sf::Color textPrimary;
        sf::Color textSecondary;
        sf::Color accent;
        sf::Color accentSoft;
        sf::Color success;
        sf::Color warning;
        sf::Color danger;
        sf::Color boardBack;
        sf::Color boardGrid;
        sf::Color tileFilledOuter;
        sf::Color tileFilledInner;
        sf::Color tileBorder;
        sf::Color trail1Outer;
        sf::Color trail1Inner;
        sf::Color trail2Outer;
        sf::Color trail2Inner;
        sf::Color player1;
        sf::Color player2;
        sf::Color enemy;
    };

    inline sf::VertexArray makeGradientQuad(const sf::FloatRect& rect, const sf::Color& top, const sf::Color& bottom)
    {
        sf::VertexArray quad(sf::Quads, 4);
        quad[0].position = sf::Vector2f(rect.left, rect.top);
        quad[1].position = sf::Vector2f(rect.left + rect.width, rect.top);
        quad[2].position = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
        quad[3].position = sf::Vector2f(rect.left, rect.top + rect.height);
        quad[0].color = top;
        quad[1].color = top;
        quad[2].color = bottom;
        quad[3].color = bottom;
        return quad;
    }

    inline sf::Color brighten(const sf::Color& color, int delta)
    {
        int r = color.r + delta; if (r > 255) r = 255;
        int g = color.g + delta; if (g > 255) g = 255;
        int b = color.b + delta; if (b > 255) b = 255;
        return sf::Color(static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b), color.a);
    }

    inline sf::Color darken(const sf::Color& color, int delta)
    {
        int r = color.r - delta; if (r < 0) r = 0;
        int g = color.g - delta; if (g < 0) g = 0;
        int b = color.b - delta; if (b < 0) b = 0;
        return sf::Color(static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b), color.a);
    }

    inline sf::Color withAlpha(const sf::Color& color, sf::Uint8 alpha)
    {
        return sf::Color(color.r, color.g, color.b, alpha);
    }

    inline bool tryLoadFont(sf::Font& font, const std::vector<std::string>& candidates)
    {
        for (std::size_t i = 0; i < candidates.size(); ++i)
        {
            if (font.loadFromFile(candidates[i]))
                return true;
        }
        return false;
    }

    inline const sf::Font& headingFont()
    {
        static sf::Font font;
        static bool loaded = false;
        if (!loaded)
        {
            tryLoadFont(font, {
                "ariblk.ttf",
                "C:/Windows/Fonts/ariblk.ttf",
                "C:/Windows/Fonts/Arialbd.ttf",
                "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"
            });
            loaded = true;
        }
        return font;
    }

    inline const sf::Font& bodyFont()
    {
        static sf::Font font;
        static bool loaded = false;
        if (!loaded)
        {
            if (!tryLoadFont(font, {
                "C:/Windows/Fonts/segoeui.ttf",
                "C:/Windows/Fonts/arial.ttf",
                "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
                "ariblk.ttf"
            }))
            {
                font.loadFromFile("ariblk.ttf");
            }
            loaded = true;
        }
        return font;
    }

    inline Palette getPalette(int themeID)
    {
        switch (themeID)
        {
        case 2: return { sf::Color(11, 18, 29), sf::Color(6, 10, 18), sf::Color(16, 24, 38, 236), sf::Color(86, 126, 174), sf::Color(151, 207, 255, 42), sf::Color(25, 39, 61), sf::Color(44, 70, 105), sf::Color(110, 47, 54), sf::Color(245, 249, 255), sf::Color(181, 198, 220), sf::Color(92, 191, 255), sf::Color(255, 205, 102), sf::Color(92, 209, 156), sf::Color(255, 214, 110), sf::Color(255, 112, 123), sf::Color(8, 14, 23), sf::Color(18, 35, 56), sf::Color(72, 94, 124), sf::Color(137, 175, 229), sf::Color(216, 233, 255), sf::Color(92, 191, 255), sf::Color(185, 228, 255), sf::Color(255, 173, 72), sf::Color(255, 232, 166), sf::Color(143, 238, 255), sf::Color(255, 202, 121), sf::Color(255, 255, 255) };
        case 3: return { sf::Color(5, 43, 67), sf::Color(3, 21, 38), sf::Color(8, 56, 82, 232), sf::Color(79, 184, 221), sf::Color(200, 245, 255, 34), sf::Color(13, 92, 130), sf::Color(31, 135, 177), sf::Color(176, 91, 62), sf::Color(247, 252, 255), sf::Color(201, 229, 241), sf::Color(73, 220, 255), sf::Color(255, 228, 138), sf::Color(91, 220, 180), sf::Color(255, 221, 145), sf::Color(255, 114, 114), sf::Color(5, 33, 52), sf::Color(15, 87, 123), sf::Color(52, 137, 176), sf::Color(121, 206, 236), sf::Color(221, 253, 255), sf::Color(46, 214, 255), sf::Color(214, 252, 255), sf::Color(255, 231, 149), sf::Color(255, 249, 205), sf::Color(240, 255, 198), sf::Color(119, 255, 232), sf::Color(255, 255, 255) };
        case 4: return { sf::Color(21, 34, 18), sf::Color(11, 18, 9), sf::Color(32, 53, 27, 233), sf::Color(121, 168, 100), sf::Color(192, 223, 171, 34), sf::Color(48, 81, 40), sf::Color(73, 110, 54), sf::Color(115, 67, 39), sf::Color(246, 249, 238), sf::Color(205, 219, 190), sf::Color(176, 224, 109), sf::Color(151, 203, 255), sf::Color(114, 214, 136), sf::Color(238, 206, 114), sf::Color(234, 104, 92), sf::Color(16, 28, 14), sf::Color(42, 72, 34), sf::Color(96, 140, 75), sf::Color(170, 207, 137), sf::Color(228, 247, 202), sf::Color(184, 240, 125), sf::Color(239, 255, 194), sf::Color(144, 188, 255), sf::Color(218, 235, 255), sf::Color(255, 230, 140), sf::Color(151, 226, 255), sf::Color(255, 250, 240) };
        case 5: return { sf::Color(58, 20, 53), sf::Color(23, 8, 23), sf::Color(69, 29, 67, 232), sf::Color(241, 160, 90), sf::Color(255, 225, 164, 38), sf::Color(107, 47, 82), sf::Color(146, 70, 103), sf::Color(124, 56, 45), sf::Color(255, 246, 242), sf::Color(239, 203, 188), sf::Color(255, 190, 97), sf::Color(218, 150, 255), sf::Color(122, 221, 162), sf::Color(255, 214, 120), sf::Color(255, 117, 117), sf::Color(32, 11, 30), sf::Color(79, 31, 69), sf::Color(170, 79, 102), sf::Color(242, 168, 107), sf::Color(255, 232, 184), sf::Color(255, 208, 102), sf::Color(255, 238, 176), sf::Color(215, 155, 255), sf::Color(245, 224, 255), sf::Color(255, 214, 120), sf::Color(198, 170, 255), sf::Color(255, 245, 227) };
        case 6: return { sf::Color(10, 9, 20), sf::Color(3, 2, 10), sf::Color(17, 14, 31, 234), sf::Color(216, 54, 255), sf::Color(255, 255, 255, 28), sf::Color(37, 21, 60), sf::Color(72, 29, 104), sf::Color(80, 22, 49), sf::Color(250, 251, 255), sf::Color(210, 215, 255), sf::Color(255, 0, 176), sf::Color(0, 255, 208), sf::Color(0, 255, 163), sf::Color(255, 219, 71), sf::Color(255, 89, 118), sf::Color(5, 5, 14), sf::Color(15, 20, 45), sf::Color(90, 55, 148), sf::Color(192, 111, 255), sf::Color(252, 195, 255), sf::Color(255, 0, 170), sf::Color(255, 188, 230), sf::Color(0, 255, 170), sf::Color(185, 255, 241), sf::Color(255, 255, 0), sf::Color(0, 255, 255), sf::Color(255, 255, 255) };
        case 7: return { sf::Color(42, 20, 64), sf::Color(19, 10, 33), sf::Color(52, 27, 77, 230), sf::Color(255, 112, 189), sf::Color(255, 209, 97, 28), sf::Color(82, 47, 122), sf::Color(116, 68, 162), sf::Color(103, 58, 45), sf::Color(255, 244, 246), sf::Color(230, 197, 210), sf::Color(255, 211, 98), sf::Color(255, 119, 182), sf::Color(91, 224, 207), sf::Color(255, 220, 121), sf::Color(255, 112, 141), sf::Color(23, 14, 39), sf::Color(62, 38, 98), sf::Color(150, 82, 190), sf::Color(245, 151, 214), sf::Color(255, 226, 148), sf::Color(255, 216, 95), sf::Color(255, 243, 180), sf::Color(255, 104, 169), sf::Color(255, 205, 226), sf::Color(255, 217, 88), sf::Color(255, 102, 168), sf::Color(255, 246, 210) };
        case 8: return { sf::Color(35, 35, 35), sf::Color(12, 12, 12), sf::Color(40, 40, 40, 236), sf::Color(180, 180, 180), sf::Color(255, 255, 255, 18), sf::Color(59, 59, 59), sf::Color(95, 95, 95), sf::Color(98, 98, 98), sf::Color(252, 252, 252), sf::Color(208, 208, 208), sf::Color(245, 245, 245), sf::Color(168, 168, 168), sf::Color(215, 215, 215), sf::Color(238, 238, 238), sf::Color(146, 146, 146), sf::Color(17, 17, 17), sf::Color(54, 54, 54), sf::Color(104, 104, 104), sf::Color(183, 183, 183), sf::Color(235, 235, 235), sf::Color(245, 245, 245), sf::Color(255, 255, 255), sf::Color(189, 189, 189), sf::Color(224, 224, 224), sf::Color(255, 255, 255), sf::Color(224, 224, 224), sf::Color(240, 240, 240) };
        case 9: return { sf::Color(53, 14, 8), sf::Color(16, 4, 2), sf::Color(61, 19, 10, 232), sf::Color(255, 122, 52), sf::Color(255, 224, 172, 28), sf::Color(95, 37, 20), sf::Color(142, 56, 25), sf::Color(126, 38, 26), sf::Color(255, 246, 235), sf::Color(247, 204, 170), sf::Color(255, 197, 67), sf::Color(255, 110, 36), sf::Color(255, 164, 75), sf::Color(255, 220, 103), sf::Color(255, 104, 74), sf::Color(23, 6, 3), sf::Color(64, 17, 9), sf::Color(154, 49, 24), sf::Color(239, 107, 45), sf::Color(255, 205, 124), sf::Color(255, 220, 70), sf::Color(255, 242, 145), sf::Color(255, 114, 35), sf::Color(255, 202, 132), sf::Color(255, 234, 137), sf::Color(255, 160, 74), sf::Color(255, 245, 231) };
        case 10:return { sf::Color(12, 43, 67), sf::Color(4, 16, 34), sf::Color(15, 54, 82, 230), sf::Color(152, 222, 255), sf::Color(222, 247, 255, 28), sf::Color(23, 83, 118), sf::Color(59, 132, 180), sf::Color(86, 128, 161), sf::Color(248, 252, 255), sf::Color(202, 224, 235), sf::Color(255, 255, 255), sf::Color(140, 219, 255), sf::Color(151, 235, 255), sf::Color(255, 241, 171), sf::Color(255, 122, 122), sf::Color(6, 24, 43), sf::Color(22, 80, 108), sf::Color(131, 186, 218), sf::Color(201, 239, 255), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(219, 247, 255), sf::Color(131, 216, 255), sf::Color(225, 247, 255), sf::Color(255, 255, 255), sf::Color(165, 236, 255), sf::Color(255, 255, 255) };
        default: return { sf::Color(19, 36, 63), sf::Color(6, 14, 28), sf::Color(17, 37, 65, 236), sf::Color(209, 174, 90), sf::Color(95, 157, 205, 32), sf::Color(27, 54, 90), sf::Color(41, 83, 131), sf::Color(118, 53, 50), sf::Color(247, 242, 228), sf::Color(202, 193, 171), sf::Color(248, 214, 103), sf::Color(81, 171, 212), sf::Color(113, 214, 162), sf::Color(255, 221, 122), sf::Color(255, 111, 116), sf::Color(7, 18, 34), sf::Color(27, 49, 79), sf::Color(80, 108, 144), sf::Color(171, 185, 196), sf::Color(226, 234, 240), sf::Color(253, 210, 93), sf::Color(255, 240, 175), sf::Color(104, 198, 227), sf::Color(196, 241, 255), sf::Color(253, 210, 93), sf::Color(106, 216, 185), sf::Color(255, 246, 231) };
        }
    }

    inline void centerText(sf::Text& text, const sf::FloatRect& rect)
    {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition(rect.left + (rect.width - bounds.width) / 2.f - bounds.left,
                         rect.top + (rect.height - bounds.height) / 2.f - bounds.top);
    }

    inline void wrapText(sf::Text& text, float maxWidth)
    {
        std::string source = text.getString().toAnsiString();
        std::string wrapped;
        std::string currentWord;
        std::string currentLine;

        auto commitLine = [&]() {
            wrapped += currentLine;
            wrapped += '\n';
            currentLine.clear();
        };

        for (std::size_t i = 0; i <= source.size(); ++i)
        {
            char ch = (i < source.size()) ? source[i] : ' ';
            if (ch == ' ' || ch == '\n' || i == source.size())
            {
                if (!currentWord.empty())
                {
                    std::string candidate = currentLine;
                    if (!candidate.empty())
                        candidate += ' ';
                    candidate += currentWord;

                    sf::Text measure = text;
                    measure.setString(candidate);
                    if (!currentLine.empty() && measure.getLocalBounds().width > maxWidth)
                    {
                        commitLine();
                        currentLine = currentWord;
                    }
                    else
                    {
                        if (!currentLine.empty())
                            currentLine += ' ';
                        currentLine += currentWord;
                    }
                    currentWord.clear();
                }

                if (ch == '\n')
                    commitLine();
            }
            else
            {
                currentWord += ch;
            }
        }

        wrapped += currentLine;
        text.setString(wrapped);
    }

    inline void fitTextToWidth(sf::Text& text, float maxWidth, unsigned int minCharacterSize = 12)
    {
        unsigned int size = text.getCharacterSize();
        while (size > minCharacterSize)
        {
            sf::FloatRect bounds = text.getLocalBounds();
            if (bounds.width <= maxWidth)
                break;
            --size;
            text.setCharacterSize(size);
        }
    }


    inline void truncateTextToWidth(sf::Text& text, float maxWidth, const std::string& ellipsis = "...")
    {
        std::string original = text.getString().toAnsiString();
        if (original.empty())
            return;

        sf::Text measure = text;
        measure.setString(original);
        if (measure.getLocalBounds().width <= maxWidth)
            return;

        std::string trimmed = original;
        while (!trimmed.empty())
        {
            trimmed.pop_back();
            std::string candidate = trimmed + ellipsis;
            measure.setString(candidate);
            if (measure.getLocalBounds().width <= maxWidth)
            {
                text.setString(candidate);
                return;
            }
        }

        text.setString(ellipsis);
    }

    inline void initializeButton(sf::RectangleShape& box, sf::Text& label, const sf::Font& font,
                                 const std::string& text, const sf::FloatRect& rect, unsigned int size = 22)
    {
        box.setPosition(rect.left, rect.top);
        box.setSize(sf::Vector2f(rect.width, rect.height));
        box.setOutlineThickness(2.f);
        label.setFont(font);
        label.setCharacterSize(size);
        label.setString(text);
        centerText(label, rect);
    }

    inline void styleButton(sf::RectangleShape& box, sf::Text& label, const Palette& palette,
                            bool hovered, bool danger = false)
    {
        sf::Color fill = danger ? palette.buttonDanger : palette.buttonFill;
        sf::Color outline = danger ? brighten(palette.buttonDanger, 50) : palette.panelOutline;
        if (hovered)
        {
            fill = danger ? brighten(palette.buttonDanger, 18) : palette.buttonHover;
            outline = palette.accent;
        }
        label.setFillColor(palette.textPrimary);
        box.setFillColor(fill);
        box.setOutlineColor(outline);
    }

    inline void drawInkSplatter(sf::RenderWindow& window, sf::Vector2f center, float radius, const sf::Color& color)
    {
        static const float offsets[10][3] = {
            {0.f, 0.f, 1.00f}, {-0.75f, -0.10f, 0.54f}, {0.68f, -0.18f, 0.44f},
            {-0.42f, 0.62f, 0.34f}, {0.56f, 0.58f, 0.27f}, {-1.05f, 0.28f, 0.22f},
            {1.10f, 0.24f, 0.18f}, {-0.18f, -0.98f, 0.20f}, {0.22f, 1.04f, 0.16f},
            {1.35f, -0.55f, 0.09f}
        };

        for (int i = 0; i < 10; ++i)
        {
            sf::CircleShape blot(radius * offsets[i][2], 18);
            blot.setOrigin(blot.getRadius(), blot.getRadius());
            blot.setPosition(center.x + radius * offsets[i][0], center.y + radius * offsets[i][1]);
            blot.setFillColor(withAlpha(color, static_cast<sf::Uint8>(color.a * (0.55f + 0.04f * i))));
            window.draw(blot);
        }

        for (int i = 0; i < 4; ++i)
        {
            sf::RectangleShape streak(sf::Vector2f(radius * (0.55f + i * 0.12f), radius * 0.06f));
            streak.setOrigin(0.f, streak.getSize().y * 0.5f);
            streak.setPosition(center.x + radius * (0.24f * i - 0.38f), center.y + radius * (0.18f * i - 0.22f));
            streak.setRotation(-25.f + i * 18.f);
            streak.setFillColor(withAlpha(color, static_cast<sf::Uint8>(color.a * 0.35f)));
            window.draw(streak);
        }
    }

    inline void drawBackdrop(sf::RenderWindow& window, const Palette& palette, float timeSeconds = 0.f)
    {
        const sf::Vector2u size = window.getSize();
        sf::VertexArray bg = makeGradientQuad(sf::FloatRect(0.f, 0.f, static_cast<float>(size.x), static_cast<float>(size.y)), palette.bgTop, palette.bgBottom);
        window.draw(bg);

        sf::RectangleShape wash(sf::Vector2f(static_cast<float>(size.x) * 1.18f, 138.f));
        wash.setOrigin(wash.getSize().x / 2.f, wash.getSize().y / 2.f);
        wash.setPosition(static_cast<float>(size.x) * 0.56f, 102.f);
        wash.setRotation(-10.f);
        wash.setFillColor(withAlpha(palette.accentSoft, 28));
        window.draw(wash);

        wash.setSize(sf::Vector2f(static_cast<float>(size.x) * 0.86f, 112.f));
        wash.setOrigin(wash.getSize().x / 2.f, wash.getSize().y / 2.f);
        wash.setPosition(static_cast<float>(size.x) * 0.28f, static_cast<float>(size.y) - 58.f);
        wash.setRotation(11.f);
        wash.setFillColor(withAlpha(palette.accent, 18));
        window.draw(wash);

        drawInkSplatter(window, sf::Vector2f(static_cast<float>(size.x) * 0.12f, 88.f), 42.f, withAlpha(palette.accent, 28));
        drawInkSplatter(window, sf::Vector2f(static_cast<float>(size.x) * 0.88f, 74.f), 34.f, withAlpha(palette.textSecondary, 18));
        drawInkSplatter(window, sf::Vector2f(static_cast<float>(size.x) * 0.82f, static_cast<float>(size.y) * 0.82f), 58.f, withAlpha(palette.accentSoft, 18));
        drawInkSplatter(window, sf::Vector2f(static_cast<float>(size.x) * 0.15f, static_cast<float>(size.y) * 0.78f), 48.f, withAlpha(palette.textPrimary, 10));

        const int spacing = 46;
        const int offset = static_cast<int>(timeSeconds * 9.f) % spacing;
        for (int x = -spacing; x < static_cast<int>(size.x) + spacing; x += spacing)
        {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(static_cast<float>(x + offset), 0.f), withAlpha(palette.textPrimary, 7)),
                sf::Vertex(sf::Vector2f(static_cast<float>(x + offset) - 45.f, static_cast<float>(size.y)), withAlpha(palette.textPrimary, 0))
            };
            window.draw(line, 2, sf::Lines);
        }
    }

    inline void drawPanel(sf::RenderWindow& window, const sf::FloatRect& rect, const Palette& palette,
                          bool accent = false, float shadowOffset = 7.f)
    {
        sf::RectangleShape shadow(sf::Vector2f(rect.width, rect.height));
        shadow.setPosition(rect.left + shadowOffset, rect.top + shadowOffset);
        shadow.setFillColor(withAlpha(sf::Color::Black, 90));
        window.draw(shadow);

        sf::RectangleShape panel(sf::Vector2f(rect.width, rect.height));
        panel.setPosition(rect.left, rect.top);
        panel.setFillColor(palette.panelFill);
        panel.setOutlineThickness(2.f);
        panel.setOutlineColor(accent ? palette.accent : palette.panelOutline);
        window.draw(panel);

        sf::RectangleShape inner(sf::Vector2f(rect.width - 8.f, rect.height - 8.f));
        inner.setPosition(rect.left + 4.f, rect.top + 4.f);
        inner.setFillColor(sf::Color::Transparent);
        inner.setOutlineThickness(1.f);
        inner.setOutlineColor(accent ? withAlpha(palette.accentSoft, 105) : withAlpha(palette.panelInner, 75));
        window.draw(inner);
    }

    inline void drawHeader(sf::RenderWindow& window, sf::Text& title, sf::Text* subtitle, const Palette& palette)
    {
        sf::Text shadow = title;
        shadow.move(3.f, 3.f);
        shadow.setFillColor(withAlpha(sf::Color::Black, 130));
        title.setFillColor(palette.textPrimary);
        window.draw(shadow);
        window.draw(title);

        if (subtitle)
        {
            sf::Text subtitleShadow = *subtitle;
            subtitleShadow.move(2.f, 2.f);
            subtitleShadow.setFillColor(withAlpha(sf::Color::Black, 100));
            subtitle->setFillColor(palette.textSecondary);
            window.draw(subtitleShadow);
            window.draw(*subtitle);
        }
    }

    inline void drawCursor(sf::RenderWindow& window, const Palette& palette, bool emphasize = false)
    {
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        struct TrailPoint { sf::Vector2f pos; };
        static std::array<TrailPoint, 5> trail = {};
        static bool initialized = false;
        if (!initialized)
        {
            for (std::size_t i = 0; i < trail.size(); ++i)
                trail[i].pos = mouse;
            initialized = true;
        }

        for (std::size_t i = trail.size() - 1; i > 0; --i)
            trail[i] = trail[i - 1];
        trail[0].pos = mouse;

        for (std::size_t i = 1; i < trail.size(); ++i)
        {
            float t = 1.f - static_cast<float>(i) / static_cast<float>(trail.size());
            sf::CircleShape sparkle((emphasize ? 2.6f : 2.1f) * t + 0.8f, 10);
            sparkle.setOrigin(sparkle.getRadius(), sparkle.getRadius());
            sparkle.setPosition(trail[i].pos.x - 3.f * static_cast<float>(i), trail[i].pos.y + 2.f * static_cast<float>(i));
            sparkle.setFillColor(withAlpha(palette.accentSoft, static_cast<sf::Uint8>(70.f * t)));
            window.draw(sparkle);
        }

        sf::ConvexShape pointer;
        pointer.setPointCount(7);
        pointer.setPoint(0, sf::Vector2f(0.f, 0.f));
        pointer.setPoint(1, sf::Vector2f(0.f, 18.f));
        pointer.setPoint(2, sf::Vector2f(4.8f, 13.2f));
        pointer.setPoint(3, sf::Vector2f(8.6f, 24.5f));
        pointer.setPoint(4, sf::Vector2f(11.8f, 22.8f));
        pointer.setPoint(5, sf::Vector2f(8.6f, 11.2f));
        pointer.setPoint(6, sf::Vector2f(18.2f, 11.2f));
        pointer.setPosition(mouse.x + 1.5f, mouse.y + 1.5f);
        pointer.setFillColor(withAlpha(sf::Color::Black, 100));
        window.draw(pointer);

        pointer.setPosition(mouse);
        pointer.setFillColor(brighten(palette.textPrimary, 8));
        pointer.setOutlineThickness(1.3f);
        pointer.setOutlineColor(brighten(palette.accent, 12));
        window.draw(pointer);

        sf::CircleShape glow(emphasize ? 4.5f : 3.8f, 18);
        glow.setOrigin(glow.getRadius(), glow.getRadius());
        glow.setPosition(mouse.x + 3.f, mouse.y + 4.f);
        glow.setFillColor(withAlpha(palette.accent, emphasize ? 84 : 62));
        window.draw(glow);
    }
}
