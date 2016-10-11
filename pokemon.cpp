﻿#define _CRT_RAND_S
#include "pokemon.h"

using namespace std;
/* srand+rand 精度保持在1秒 1秒内生成的随机数相同
 * rand_s 1秒内可以产生不同随机数*/
unsigned int Random(int max) {
    errno_t err;
        unsigned int number;
        err = rand_s(&number);
        if(err != 0)
        {
          return 0;
        }
        return (unsigned int)((double)number / ((double)UINT_MAX + 1) * double(max)) + 1;
}

/* 攻击函数
 * 暴击双倍伤害 依小概率造成烧伤，冻伤，中毒和麻痹效果 火属性不能对火属性造成烧伤 其他属性类比*/
void Pokemon::Attack(Pokemon *dePokemon) {
    int attackDamage = 0;
    if (this->CriticalStrike()) {
        attackDamage = (2* this->getAttackPoint())- dePokemon->getDefencePoint();
    }
    else {
        attackDamage = this->getAttackPoint()- dePokemon->getDefencePoint();
    }
    if (attackDamage < 0) {
        attackDamage = 0;
    }
    //cout << "dePokemon HP before attack:" << dePokemon->getCurrentHP();
    dePokemon->setCurrentHP(dePokemon->getCurrentHP()- attackDamage);
    //cout << "  attackDamage:" << attackDamage << "  dePokemon CurrentHP" << dePokemon->getCurrentHP() << endl;
}

void Pokemon::SpecialAttackDamage(Pokemon *dePokemon) {
    int specialDamage = this->getAttackPoint() - dePokemon->getDefencePoint();
    if (this->getCounterSet().count(dePokemon->getNature())) {
        specialDamage *= 3;
    }
    else if (dePokemon->getCounterSet().count(this->getNature())){
        specialDamage = (int)0.25* specialDamage;
    }
    if (specialDamage < 0)
        specialDamage = 0;
    dePokemon->setCurrentHP(dePokemon->getCurrentHP() - specialDamage);
}

int randFunction() {
    const int MIN = 0;
    const int MAX = 99;
    return MIN + Random(MAX - MIN);
}

bool Pokemon::EnSickPossible() {
    int randNum = randFunction();
    if (randNum <= 19) {
        cout << "EnSick True" << endl;
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
        return true;
    }
    cout << "EnSick False" << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
    return false;
}

bool Pokemon::CriticalStrike() {
    int randNum = randFunction();
    if (randNum <= this->getCriticalPoint()) {
        cout << "critical true" << endl;
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
        return true;
    }
    cout << "critical false" << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
    return false;
}

//持续伤害函数 不健康状态下每回合造成伤害 伤害大小取决于造成持续伤害时攻击方的攻击力
void Pokemon::Hurt()
{
    if (this->getState() != HEALTHY) {
        this->setCurrentHP(this->getCurrentHP() - this->getSickPoint());
        this->setSickCounter(this->getSickCounter()- 1);
    }
}

void Pokemon::DeadJudge()
{
    if (this->getCurrentHP() <= 0) {
        this->setAlive(DEAD);
        this->setCurrentHP(0);
    }
}

bool isInStandard(Kind kind, const int standard[]) {
    for (int i = 0; i < 36; i++) {
        if (kind == standard[i])
            return true;
    }
    return false;
}

