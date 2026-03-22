#pragma once

class Game;
struct GameData;

namespace Auth
{
    // Old code used this as an entry point to login/register.
    // Now it just forwards into our new LoginState screen.
    void handleLoginOrRegister(Game& game, GameData& data);
}
