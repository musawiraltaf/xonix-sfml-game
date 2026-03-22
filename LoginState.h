#pragma once

#include "State.h"
#include "GameData.h"
#include <SFML/Graphics.hpp>
#include <string>

// Why are we logging in?
enum LoginPurpose
{
    LOGIN_FOR_MAIN,        // normal login at game start
    LOGIN_FOR_COOP_EXTRA,  // login extra player for local co-op
    LOGIN_FOR_ROOM// login N players for matchmaking room
};

class LoginState : public State
{
public:
    // default purpose = LOGIN_FOR_MAIN, so old calls still work
    LoginState(Game& game, GameData& data, LoginPurpose purpose = LOGIN_FOR_MAIN);

    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    void tryLogin();
    void tryRegister();
    void showError(const std::string& msg);
    void rebuildTexts(); // rebuild SFML Text from string buffers

private:
    LoginPurpose purpose;

    sf::Font font;

    sf::Text title;
    sf::Text usernameLabel;
    sf::Text usernameText;
    sf::Text passwordLabel;
    sf::Text passwordText;
    sf::Text helper;
    sf::Text errorText;

    std::string usernameInput;
    std::string passwordInput;
    bool        enteringUsername; // true = editing username, false = password
};