void Pokemon::Upgrade()
{
    this->setLevel(this->getLevel()+ 1);
    cout << "Kind: " << kindOfString[this->getKind()] << "  Level:" << this->getLevel() << "  Kind:" << kindOfString[this->getKind()] << endl;
    switch (this->getCharacter()) {
    case AGGRESIVE:
        this->setAttackPoint(this->getAttackPoint() + UpGradeAttrIncStrong);
        this->setDefencePoint(this->getDefencePoint() + UpGradeAttrIncNormal);
        this->setTotalHP(this->getTotalHP() + UpGradeAttrIncNormal * 2);
        this->setIntervalIncrease(this->getIntervalIncrease() + UpGradeAttrIncWeak);
        break;
    case DEFENSIVE:
        this->setAttackPoint(this->getAttackPoint() + UpGradeAttrIncNormal);
        this->setDefencePoint(this->getDefencePoint() + UpGradeAttrIncStrong);
        this->setTotalHP(this->getTotalHP() + UpGradeAttrIncNormal * 2);
        this->setIntervalIncrease(this->getIntervalIncrease() + UpGradeAttrIncWeak);
        break;
    case TANK:
        this->setAttackPoint(this->getAttackPoint() + UpGradeAttrIncNormal);
        this->setDefencePoint(this->getDefencePoint() + UpGradeAttrIncNormal);
        this->setTotalHP(this->getTotalHP() + UpGradeAttrIncStrong * 2);
        this->setIntervalIncrease(this->getIntervalIncrease() + UpGradeAttrIncWeak);
        break;
    case QUICK:
        this->setAttackPoint(this->getAttackPoint() + UpGradeAttrIncNormal);
        this->setDefencePoint(this->getDefencePoint() + UpGradeAttrIncNormal);
        this->setTotalHP(this->getTotalHP() + UpGradeAttrIncNormal * 2);
        this->setIntervalIncrease(this->getIntervalIncrease() + UpGradeAttrIncNormal);
        break;
    default:
        cout << "只包含 攻击 防御 肉盾 敏捷 四种性格 如有改动请修改case语句" << endl;
    };
    this->setCriticalPoint(this->getCriticalPoint() + (this->getLevel()% 3== 0? 5: 0));
    if (this->getLevel() == EL1) {
        this->Evolution(EL1);
    }
    if (this->getLevel() == EL2 && !(isInStandard(this->getKind(), LastPokemon))) {
        this->Evolution(EL2);
    }
}

//换图标 换Kind
void Pokemon::Evolution(int evoLevel) {
    //cout << kindOfString[this->getKind()];
    this->setKind((Kind)(this->getKind()+ 1));
    switch (evoLevel) {
        case EL1:
            this->setAttackPoint((int) this->getAttackPoint()* 1.2);
            this->setDefencePoint((int) this->getDefencePoint()* 1.2);
            this->setTotalHP((int) this->getTotalHP()* 1.5);
            break;
        case EL2:
            this->setAttackPoint((int) this->getAttackPoint()* 1.5);
            this->setDefencePoint((int) this->getDefencePoint()* 1.5);
            this->setTotalHP((int) this->getTotalHP()* 1.8);
            break;
        default:
            cout << "Errro In Evolution Function: illegal evolution level" << endl;
            break;
    };
    //cout<< " is evoluting to " << kindOfString[this->getKind()] << endl;
}

int GiftGenFunc(int MIN, int MAX) {
    return MIN + Random(MAX - MIN);
}

//御三家构造时经验值为零 模拟对战 或者 野外抓捕时为随机经验值 在相应的类函数解决
Fire::Fire(Kind kind, int level) {
    this->setNature(FIRE);
    this->setKind(kind);
    //this->setName("UNKNOWN");
    this->setCharacter((Character)GiftGenFunc(0, 3));
    this->setLevel(1);
    this->setExperiencePoint(0);
    this->setAttackPoint(GiftGenFunc(20, 30));
    this->setDefencePoint(GiftGenFunc(10, 20));
    this->setTotalHP(GiftGenFunc(50, 60));
    this->setCurrentHP(this->getTotalHP());
    this->setIntervalIncrease(GiftGenFunc(2, 4));
    this->setCriticalPoint(10);
    set<Nature> counterNature;
    counterNature.insert(BUSH);
    this->setCounter(counterNature);
    this->setState(HEALTHY);
    this->setSickPoint(0);
    this->setSickCounter(0);
    this->setAlive(ALIVE);
    while (this->getLevel() < level) {
        //cout << "Kind: " << kindOfString[this->getKind()] << "  Level:" << this->getLevel() << endl;
        this->Upgrade();
    }
    this->setCurrentHP(this->getTotalHP());
    //cout << "Kind" << kindOfString[this->getKind()] << "Level:" << this->getLevel() << "  Defence Point:" << this->getDefencePoint() << "  TotalHP:" << this->getTotalHP() << endl;
}

void Fire::SpecialAttack(Pokemon *dePokemon) {
    this->SpecialAttackDamage(dePokemon);
    if (EnSickPossible()) {
        this->EnSick(dePokemon);
    }
}

void Fire::EnSick(Pokemon *sickPokemon) {
    sickPokemon->setState(BURNED);
    sickPokemon->setSickPoint((int)(0.25* this->getAttackPoint()));
    sickPokemon->setSickCounter(3);
}

