#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "lib/PoorORMLite.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

using namespace std;

/** Pokemon information stored in database
 *  How to create an object with this information:
    step1.Choose construct function depending on nature
    step2.Create an object
 *  attention:
    1.'nature' 'kind' 'character' 'state' correspond Enumeration in Pokemon.h
       transfer them to Enumeration using array xxxOfString when instantiating
    2.counterVec info is not recorded in db but construct function knows
**/
struct PokemonInfo
{
    //owner of pokemon
    string owner;    
    //name of pokemon
    string name;
    //nature of pokemon (eg.fire, water...)
    int nature;
    //kind of pokemon (eg.charmander, squirtle...)
    int kind;
    //character of pokemon (eg.aggresive, defensive...)
    int character;
    //level of pokemon
    int level;
    //experience pokemon has gained
    int experiencePoint;
    //attack point of pokemon
    int attackPoint;
    //defence point of pokemon
    int defencePoint;
    //total HP of pokemon
    int totalHP;
    //current HP of pokemon
    int currentHP;
    //interval increasing of pokemon, how fast it moves on initiative bar
    int intervalIncrease;
    //critical possibility of pokemon
    int criticalPoint;
    //health state of pokemon
    int state;
    //round count sick state if it is unhealthy
    int sickCounter;
    //hurt by sick state everytime
    int sickPoint;
    //alive or dead
    int alive;
    /** Object-Relation-Map, map this struct to a sqlite tab
     *  table contains filed in struct
     **/
    ORMAP(PokemonInfo, name, owner, nature, kind, character, level, experiencePoint,
          attackPoint, defencePoint, totalHP, currentHP, intervalIncrease,
          criticalPoint, state, sickCounter, sickPoint, alive)
};

class Pokemon
{
private:
    //owner of pokemon
    string _owner_;
    //nature of pokemon
    Nature _nature_;
    //kind of pokemon
    Kind _kind_;
    //name of pokemon
    string _name_;
    //character of pokemon
    Character _character_;
    //level of pokemon
    int _level_;
    //experience pokemon has gained
    int _experiencePoint_;
    //attack point of pokemon
    int _attackPoint_;
    //defence point of pokemon
    int _defencePoint_;
    //total HP of pokemon
    int _totalHP_;
    //current HP of pokemon
    int _currentHP_;
    //interval increasing of pokemon, how fast it moves on initiative bar
    int _intervalIncrease_;
    //critical possibility of pokemon
    int _criticalPoint_;
    //nature(s) countered by pokemon
    vector<Nature> _counter_;
    //state of pokemon
    State _state_;
    //round cnount of unhealthy state if it is sick
    int _sickCounter_;
    //hurt by sick state everytime
    int _sickPoint_;
    //alive or dead
    bool _alive_;
public:
    Pokemon(){}

    ~Pokemon(){}

    /**
     * Function : ToPokeStruInfo
     * Description : get a pokemon info struct object according to pokemon object
     * Input : None
     * Return : struct pokemon info object
     * Other : None
    **/
    struct PokemonInfo ToPokeStruInfo();

    /**
     * Function : setOwner
     * Description : set owner of pokemon
     * Input : std::string
     * Return : None
     * Other : None
    **/
    void setOwner(std::string o)
    {
        this->_owner_ = o;
    }

    /**
     * Function : setNature
     * Description : set nature of pokemon
     * Input : Nature
     * Return : None
     * Other : None
    **/
    void setNature(Nature nature) {
        this->_nature_ = nature;
    }

    /**
     * Function : setKind
     * Description : set kind of pokemon
     * Input : Kind
     * Return : None
     * Other : None
    **/
    void setKind(Kind kind) {
        this->_kind_ = kind;
    }

    /**
     * Function : setName
     * Description : set name of pokemon
     * Input : std::string
     * Return : None
     * Other : None
    **/
    void setName(string name) {
        this->_name_ = name;
    }

    /**
     * Function : setCharacter
     * Description : set character of pokemon
     * Input : Character
     * Return : None
     * Other : None
    **/
    void setCharacter(Character character) {
        this->_character_ = character;
    }

    /**
     * Function : setLevel
     * Description : set level of pokemon
     * Input : int
     * Return : None
     * Other : None
    **/
    void setLevel(int level) {
        this->_level_ = level;
    }

