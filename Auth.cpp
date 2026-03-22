#include "Auth.h"
#include "Game.h"
#include "GameData.h"
#include "LoginState.h"

namespace Auth
{
    void handleLoginOrRegister(Game& game, GameData& data)
    {
        // Forward into the new login state.
        // LOGIN_FOR_MAIN is the normal purpose.
        game.changeState(new LoginState(game, data, LOGIN_FOR_MAIN));
    }
}
