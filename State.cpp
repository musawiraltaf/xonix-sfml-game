#include "State.h"
#include "Game.h"
#include "GameData.h"

State::State(Game& game, GameData& data)
    : game(game), data(data)
{
}

State::~State() {}
