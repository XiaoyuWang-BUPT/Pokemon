#include "pokemon.h"

using namespace std;

/* 攻击函数
 * 暴击双倍伤害 依小概率造成烧伤，冻伤，中毒和麻痹效果 火属性不能对火属性造成烧伤 其他属性类比*/
void Pokemon::Attack(Pokemon *dePokemon) {
    int attackDamage = this->getAttackPoint() - dePokemon->getDefencePoint();
    if (this->CriticalStrike()) {
        attackDamage *= 2;
    }
    dePokemon->setCurrentHP(dePokemon->getCurrentHP()- attackDamage);
}

int Pokemon::randFunction() {
    const int MIN = 0;
    const int MAX = 99;
    srand((unsigned)time(NULL));
    return MIN + rand() % (MAX + MIN - 1);
}

bool Pokemon::EnSickPossible() {
    int randNum = randFunction();
    if (randNum <= 19)
        return true;
    return false;
}

bool Pokemon::CriticalStrike() {
    int randNum = randFunction();
    if (randNum <= this->getCriticalPoint())
        return true;
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

