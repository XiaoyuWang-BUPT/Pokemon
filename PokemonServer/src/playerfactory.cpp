#include "playerfactory.h"

Player *PlayerFactory::CreatePlayer(PlayerInfo playerInfo)
{
    Player *p;
    p = new Player(playerInfo);
    return p;
}
