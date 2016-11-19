#include "playerfactory.h"

//Player *PlayerFactory::CreatePlayer(PlayerKind playerKind, string name, string password) {
//    Player *p;
//    switch (playerKind) {
//    case NEW:
//        p = new Player(name, password);
//        break;
//    case OLD:
//        p = new Player();
//        //TODO read player information from database AND initialize them
//        break;
//    default:
//        cout << "Not a new Nor old player(check PlayerFactory.c CreatePlayer)" << endl;
//        break;
//    }
//    return p;
//}
Player *PlayerFactory::CreatePlayer(PlayerInfo playerInfo)
{
    Player *p;
    p = new Player(playerInfo);
    return p;
}
