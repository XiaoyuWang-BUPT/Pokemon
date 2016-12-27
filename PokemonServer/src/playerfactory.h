#ifndef PLAYERFACTORY_H
#define PLAYERFACTORY_H

#include "player.h"

//enumeration of player's kind, new means new-comer, old means has logged
enum PlayerKind{NEW, OLD};

class PlayerFactory
{
public:
    PlayerFactory(){}
    ~PlayerFactory(){}

    /**
     * Function : CreatePlayer(PlayerInfo)
     * Description : create player object with struct player info
     * Input : struct player inffo
     * Return : Player object
     * Other :   None
    **/
    Player *CreatePlayer(struct PlayerInfo playerInfo);
};

#endif // PLAYERFACTORY_H