    /**
     * Function : setExperiencePoint
     * Description : set experiance point of pokemon
     * Input : int
     * Return : None
     * Other : None
    **/
    void setExperiencePoint(int experiencePoint) {
        this->_experiencePoint_ = experiencePoint;
    }

    /**
     * Function : setAttackPoint
     * Description : set attack point of pokemon
     * Input : int
     * Return : None
     * Other : None
    **/
    void setAttackPoint(int attackPoint) {
        this->_attackPoint_ = attackPoint;
    }

    /**
     * Function : setDefencePoint
     * Description : set defence point of pokemon
     * Input : int
     * Return : None
     * Other : None
    **/
    void setDefencePoint(int defencePoint) {
        this->_defencePoint_ = defencePoint;
    }

    /**
     * Function : setTotalHP
     * Description : set total HP of pokemon
     * Input : int
     * Return : None
     * Other : None
    **/
    void setTotalHP(int totalHP) {
        this->_totalHP_ = totalHP;
    }

    /**
     * Function : setCurrentHP
     * Description : set current HP of pokemon
     * Input : int
     * Return : None
     * Other : None
    **/
    void setCurrentHP(int currentHP) {
        this->_currentHP_ = currentHP;
    }

    /**
     * Function : setIntervalIncrease
     * Description : set interval increase of pokemon
     * Input : int
     * Return : None
     * Other : None
    **/
    void setIntervalIncrease(int intervalIncrease) {
        this->_intervalIncrease_ = intervalIncrease;
    }

    /**
     * Function : setCriticalPoint
     * Description : set critical point of pokemon
     * Input : int
     * Return : None
     * Other : None
    **/
    void setCriticalPoint(int criticalPoint) {
        this->_criticalPoint_ = criticalPoint;
    }

    /**
     * Function : setCounter
     * Description : set counter vector of pokemon
     * Input : std:vector
     * Return : None
     * Other : None
    **/
    void setCounter(vector<Nature> counter) {
        this->_counter_ = counter;
    }

    /**
     * Function : setState
     * Description : set state of pokemon
     * Input : State
     * Return : None
     * Other : None
    **/
    void setState(State state) {
        this->_state_ = state;
    }

    /**
     * Function : setSickCounter
     * Description : set sick counter of pokemon
     * Input : int
     * Return : None
     * Other : None
    **/
    void setSickCounter(int sickCounter) {
        this->_sickCounter_ = sickCounter;
    }

    /**
     * Function : setSickPoint
     * Description : set sick point of pokemon
     * Input : int
     * Return : None
     * Other : None
    **/
    void setSickPoint(int sickPoint) {
        this->_sickPoint_ = sickPoint;
    }

    /**
     * Function : setAlive
     * Description : set alive of pokemon
     * Input : bool
     * Return : None
     * Other : None
    **/
    void setAlive(bool alive) {
        this->_alive_ = alive;
    }

    /**
     * Function : getOwner
     * Description : get owner of pokemon
     * Input : None
     * Return : std::string
     * Other : None
    **/
    std::string getOwner()
    {
        return this->_owner_;
    }

    /**
     * Function : getNature
     * Description : get nature of pokemon
     * Input : None
     * Return : Nature
     * Other : None
    **/
    Nature getNature()
    {
        return this->_nature_;
    }

    /**
     * Function : getKind
     * Description : get kind of pokemon
     * Input : None
     * Return : Kind
     * Other : None
    **/
    Kind getKind() {
        return this->_kind_;
    }

    /**
     * Function : getName
     * Description : get name of pokemon
     * Input : None
     * Return : std::string
     * Other : None
    **/
    string getName() {
        return this->_name_;
    }

    /**
     * Function : getCharacter
     * Description : get character of pokemon
     * Input : None
     * Return : Character
     * Other : None
    **/
    Character getCharacter() {
        return this->_character_;
    }

    /**
     * Function : getLevel
     * Description : get level of pokemon
     * Input : None
     * Return : int
     * Other : None
    **/
    int getLevel() {
        return this->_level_;
    }

    /**
     * Function : getExperiencePoint
     * Description : get experience point of pokemon
     * Input : None
     * Return : int
     * Other : None
    **/
    int getExperiencePoint()
    {
        return this->_experiencePoint_;
    }

    /**
     * Function : getAttackPoint
     * Description : get attack point of pokemon
     * Input : None
     * Return : int
     * Other : None
    **/
    int getAttackPoint()
    {
        return this->_attackPoint_;
    }

