#ifndef PLAYER_H
#define PLAYER_H

#include "pokemon.h"
#include <QDateTime>

class Player
{
private:
    string _name_;
    string _password_;
    int _pokemonNumber_;
    int _rank_;
    QDateTime _beginDateTime_;
    QTime _gameTime_;
    set<Pokemon> _pokemonGot_;
public:
    Player(); //老玩家回归构造函数 具体成员变量的赋值后set
    Player(string name, string password); //新注册用户构造函数
    ~Player() {}
    string getName();
    string getPassword();
    int getPMNumber();
    int getRank();
    QDateTime getBeginDT();
    QTime getGameTime();
    set<Pokemon> getGotPokemon();

    void setName(string name);
    void setPassword(string password);
    void setPMNumber(int pokemonNumber);
    void setRank(int rank);
    void setBeginDateTime(QDateTime beginDateTime);
    void setGameTime(QDateTime startDateTime);
    void addPokemon(Pokemon PMtoGet);
};

#endif // PLAYER_H
