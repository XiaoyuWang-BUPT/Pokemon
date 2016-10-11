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
    int attackDamage = this->getAttackPoint() - dePokemon->getDefencePoint();
    if (this->CriticalStrike()) {
        attackDamage *= 2;
    }
    if (attackDamage < 0) {
        attackDamage = 0;
    }
    //cout << "dePokemon HP before attack:" << dePokemon->getCurrentHP();
    dePokemon->setCurrentHP(dePokemon->getCurrentHP()- attackDamage);
    //cout << "  attackDamage:" << attackDamage << "  dePokemon CurrentHP" << dePokemon->getCurrentHP() << endl;
}

int randFunction() {
    const int MIN = 0;
    const int MAX = 99;
    return MIN + Random(MAX - MIN);
}

bool Pokemon::EnSickPossible() {
    int randNum = randFunction();
    if (randNum <= 19)
        return true;
    return false;
}

bool Pokemon::CriticalStrike() {
    int randNum = randFunction();
    if (randNum <= this->getCriticalPoint()) {
        cout << "critical true" << endl;
        return true;
    }
    cout << "critical false" << endl;
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

void Pokemon::Upgrade()
{
    this->setLevel(this->getLevel()+ 1);
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
        this->setAttackPoint((int) this->getAttackPoint()* 1.2);
        this->setDefencePoint((int) this->getDefencePoint()* 1.2);
        this->setTotalHP((int) this->getTotalHP()* 1.5);
        this->Evolution(EL1);
    }
    if (this->getLevel() == EL2) {
        this->setAttackPoint((int) this->getAttackPoint()* 1.5);
        this->setDefencePoint((int) this->getDefencePoint()* 1.5);
        this->setTotalHP((int) this->getTotalHP()* 1.8);
        this->Evolution(EL2);
    }
}

int GiftGenFunc(int MIN, int MAX) {
    return MIN + Random(MAX - MIN);
}

bool isInStandard(Kind kind, int standard[]) {
    for (int i = 0; i < 36; i++) {
        if (kind == standard[i])
            return true;
    }
    return false;
}

//Fire::Fire(Kind kind) {
//    this->setNature(FIRE);
//    this->setKind(kind);
//    this->setName("UNKNOWN");
//    this->setCharacter((Character)GiftGenFunc(0, 3));
//    this->setLevel(1);
//    this->setExperiencePoint(0);
//    this->setAttackPoint(GiftGenFunc(20, 30));
//    this->setDefencePoint(GiftGenFunc(10, 20));
//    this->setTotalHP(GiftGenFunc(50, 60));
//    this->setCurrentHP(this->getTotalHP());
//    this->setIntervalIncrease(GiftGenFunc(2, 4));
//    this->setCriticalPoint(10);
//    set<Nature> counterNature;
//    counterNature.insert(BUSH);
//    this->setCounter(counterNature);
//    this->setState(HEALTHY);
//    this->setSickPoint(0);
//    this->setSickPoint(0);
//    this->setAlive(ALIVE);
//}

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
    this->setSickPoint(0);
    this->setAlive(ALIVE);
    while (this->getLevel() < level) {
        //cout << "Kind" << kindOfString[this->getKind()] << "Level:" << this->getLevel() << "  Defence Point:" << this->getDefencePoint() << "  TotalHP:" << this->getTotalHP() << endl;
        this->Upgrade();
    }
    this->setCurrentHP(this->getTotalHP());
    //cout << "Kind" << kindOfString[this->getKind()] << "Level:" << this->getLevel() << "  Defence Point:" << this->getDefencePoint() << "  TotalHP:" << this->getTotalHP() << endl;
}

void Fire::SpecialAttack(Pokemon *dePokemon) {
    if (this->getCounterSet().count(dePokemon->getNature())) {
        dePokemon->setCurrentHP(dePokemon->getCurrentHP() - 3* (this->getAttackPoint() - dePokemon->getDefencePoint()));
    }
    else if (dePokemon->getCounterSet().count(this->getNature())){
        dePokemon->setCurrentHP(dePokemon->getCurrentHP() - (int)0.25* (this->getAttackPoint() - dePokemon->getDefencePoint()));
    }
    else {
        dePokemon->setCurrentHP(dePokemon->getCurrentHP() - (this->getAttackPoint() - dePokemon->getDefencePoint()));
    }

    if (EnSickPossible()) {
        this->EnSick(dePokemon);
    }
}

void Fire::EnSick(Pokemon *sickPokemon) {
    sickPokemon->setState(BURNED);
    sickPokemon->setSickPoint((int) 0.2* this->getAttackPoint());
    sickPokemon->setSickCounter(3);
}

//Water::Water(Kind kind) {
//    this->setNature(WATER);
//    this->setKind(kind);
//    this->setName("UNKNOWN");
//    this->setCharacter((Character)GiftGenFunc(0, 3));
//    this->setLevel(1);
//    this->setExperiencePoint(0);
//    this->setAttackPoint(GiftGenFunc(20, 30));
//    this->setDefencePoint(GiftGenFunc(10, 20));
//    this->setTotalHP(GiftGenFunc(50, 60));
//    this->setCurrentHP(this->getTotalHP());
//    this->setIntervalIncrease(GiftGenFunc(2, 4));
//    this->setCriticalPoint(10);
//    set<Nature> counterNature;
//    counterNature.insert(FIRE);
//    this->setCounter(counterNature);
//    this->setState(HEALTHY);
//    this->setSickPoint(0);
//    this->setSickPoint(0);
//    this->setAlive(ALIVE);
//}

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
    this->setSickPoint(0);
    this->setAlive(ALIVE);
    while (this->getLevel() < level) {
        this->Upgrade();
    }
    this->setCurrentHP(this->getTotalHP());
}