    /**
     * Function : getDefencePoint
     * Description : get defence point of pokemon
     * Input : None
     * Return : int
     * Other : None
    **/
    int getDefencePoint()
    {
        return this->_defencePoint_;
    }

    /**
     * Function : getTotalHP
     * Description : get total HP of pokemon
     * Input : None
     * Return : int
     * Other : None
    **/
    int getTotalHP()
    {
        return this->_totalHP_;
    }

    /**
     * Function : getCurrentHP
     * Description : get current HP of pokemon
     * Input : None
     * Return : int
     * Other : None
    **/
    int getCurrentHP() {
        return this->_currentHP_;
    }

    /**
     * Function : getCounterVec
     * Description : get counter vector of pokemon
     * Input : None
     * Return : std::vector<Nature>
     * Other : None
    **/
    vector<Nature> getCounterVec() {
        return this->_counter_;
    }

    /**
     * Function : getState
     * Description : get state of pokemon
     * Input : None
     * Return : State
     * Other : None
    **/
    State getState() {
        return this->_state_;
    }

    /**
     * Function : getCriticalPoint
     * Description : get critical point of pokemon
     * Input : None
     * Return : int
     * Other : None
    **/
    int getCriticalPoint() {
        return this->_criticalPoint_;
    }

    /**
     * Function : getSickCounter
     * Description : get sick counter of pokemon
     * Input : None
     * Return : int
     * Other : None
    **/
    int getSickCounter() {
        return this->_sickCounter_;
    }

    /**
     * Function : getSickPoint
     * Description : get sick point of pokemon
     * Input : None
     * Return : int
     * Other : None
    **/
    int getSickPoint() {
        return this->_sickPoint_;
    }

    /**
     * Function : getAlive
     * Description : get alive of pokemon
     * Input : None
     * Return : bool
     * Other : None
    **/
    bool getAlive() {
        return this->_alive_;
    }

    /**
     * Function : getIntervalIncrease
     * Description : get interval increase of pokemon
     * Input : None
     * Return : int
     * Other : None
    **/
    int getIntervalIncrease()
    {
        return this->_intervalIncrease_;
    }

    /**
     * Function : Attack
     * Description : attack another pokemon
     * Input : pokemon which is attacked by this pokemon
     * Return : None
     * Other : attack is normal, and it can cause critical damage
    **/
    void Attack(Pokemon *dePokemon);

    /**
     * Function : SpecialAttack
     * Description : special attack another pokemon
     * Input : pokemon which is attacked by this pokemon
     * Return : None
     * Other : need overwriten in inherited class
    **/
    virtual void SpecialAttack(Pokemon *dePokemon){}

    /**
     * Function : SpecialAttackDamage
     * Description : special attack cause damage immediately
     * Input : pokemon which is be attacked
     * Return : None
     * Other : seperate inmmediate damage and burned/drwoned/poisoned/shocked hurt
    **/
    void SpecialAttackDamage(Pokemon *dePokemon);

    /**
     * Function : EnSick
     * Description : make pokemon which is be attacked by special attack sick
     * Input : pokemon which is be attacked by special attack
     * Return : None
     * Other : need overwriten in inherited class
    **/
    virtual void EnSick(Pokemon *sickPokemon){}

    /**
     * Function : EnsickPossible
     * Description : whether special attack can cause sick state
     * Input : None
     * Return : bool
     * Other : possibility is constant even if level up or between different pokemons
    **/
    bool EnSickPossible();

    /**
     * Function : CriticalStrike
     * Description : whether normal attack is a critical one
     * Input : None
     * Return : bool
     * Other : possibility differs among pokemons in different levels
    **/
    bool CriticalStrike();

    /**
     * Function : Hurt
     * Description : pokemon get hurt by sick state
     * Input : None
     * Return : None
     * Other : pokemon get hurt by sick state before it attacks
    **/
    void Hurt();

    /**
     * Function : DeadJudge
     * Description : judget whether pokemon is dead or not and set variables
     * Input : None
     * Return : None
     * Other : None
    **/
    void DeadJudge();

    /**
     * Function : UpGrade
     * Description : pokemon upgrade
     * Input : None
     * Return : None
     * Other : None
    **/
    void Upgrade();

    /**
     * Function : Evolution
     * Description : pokemon evolute
     * Input : current level of pokemon
     * Return : None
     * Other : None
    **/
    void Evolution(int evoLevel);

