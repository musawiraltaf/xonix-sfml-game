#pragma once

#include "State.h"
#include "GameData.h"
#include <SFML/Graphics.hpp>
#include <string>

enum LoginPurpose
{
    LOGIN_FOR_MAIN,
    LOGIN_FOR_COOP_EXTRA,
    LOGIN_FOR_ROOM
};

class LoginState : public State
{
public:
    LoginState(Game& game, GameData& data, LoginPurpose purpose = LOGIN_FOR_MAIN);

    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    void tryLogin();
    void tryRegister();
    void showError(const std::string& msg);
    void rebuildTexts();

private:
    LoginPurpose purpose;

    sf::Font font;
    sf::Text title;
    sf::Text subtitle;
    sf::Text usernameLabel;
    sf::Text usernameText;
    sf::Text passwordLabel;
    sf::Text passwordText;
    sf::Text helper;
    sf::Text errorText;
    float animTime;

    std::string usernameInput;
    std::string passwordInput;
    bool enteringUsername;
};
