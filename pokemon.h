#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <set>
#include "widget.h"

using namespace std;

enum Nature {FIRE, WATER, BUSH, ELECTRICITY};
enum Kind{/*TODO 具体的小精灵的细类--如小火龙 皮卡丘 水箭龟 等*/
            CHARMANDER, CHARMELEON, CHARIZARD, //小火龙 火恐龙 喷火龙
            SQUIRTLE, WARTORTLE, BLASTOISE, //杰尼龟 卡咪龟 水箭龟
            BULBASAUR, IVYSAUR, VENUSAUR, //秒蛙种子 秒蛙草 妙蛙花
            PICHU, PIKACHU, RAICHU, //皮丘 皮卡丘 雷丘
            TORCHIC, COMBUSKEN, BLAZIKEN, //火稚鸡 力壮鸡 火焰鸡
            MUDKIP, MARSHTOMP, SWAMPERT, //水跃鱼 沼跃鱼 巨沼怪
            TREECKO, GROVYLE, SCEPTILE, //木守宫 森林蜥蜴 蜥蜴王
            SHINX, LUXIO, LUXRAY, //小猫怪 勒克猫 伦琴猫
            CHIMCHAR, MONFERNO, INFERNAPE, //小火焰猴 猛火猴 烈焰猴
            PIPLUP, PRINPLUP, EMPOLEON, //波加曼 波皇子 帝王拿波
            TURTWIG, GROTLE, TORTERRA, //草苗龟 树林龟 土台龟
            MARREP, FLAAFFY, AMPHAROS //咩利羊 绵绵 电龙
            };
enum State {HEALTHY, DROWNED, BURNED, POISONED, FROZEN};
enum Character {AGGRESIVE, DEFENSIVE, TANK, QUICK};
enum evoLevel {EL1 = 6, EL2 = 13};
const int ExpGrade[17] = {0, 0, 50, 120, 210, 320, 500, 720,
                   1000, 1320, 1800, 2400, 3160, 3900,
                   4900, 6200};
const bool ALIVE = true, DEAD = false;
const int UpGradeAttrIncStrong = 10;
const int UpGradeAttrIncNormal = 7;
const int UpGradeAttrIncWeak = 4;
const int UpGradeCriticalInc = 5;

class Pokemon
{
private:
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
    set<Nature> _counter_; //克制的属性
    State _state_; //当前所处的健康状态 健康 烧伤 冻伤 中毒 麻痹
    int _sickCounter_; //不健康计数器 用于记录烧伤 冻伤 中毒 麻痹 的回合数
    int _sickPoint_; //烧伤 冻伤 中毒 麻痹 每次的伤害
    bool _alive_; //存活 死亡
public:
    Pokemon(){
        this->_nature_ = FIRE;
        this->_kind_ = CHARMANDER;
        this->_name_ = "Charmander";
        this->_character_ = AGGRESIVE;
        this->_level_ = 1;
        this->_experiencePoint_ = 0;
        this->_attackPoint_ = 15;
        this->_defencePoint_ = 7;
        this->_totalHP_ = 60;
        this->_currentHP_ = this->_totalHP_;
        this->_intervalIncrease_ = 4;
        this->_criticalPoint_ = 5;
        this->_counter_.insert(BUSH);
        this->_counter_.insert(ELECTRICITY);
        this->_state_ = HEALTHY;
        this->_sickCounter_ = 0;
        this->_sickPoint_ = 0;
        this->_alive_ = ALIVE;
    }

    ~Pokemon(){}

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

    void setCounter(set<Nature> counter) {
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

    set<Nature> getCounterSet() {
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

    virtual void SpecialAttack(Pokemon *dePokemon){}//TODO 子类继承覆盖 根据属性进行附带属性效果的特殊攻击 特殊攻击可以借助属性克制增加伤害 有概率致病

    virtual void EnSick(Pokemon *sickPokemon){}

    int randFunction();

    bool EnSickPossible();

    bool CriticalStrike();

    void Hurt();

    void DeadJudge();

    void Upgrade();

    virtual void Evolution(int evoLevel){
        //rTODO  子类继承覆盖  换图标 如果时默认命名则更新名字（小火龙-> 火恐龙-> 喷火龙）
    }
};

class Fire : public Pokemon
{
public:
    Fire() {}
    ~Fire() {}
    void EnSick(Pokemon *sickPokemon);
    void SpecialAttack(Pokemon *dePokemon);
    virtual void Evolution(int evoLevel){}//TODO  子类继承覆盖  换图标 如果时默认命名则更新名字（小火龙-> 火恐龙-> 喷火龙）
};

class Water : public Pokemon
{
public:
    Water() {}
    ~Water() {}
    void EnSick(Pokemon *sickPokemon);
    void SpecialAttack(Pokemon *dePokemon);
    virtual void Evolution(int evoLevel){}//TODO  子类继承覆盖  换图标 如果时默认命名则更新名字（小火龙-> 火恐龙-> 喷火龙）
};

class Bush : public Pokemon
{
public:
    Bush() {}
    ~Bush() {}
    void EnSick(Pokemon *sickPokemon);
    void SpecialAttack(Pokemon *dePokemon);
    virtual void Evolution(int evoLevel){}//TODO  子类继承覆盖  换图标 如果时默认命名则更新名字（小火龙-> 火恐龙-> 喷火龙）
};

class Electricity : public Pokemon
{
public:
    Electricity() {}
    ~Electricity() {}
    void EnSick(Pokemon *sickPokemon);
    void SpecialAttack(Pokemon *dePokemon);
    virtual void Evolution(int evoLevel){}//TODO  子类继承覆盖  换图标 如果时默认命名则更新名字（小火龙-> 火恐龙-> 喷火龙）
};

#endif // POKEMON_H
