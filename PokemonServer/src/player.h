#ifndef PLAYER_H
#define PLAYER_H

#include "pokemon.h"
#include <QDateTime>
#include <set>

#define CAPACITY 3

//struct connected with database
struct PlayerInfo
{
    string name; //player's name
    string password; //player's password
    int pokemonNumber; //number of pokemon of player
    int packageCapacity; //capacity of player's package
    double rate; //player's rate
    int games; //number of game player has joined
    int rank; //rank score of player
    int thumb; //number of thumb player has gained
    string beginDateTime; //date time when player first play (eg.201611171230 2016-11-17 12:30)
    string gameTime; //time that player has spent on this game (eg.12003 120hours 3minutes)
    /** Object-Relation-Map, map this struct to a sqlite tab
     *  table contains filed in struct
     **/
    ORMAP(PlayerInfo, name, password,
          pokemonNumber, packageCapacity, rate, games, rank, thumb, beginDateTime,
          gameTime)
};

class Player
{
private:
    string _name_; //player's name
    string _password_; //player's password
    int _pokemonNumber_; //player's password
    int _packageCapacity_; //capacity of player's package
    double _rate_; //player's rate
    int _games_; //number of game player has joined
    int _rank_; //rank score of player
    int _thumb_; //number of thumb player has gained
    QDateTime _beginDateTime_; //date time when player first play
    string _gameTime_; //time that player has spent on this game
    set<Pokemon*> _pokemonGot_; //pokemon that player has got
public:
    /**
        Function : Player
        Description : construct function, construct a new player
        Input : PlayerInfo
        Return : None
        Other : None
    **/
    Player();

    /**
        Function : Player(PlayerInfo)
        Description : construct function with struct playerinfo, construct a player existing
        Input : PlayerInfo
        Return : None
        Other : None
    **/
    Player(struct PlayerInfo playerInfo);
    ~Player() {}

    /**
        Function : getName
        Description : return player's name
        Input : None
        Return : player's name
        Other : None
    **/
    string getName();

    /**
        Function : getPassword
        Description : return player's password
        Input : None
        Return : player's password
        Other : None
    **/
    string getPassword();

    /**
        Function : getPMNumber
        Description : return the number of player's pokemon
        Input : None
        Return : the number of player's pokemon
        Other : None
    **/
    int getPMNumber();

    /**
        Function : getRate
        Description : return player's rate
        Input : None
        Return : player's rate
        Other : None
    **/
    double getRate();

    /**
        Function : getRank
        Description : return player's rank score
        Input : None
        Return : player's rank score
        Other : None
    **/
    int getRank();

    /**
        Function : getThumb
        Description : return the number of thumb player has gained
        Input : None
        Return : the number of thumb player has gained
        Other : None
    **/
    int getThumb();

    /**
        Function : getThumb
        Description : return the number of games player has battled
        Input : None
        Return : the number of games player has battled
        Other : None
    **/
    int getGames();

    /**
        Function : getBeginDT
        Description : return the date time when player began
        Input : None
        Return : the date time when player began
        Other : None
    **/
    QDateTime getBeginDT();

    /**
        Function : getGameTime
        Description : return the time player has spent
        Input : None
        Return : the time player has spent
        Other : None
    **/
    string getGameTime();

    /**
        Function : getGotPokemon
        Description : return the pokemon player has got
        Input : None
        Return : the pokemon player has got
        Other : None
    **/
    set<Pokemon*> getGotPokemon();

    /**
        Function : ToPlayerInfo
        Description : transfer from Player object to PlayerInfo object
        Input : None
        Return : PlayerInfo object
        Other : None
    **/
    struct PlayerInfo ToPlayerInfo();

    /**
        Function : setName
        Description : set player's name
        Input : name
        Return : None
        Other : None
    **/
    void setName(string name);

    /**
        Function : setPassword
        Description : set player's password
        Input : password
        Return : None
        Other : None
    **/
    void setPassword(string password);

    /**
        Function : setPMNumber
        Description : set the number of player's pokemon
        Input : the number of player's pokemon
        Return : None
        Other : None
    **/
    void setPMNumber(int pokemonNumber);

    /**
        Function : setRank
        Description : set player's rank score
        Input : rank score
        Return : None
        Other : None
    **/
    void setRank(int rank);

    /**
        Function : setThumb
        Description : set the number of thumbs player has gained
        Input : the number of thumbs player has gained
        Return : None
        Other : None
    **/
    void setThumb(int thumb);

    /**
        Function : setBeginDateTime
        Description : set the begin date time player began
        Input : the begin date time player began
        Return : None
        Other : None
    **/
    void setBeginDateTime(QDateTime beginDateTime);

    /**
        Function : setGameTime
        Description : set the time player spent
        Input : the begin date time player began
        Return : None
        Other : None
    **/
    void setGameTime(QDateTime startDateTime);

    /**
        Function : addPokemon
        Description : add player's pokemon
        Input : Pokemon*
        Return : None
        Other : None
    **/
    void addPokemon(Pokemon *PMtoGet);
};

#endif // PLAYER_H