Water::Water(Kind kind, int level) {
    this->setNature(WATER);
    this->setKind(kind);
    //this->setName("UNKNOWN");
    this->setCharacter((Character)GiftGenFunc(0, 3));
    this->setLevel(1);
    this->setExperiencePoint(0);
    this->setAttackPoint(GiftGenFunc(20, 30));
    this->setDefencePoint(GiftGenFunc(10, 20));
    this->setTotalHP(GiftGenFunc(50, 60));
    this->setCurrentHP(this->getTotalHP());
    this->setIntervalIncrease(GiftGenFunc(2, 4));
    this->setCriticalPoint(10);
    set<Nature> counterNature;
    counterNature.insert(FIRE);
    this->setCounter(counterNature);
    this->setState(HEALTHY);
    this->setSickPoint(0);
    this->setSickCounter(0);
    this->setAlive(ALIVE);
    while (this->getLevel() < level) {
        this->Upgrade();
    }
    this->setCurrentHP(this->getTotalHP());
}

void Water::SpecialAttack(Pokemon *dePokemon) {
    this->SpecialAttackDamage(dePokemon);
    if (EnSickPossible()) {
        this->EnSick(dePokemon);
    }
}

void Water::EnSick(Pokemon *sickPokemon) {
    sickPokemon->setState(DROWNED);
    sickPokemon->setSickPoint((int) 0.2* this->getAttackPoint());
    sickPokemon->setSickCounter(3);
}

Bush::Bush(Kind kind, int level) {
    this->setNature(BUSH);
    this->setKind(kind);
    //this->setName("UNKNOWN");
    this->setCharacter((Character)GiftGenFunc(0, 3));
    this->setLevel(1);
    this->setExperiencePoint(0);
    this->setAttackPoint(GiftGenFunc(20, 30));
    this->setDefencePoint(GiftGenFunc(10, 20));
    this->setTotalHP(GiftGenFunc(50, 60));
    this->setCurrentHP(this->getTotalHP());
    this->setIntervalIncrease(GiftGenFunc(2, 4));
    this->setCriticalPoint(10);
    set<Nature> counterNature;
    counterNature.insert(WATER);
    counterNature.insert(ELECTRICITY);
    this->setCounter(counterNature);
    this->setState(HEALTHY);
    this->setSickPoint(0);
    this->setSickCounter(0);
    this->setAlive(ALIVE);
    while (this->getLevel() < level) {
        this->Upgrade();
    }
    this->setCurrentHP(this->getTotalHP());
}

void Bush::SpecialAttack(Pokemon *dePokemon) {
    this->SpecialAttackDamage(dePokemon);
    if (EnSickPossible()) {
        this->EnSick(dePokemon);
    }
}

void Bush::EnSick(Pokemon *sickPokemon) {
    sickPokemon->setState(POISONED);
    sickPokemon->setSickPoint((int) 0.2* this->getAttackPoint());
    sickPokemon->setSickCounter(3);
}

Electricity::Electricity(Kind kind, int level) {
    this->setNature(ELECTRICITY);
    this->setKind(kind);
    //this->setName("UNKNOWN");
    this->setCharacter((Character)GiftGenFunc(0, 3));
    this->setLevel(1);
    this->setExperiencePoint(0);
    this->setAttackPoint(GiftGenFunc(20, 30));
    this->setDefencePoint(GiftGenFunc(10, 20));
    this->setTotalHP(GiftGenFunc(50, 60));
    this->setCurrentHP(this->getTotalHP());
    this->setIntervalIncrease(GiftGenFunc(2, 4));
    this->setCriticalPoint(10);
    set<Nature> counterNature;
    counterNature.insert(WATER);
    this->setCounter(counterNature);
    this->setState(HEALTHY);
    this->setSickPoint(0);
    this->setSickCounter(0);
    this->setAlive(ALIVE);
    while (this->getLevel() < level) {
        //cout << "Kind" << kindOfString[this->getKind()] << "Level:" << this->getLevel() << "  Attack Point:" << this->getAttackPoint() << endl;
        this->Upgrade();
    }
    this->setCurrentHP(this->getTotalHP());
    //cout << "Kind" << kindOfString[this->getKind()] << "Level:" << this->getLevel() << "  Attack Point:" << this->getAttackPoint() << endl;
}

void Electricity::SpecialAttack(Pokemon *dePokemon) {
    this->SpecialAttackDamage(dePokemon);
    if (EnSickPossible()) {
        this->EnSick(dePokemon);
    }
}

void Electricity::EnSick(Pokemon *sickPokemon) {
    sickPokemon->setState(FROZEN);
    sickPokemon->setSickPoint((int) 0.2* this->getAttackPoint());
    sickPokemon->setSickCounter(3);
}