    //overload < to complete Pokemon set in Player
    bool operator < (const Pokemon& anotherPokemon) const
    {return _level_ < anotherPokemon._level_;}
};

class Fire : public Pokemon
{
public:
    /**
     * Function : Fire
     * Description : construc function with pokemon's kind, level, name and owner
     * Input : kind, level, name, owner
     * Return : None
     * Other : None
    **/
    Fire(Kind kind, int level, string name, string owner);

    /**
     * Function : Fire
     * Description : construct function with PokemonInfo struct
     * Input : struct PokemonInfo
     * Return : None
     * Other : None
    **/
    Fire(struct PokemonInfo pokemonInfo);
    ~Fire() {}

    /**
     * Function : Ensick
     * Description : make dePokemon burned
     * Input : pokemon which is be attacked by special attack
     * Return : None
     * Other : None
    **/
    void EnSick(Pokemon *sickPokemon);

    /**
     * Function : SpecialAttack
     * Description : special attack
     * Input : pokemon which is be attacked by special attack
     * Return : None
     * Other : None
    **/
    void SpecialAttack(Pokemon *dePokemon);
};

class Water : public Pokemon
{
public:
    /**
     * Function : Water
     * Description : construc function with pokemon's kind, level, name and owner
     * Input : kind, level, name, owner
     * Return : None
     * Other : None
    **/
    Water(Kind kind, int level, string name, string owner); //对战时随机生成的精灵构造函数 野生精灵构造函数

    /**
     * Function : Water
     * Description : construct function with PokemonInfo struct
     * Input : struct PokemonInfo
     * Return : None
     * Other : None
    **/
    Water(struct PokemonInfo pokemonInfo);
    ~Water() {}

    /**
     * Function : Ensick
     * Description : make dePokemon drwoned
     * Input : pokemon which is be attacked by special attack
     * Return : None
     * Other : None
    **/
    void EnSick(Pokemon *sickPokemon);

    /**
     * Function : SpecialAttack
     * Description : special attack
     * Input : pokemon which is be attacked by special attack
     * Return : None
     * Other : None
    **/
    void SpecialAttack(Pokemon *dePokemon);
};

class Bush : public Pokemon
{
public:
    /**
     * Function : Bush
     * Description : construc function with pokemon's kind, level, name and owner
     * Input : kind, level, name, owner
     * Return : None
     * Other : None
    **/
    Bush(Kind kind, int level, string name, string owner); //对战时随机生成的精灵构造函数 野生精灵构造函数

    /**
     * Function : Bush
     * Description : construct function with PokemonInfo struct
     * Input : struct PokemonInfo
     * Return : None
     * Other : None
    **/
    Bush(struct PokemonInfo pokemonInfo);
    ~Bush() {}

    /**
     * Function : Ensick
     * Description : make dePokemon poisoned
     * Input : pokemon which is be attacked by special attack
     * Return : None
     * Other : None
    **/
    void EnSick(Pokemon *sickPokemon);

    /**
     * Function : SpecialAttack
     * Description : special attack
     * Input : pokemon which is be attacked by special attack
     * Return : None
     * Other : None
    **/
    void SpecialAttack(Pokemon *dePokemon);
};

class Electricity : public Pokemon
{
public:
    /**
     * Function : Electricity
     * Description : construc function with pokemon's kind, level, name and owner
     * Input : kind, level, name, owner
     * Return : None
     * Other : None
    **/
    Electricity(Kind kind, int level, string name, string owner); //对战时随机生成的精灵构造函数 野生精灵构造函数

    /**
     * Function : Electricity
     * Description : construct function with PokemonInfo struct
     * Input : struct PokemonInfo
     * Return : None
     * Other : None
    **/
    Electricity(struct PokemonInfo pokemonInfo);
    ~Electricity() {}

    /**
     * Function : Ensick
     * Description : make dePokemon shocked
     * Input : pokemon which is be attacked by special attack
     * Return : None
     * Other : None
    **/
    void EnSick(Pokemon *sickPokemon);

    /**
     * Function : SpecialAttack
     * Description : special attack
     * Input : pokemon which is be attacked by special attack
     * Return : None
     * Other : None
    **/
    void SpecialAttack(Pokemon *dePokemon);
};

/**
 * Function : Random
 * Description : high-precison random function, generate a random number between 0 and max
 * Input : maximum of random number generated
 * Return : random number
 * Other : None
**/
unsigned int Random(int max);

#endif // POKEMON_H
