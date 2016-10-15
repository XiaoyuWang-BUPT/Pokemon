#ifndef PLAYERFACTORY_H
#define PLAYERFACTORY_H

#include "player.h"

enum PlayerKind{NEW, OLD};

class PlayerFactory
{
public:
    PlayerFactory(){}
    ~PlayerFactory(){}
    Player *CreatePlayer(PlayerKind playerKind, string name, string password);
};

#endif // PLAYERFACTORY_H
