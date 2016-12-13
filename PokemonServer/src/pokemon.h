#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "lib/PoorORMLite.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

using namespace std;

/* Pokemon information stored in database
 * How to create an object with this information:
   step1.Choose construct function depending on nature
   step2.Create an object
 * attention:
   1.'nature' 'kind' 'character' 'state' correspond Enumeration in Pokemon.h
      transfer them to Enumeration using array xxxOfString when instantiating
   2.counterVec info is not recorded in db but construct function knows*/
struct PokemonInfo
{
    string owner;
    string name; //获得新的小精灵时命名 不命名则默认为 小火龙 水箭龟 皮卡丘
    int nature; //火 水 草 冰 四种属性 FIRE> BUSH; FIRE> ELECTRICITY; WATER> FIRE; BUSH> WATER; ELECTRICITY> FIRE
    int kind; //小精灵的细类 小火龙 水箭龟 皮卡丘 等
    int character; //攻击型 防御型 肉盾型 敏捷型 四种性格
    int level;
    int experiencePoint;
    int attackPoint;
    int defencePoint;
    int totalHP;
    int currentHP;
    int intervalIncrease;
    int criticalPoint;
    //set<Nature> counter;
    int state;
    int sickCounter;
    int sickPoint;
    int alive;
    ORMAP(PokemonInfo, name, owner, nature, kind, character, level, experiencePoint,
          attackPoint, defencePoint, totalHP, currentHP, intervalIncrease,
          criticalPoint, state, sickCounter, sickPoint, alive)
};

class Pokemon
{
private:
    string _owner_;
    Nature _nature_; //火 水 草 冰 四种属性 FIRE> BUSH; FIRE> ELECTRICITY; WATER> FIRE; BUSH> WATER; ELECTRICITY> FIRE
    Kind _kind_; //小精灵的细类 小火龙 水箭龟 皮卡丘 等
    string _name_; //获得新的小精灵时命名 不命名则默认为 小火龙 水箭龟 皮卡丘
    Character _character_; //攻击型 防御型 肉盾型 敏捷型 四种性格
    int _level_; //小精灵等级
    int _experiencePoint_; //经验值 累计达到经验槽设定数值升级
    int _attackPoint_; //攻击力
    int _defencePoint_; //防御力
    int _totalHP_; //总生命值
    int _currentHP_; //当前生命值
    int _intervalIncrease_; //攻击间隔增长
    //TODO while循环中两小精灵累加_intervalIncrease_，直至同一阈值（如50），率先到达者先攻击，调用攻击者的攻击函数及动画
    int _criticalPoint_; //暴击指数
    vector<Nature> _counter_; //克制的属性
    State _state_; //当前所处的健康状态 健康 烧伤 冻伤 中毒 麻痹
    int _sickCounter_; //不健康计数器 用于记录烧伤 冻伤 中毒 麻痹 的回合数
    int _sickPoint_; //烧伤 冻伤 中毒 麻痹 每次的伤害
    bool _alive_; //存活 死亡
public:
    Pokemon(){}

    ~Pokemon(){}

    struct PokemonInfo ToPokeStruInfo();

    void setOwner(std::string o)
    {
        this->_owner_ = o;
    }

    void setNature(Nature nature) {
        this->_nature_ = nature;
    }

    void setKind(Kind kind) {
        this->_kind_ = kind;
    }

    void setName(string name) {
        this->_name_ = name;
    }

    void setCharacter(Character character) {
        this->_character_ = character;
    }

    void setLevel(int level) {
        this->_level_ = level;
    }

    void setExperiencePoint(int experiencePoint) {
        this->_experiencePoint_ = experiencePoint;
    }

    void setAttackPoint(int attackPoint) {
        this->_attackPoint_ = attackPoint;
    }

    void setDefencePoint(int defencePoint) {
        this->_defencePoint_ = defencePoint;
    }

    void setTotalHP(int totalHP) {
        this->_totalHP_ = totalHP;
    }

    void setCurrentHP(int currentHP) {
        this->_currentHP_ = currentHP;
    }

    void setIntervalIncrease(int intervalIncrease) {
        this->_intervalIncrease_ = intervalIncrease;
    }

