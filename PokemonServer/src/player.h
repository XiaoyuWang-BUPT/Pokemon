#ifndef PLAYER_H
#define PLAYER_H

#include "pokemon.h"
#include <QDateTime>
#include <set>

struct PlayerInfo
{
    string name;
    string password;
    int pokemonNumber;
    int packageCapacity;
    int rank;
    string beginDateTime; //201611171230 2016-11-17 12:30
    string gameTime; //12003 120hours 3minutes
    ORMAP(PlayerInfo, name, password,
          pokemonNumber, packageCapacity, rank, beginDateTime,
          gameTime)
};

class Player
{
private:
    string _name_;
    string _password_;
    int _pokemonNumber_;
    int _rank_;
    QDateTime _beginDateTime_;
    string _gameTime_;
    set<Pokemon*> _pokemonGot_;
public:
    Player();
    Player(struct PlayerInfo playerInfo); //new player get new info, old player fetch from db
    ~Player() {}
    string getName();
    string getPassword();
    int getPMNumber();
    int getRank();
    QDateTime getBeginDT();
    string getGameTime();
    set<Pokemon*> getGotPokemon();

    void setName(string name);
    void setPassword(string password);
    void setPMNumber(int pokemonNumber);
    void setRank(int rank);
    void setBeginDateTime(QDateTime beginDateTime);
    void setGameTime(QDateTime startDateTime);
    void addPokemon(Pokemon *PMtoGet);
};

#endif // PLAYER_H