void Water::SpecialAttack(Pokemon *dePokemon) {
    if (this->getCounterSet().count(dePokemon->getNature())) {
        dePokemon->setCurrentHP(dePokemon->getCurrentHP() - 3* (this->getAttackPoint() - dePokemon->getDefencePoint()));
    }
    else if (dePokemon->getCounterSet().count(this->getNature())){
        dePokemon->setCurrentHP(dePokemon->getCurrentHP() - (int)0.25* (this->getAttackPoint() - dePokemon->getDefencePoint()));
    }
    else {
        dePokemon->setCurrentHP(dePokemon->getCurrentHP() - (this->getAttackPoint() - dePokemon->getDefencePoint()));
    }

    if (EnSickPossible()) {
        this->EnSick(dePokemon);
    }
}

void Water::EnSick(Pokemon *sickPokemon) {
    sickPokemon->setState(DROWNED);
    sickPokemon->setSickPoint((int) 0.2* this->getAttackPoint());
    sickPokemon->setSickCounter(3);
}

//Bush::Bush(Kind kind) {
//    this->setNature(BUSH);
//    this->setKind(kind);
//    this->setName("UNKNOWN");
//    this->setCharacter((Character)GiftGenFunc(0, 3));
//    this->setLevel(1);
//    this->setExperiencePoint(0);
//    this->setAttackPoint(GiftGenFunc(20, 30));
//    this->setDefencePoint(GiftGenFunc(10, 20));
//    this->setTotalHP(GiftGenFunc(50, 60));
//    this->setCurrentHP(this->getTotalHP());
//    this->setIntervalIncrease(GiftGenFunc(2, 4));
//    this->setCriticalPoint(10);
//    set<Nature> counterNature;
//    counterNature.insert(WATER);
//    counterNature.insert(ELECTRICITY);
//    this->setCounter(counterNature);
//    this->setState(HEALTHY);
//    this->setSickPoint(0);
//    this->setSickPoint(0);
//    this->setAlive(ALIVE);
//}

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
    this->setSickPoint(0);
    this->setAlive(ALIVE);
    while (this->getLevel() < level) {
        this->Upgrade();
    }
    this->setCurrentHP(this->getTotalHP());
}

void Bush::SpecialAttack(Pokemon *dePokemon) {
    if (this->getCounterSet().count(dePokemon->getNature())) {
        dePokemon->setCurrentHP(dePokemon->getCurrentHP() - 3* (this->getAttackPoint() - dePokemon->getDefencePoint()));
    }
    else if (dePokemon->getCounterSet().count(this->getNature())){
        dePokemon->setCurrentHP(dePokemon->getCurrentHP() - (int)0.25* (this->getAttackPoint() - dePokemon->getDefencePoint()));
    }
    else {
        dePokemon->setCurrentHP(dePokemon->getCurrentHP() - (this->getAttackPoint() - dePokemon->getDefencePoint()));
    }

    if (EnSickPossible()) {
        this->EnSick(dePokemon);
    }
}

void Bush::EnSick(Pokemon *sickPokemon) {
    sickPokemon->setState(POISONED);
    sickPokemon->setSickPoint((int) 0.2* this->getAttackPoint());
    sickPokemon->setSickCounter(3);
}

//Electricity::Electricity(Kind kind) {
//    this->setNature(ELECTRICITY);
//    this->setKind(kind);
//    this->setName("UNKNOWN");
//    this->setCharacter((Character)GiftGenFunc(0, 3));
//    this->setLevel(1);
//    this->setExperiencePoint(0);
//    this->setAttackPoint(GiftGenFunc(20, 30));
//    this->setDefencePoint(GiftGenFunc(10, 20));
//    this->setTotalHP(GiftGenFunc(50, 60));
//    this->setCurrentHP(this->getTotalHP());
//    this->setIntervalIncrease(GiftGenFunc(2, 4));
//    this->setCriticalPoint(10);
//    set<Nature> counterNature;
//    counterNature.insert(WATER);
//    this->setCounter(counterNature);
//    this->setState(HEALTHY);
//    this->setSickPoint(0);
//    this->setSickPoint(0);
//    this->setAlive(ALIVE);
//}

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
    this->setSickPoint(0);
    this->setAlive(ALIVE);
    while (this->getLevel() < level) {
        //cout << "Kind" << kindOfString[this->getKind()] << "Level:" << this->getLevel() << "  Attack Point:" << this->getAttackPoint() << endl;
        this->Upgrade();
    }
    this->setCurrentHP(this->getTotalHP());
    //cout << "Kind" << kindOfString[this->getKind()] << "Level:" << this->getLevel() << "  Attack Point:" << this->getAttackPoint() << endl;
}

void Electricity::SpecialAttack(Pokemon *dePokemon) {
    if (this->getCounterSet().count(dePokemon->getNature())) {
        dePokemon->setCurrentHP(dePokemon->getCurrentHP() - 3* (this->getAttackPoint() - dePokemon->getDefencePoint()));
    }
    else if (dePokemon->getCounterSet().count(this->getNature())){
        dePokemon->setCurrentHP(dePokemon->getCurrentHP() - (int)0.25* (this->getAttackPoint() - dePokemon->getDefencePoint()));
    }
    else {
        dePokemon->setCurrentHP(dePokemon->getCurrentHP() - (this->getAttackPoint() - dePokemon->getDefencePoint()));
    }

    if (EnSickPossible()) {
        this->EnSick(dePokemon);
    }
}

void Electricity::EnSick(Pokemon *sickPokemon) {
    sickPokemon->setState(FROZEN);
    sickPokemon->setSickPoint((int) 0.2* this->getAttackPoint());
    sickPokemon->setSickCounter(3);
}