    void setCriticalPoint(int criticalPoint) {
        this->_criticalPoint_ = criticalPoint;
    }

    void setCounter(vector<Nature> counter) {
        this->_counter_ = counter;
    }

    void setState(State state) {
        this->_state_ = state;
    }

    void setSickCounter(int sickCounter) {
        this->_sickCounter_ = sickCounter;
    }

    void setSickPoint(int sickPoint) {
        this->_sickPoint_ = sickPoint;
    }

    void setAlive(bool alive) {
        this->_alive_ = alive;
    }

    std::string getOwner()
    {
        return this->_owner_;
    }

    Nature getNature()
    {
        return this->_nature_;
    }

    Kind getKind() {
        return this->_kind_;
    }

    string getName() {
        return this->_name_;
    }

    Character getCharacter() {
        return this->_character_;
    }

    int getLevel() {
        return this->_level_;
    }

    int getExperiencePoint()
    {
        return this->_experiencePoint_;
    }

    int getAttackPoint()
    {
        return this->_attackPoint_;
    }

    int getDefencePoint()
    {
        return this->_defencePoint_;
    }

    int getTotalHP()
    {
        return this->_totalHP_;
    }

    int getCurrentHP() {
        return this->_currentHP_;
    }

    vector<Nature> getCounterVec() {
        return this->_counter_;
    }

    State getState() {
        return this->_state_;
    }

    int getCriticalPoint() {
        return this->_criticalPoint_;
    }

    int getSickCounter() {
        return this->_sickCounter_;
    }

    int getSickPoint() {
        return this->_sickPoint_;
    }

    bool getAlive() {
        return this->_alive_;
    }

    int getIntervalIncrease()
    {
        return this->_intervalIncrease_;
    }

    void Attack(Pokemon *dePokemon); //普通攻击可以暴击

    virtual void SpecialAttack(Pokemon *dePokemon){} //TODO 子类继承覆盖 根据属性进行附带属性效果的特殊攻击 特殊攻击可以借助属性克制增加伤害 有概率致病

    void SpecialAttackDamage(Pokemon *dePokemon); // 特殊攻击当即造成伤害

    virtual void EnSick(Pokemon *sickPokemon){} // 特殊攻击致病

    bool EnSickPossible();

    bool CriticalStrike();

    void Hurt();

    void DeadJudge();

    void Upgrade();

    void Evolution(int evoLevel); //换图标 换Kind

    //重载 < 用于Player中创建Pokemon容器set
    bool operator < (const Pokemon& anotherPokemon) const
    {return _level_ < anotherPokemon._level_;}
};

class Fire : public Pokemon
{
public:
    //Fire(Kind kind);// 御三家构造函数
    Fire(Kind kind, int level, string name, string owner); //对战时随机生成的精灵构造函数 野生精灵构造函数
    Fire(struct PokemonInfo pokemonInfo);
    ~Fire() {}
    void EnSick(Pokemon *sickPokemon);
    void SpecialAttack(Pokemon *dePokemon);
};

class Water : public Pokemon
{
public:
    //Water(Kind kind);
    Water(Kind kind, int level, string name, string owner); //对战时随机生成的精灵构造函数 野生精灵构造函数
    Water(struct PokemonInfo pokemonInfo);
    ~Water() {}
    void EnSick(Pokemon *sickPokemon);
    void SpecialAttack(Pokemon *dePokemon);
};

class Bush : public Pokemon
{
public:
    //Bush(Kind kind);
    Bush(Kind kind, int level, string name, string owner); //对战时随机生成的精灵构造函数 野生精灵构造函数
    Bush(struct PokemonInfo pokemonInfo);
    ~Bush() {}
    void EnSick(Pokemon *sickPokemon);
    void SpecialAttack(Pokemon *dePokemon);
};

class Electricity : public Pokemon
{
public:
    //Electricity(Kind kind);
    Electricity(Kind kind, int level, string name, string owner); //对战时随机生成的精灵构造函数 野生精灵构造函数
    Electricity(struct PokemonInfo pokemonInfo);
    ~Electricity() {}
    void EnSick(Pokemon *sickPokemon);
    void SpecialAttack(Pokemon *dePokemon);
};

unsigned int Random(int max);

#endif // POKEMON_H
