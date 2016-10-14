#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <set>

using namespace std;

enum Nature {FIRE, WATER, BUSH, ELECTRICITY};
const string natureOfString[] = {"Fire", "Water", "Bush", "Electricity"};
enum Kind{/*TODO 具体的小精灵的细类--如小火龙 皮卡丘 水箭龟 等*/
            CHARMANDER, CHARMELEON, CHARIZARD, //小火龙 火恐龙 喷火龙
            TORCHIC, COMBUSKEN, BLAZIKEN, //火稚鸡 力壮鸡 火焰鸡
            CHIMCHAR, MONFERNO, INFERNAPE, //小火焰猴 猛火猴 烈焰猴
            SQUIRTLE, WARTORTLE, BLASTOISE, //杰尼龟 卡咪龟 水箭龟
            MUDKIP, MARSHTOMP, SWAMPERT, //水跃鱼 沼跃鱼 巨沼怪
            PIPLUP, PRINPLUP, EMPOLEON, //波加曼 波皇子 帝王拿波
            BULBASAUR, IVYSAUR, VENUSAUR, //秒蛙种子 秒蛙草 妙蛙花
            TREECKO, GROVYLE, SCEPTILE, //木守宫 森林蜥蜴 蜥蜴王
            TURTWIG, GROTLE, TORTERRA, //草苗龟 树林龟 土台龟
            PICHU, PIKACHU, RAICHU, //皮丘 皮卡丘 雷丘
            SHINX, LUXIO, LUXRAY, //小猫怪 勒克猫 伦琴猫
            MARREP, FLAAFFY, AMPHAROS //咩利羊 绵绵 电龙
            };
const string kindOfString[] = {"Charamander", "Charmeleon", "Charizard",
                         "Torchic", "Combusken", "Blaziken",
                         "Chimchar", "Monferno", "Infernape",
                        "Squirtle", "Watortle", "Blastoise",
                         "Mudkip", "Marshtomp", "Swampert",
                         "Piplup", "Prinplup", "Empoleon",
                        "Bulbasaur", "Ivysaur", "Venusaur",
                         "Treecko", "Grovyle", "Sceptile",
                         "Turtwig", "Grotle", "Torterra",
                        "Pichu", "Pikachu", "Raichu",
                         "Shinx", "Luxio", "Luxray",
                         "Marrep", "Flaaffy", "Ampharos"};
//在对局时使用下列数组对应获得 Kind 中的枚举类型 例如(Kind1) == CHARMELEON
const int EmbryoPokemon[] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33}; //对战难度--简单 创建初级口袋妖怪
const int IntermediatePokemon[] = {1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34}; //对战难度--普通 创建中级口袋妖怪
const int LastPokemon[]= {2, 5, 8, 11, 14, 17, 20, 23, 26, 29, 32, 35}; //对战难度--困难 创建高级口袋妖怪
enum State {HEALTHY, DROWNED, BURNED, POISONED, FROZEN};
const string stateOfString[] = {"Healthy", "Drowned", "Burned", "Poisoned", "Frozen"};
enum Character {AGGRESIVE, DEFENSIVE, TANK, QUICK};
const string characterOfString[] = {"Aggresive", "Denfensive", "Tank", "Quick"};
enum evoLevel {EL1 = 6, EL2 = 13};
const int ExpGrade[17] = {0, 0, 50, 120, 210, 320, 500, 720,
                   1000, 1320, 1800, 2400, 3160, 3900,
                   4900, 6200};
const bool ALIVE = true, DEAD = false;
const string aliveOfString[] = {"Dead", "Alive"};
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
    Pokemon(){}

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

    void SpecialAttackDamage(Pokemon *dePokemon);// 特殊攻击当即造成伤害

    virtual void EnSick(Pokemon *sickPokemon){}// 特殊攻击致病

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
    Fire(Kind kind, int level, string name); //对战时随机生成的精灵构造函数 野生精灵构造函数
    ~Fire() {}
    void EnSick(Pokemon *sickPokemon);
    void SpecialAttack(Pokemon *dePokemon);
};

class Water : public Pokemon
{
public:
    //Water(Kind kind);
    Water(Kind kind, int level, string name); //对战时随机生成的精灵构造函数 野生精灵构造函数
    ~Water() {}
    void EnSick(Pokemon *sickPokemon);
    void SpecialAttack(Pokemon *dePokemon);
};

class Bush : public Pokemon
{
public:
    //Bush(Kind kind);
    Bush(Kind kind, int level, string name); //对战时随机生成的精灵构造函数 野生精灵构造函数
    ~Bush() {}
    void EnSick(Pokemon *sickPokemon);
    void SpecialAttack(Pokemon *dePokemon);
};

class Electricity : public Pokemon
{
public:
    //Electricity(Kind kind);
    Electricity(Kind kind, int level, string name); //对战时随机生成的精灵构造函数 野生精灵构造函数
    ~Electricity() {}
    void EnSick(Pokemon *sickPokemon);
    void SpecialAttack(Pokemon *dePokemon);
};

#endif // POKEMON_H
