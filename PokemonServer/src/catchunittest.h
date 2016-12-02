#ifndef CATCHUNITTEST_H
#define CATCHUNITTEST_H

#define CATCH_CONFIG_RUNNER  // This tells Catch to provide a main() - only do this in one cpp file
#include "./lib/catch.hpp"
#include "pokemonfactory.h"
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"

TEST_CASE("Pokemon Unit Test") {
    PokemonFactory *pokemonFactory4Test = new PokemonFactory();

    Pokemon *charmander4Test = pokemonFactory4Test->CreatePokemon(CHARMANDER, 4, "charmander4Test");
    Pokemon *charmeleon4Test = pokemonFactory4Test->CreatePokemon(CHARMELEON, 10, "charmeleon4Test");
    Pokemon *charizard4Test = pokemonFactory4Test->CreatePokemon(CHARIZARD, 14, "charizard4Test");

    SECTION("Experience Point Test")
    {
        REQUIRE( charmander4Test->getExperiencePoint() == ExpGrade[charmander4Test->getLevel()]);
        REQUIRE( charmeleon4Test->getExperiencePoint() == ExpGrade[charmeleon4Test->getLevel()]);
        REQUIRE( charizard4Test->getExperiencePoint() == ExpGrade[charizard4Test->getLevel()]);
    }
    SECTION("Attack Point Test")
    {
        int attackRangeMin = 0;
        int attackRangeMax = 0;
        {
            if (charmander4Test->getCharacter() == AGGRESIVE)
            {
                attackRangeMin = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong + 20;
                attackRangeMax = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong + 30;
            }
            else
            {
                attackRangeMin = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 20;
                attackRangeMax = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 30;
            }
            REQUIRE(charmander4Test->getAttackPoint() >= attackRangeMin);
            REQUIRE(charmander4Test->getAttackPoint() <= attackRangeMax);
        }
        {
            if (charmeleon4Test->getCharacter() == AGGRESIVE)
            {
                attackRangeMin = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong + 20;
                attackRangeMax = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong + 30;
            }
            else
            {
                attackRangeMin = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 20;
                attackRangeMax = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 30;
            }
            REQUIRE(charmeleon4Test->getAttackPoint() >= attackRangeMin);
            REQUIRE(charmeleon4Test->getAttackPoint() <= attackRangeMax);
        }
        {
            if (charizard4Test->getCharacter() == AGGRESIVE)
            {
                attackRangeMin = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong + 20;
                attackRangeMax = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong + 30;
            }
            else
            {
                attackRangeMin = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 20;
                attackRangeMax = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 30;
            }
            REQUIRE(charizard4Test->getAttackPoint() >= attackRangeMin);
            REQUIRE(charizard4Test->getAttackPoint() <= attackRangeMax);
        }

    }
    SECTION("Defence Point Test")
    {
        int defenceRangeMin = 0;
        int defenceRangeMax = 0;
        {
            if (charmander4Test->getCharacter() == DEFENSIVE)
            {
                defenceRangeMin = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong + 10;
                defenceRangeMax = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong + 20;
            }
            else
            {
                defenceRangeMin = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 10;
                defenceRangeMax = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 20;
            }
            REQUIRE(charmander4Test->getDefencePoint() >= defenceRangeMin);
            REQUIRE(charmander4Test->getDefencePoint() <= defenceRangeMax);
        }
        {
            if (charmeleon4Test->getCharacter() == DEFENSIVE)
            {
                defenceRangeMin = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong + 10;
                defenceRangeMax = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong + 20;
            }
            else
            {
                defenceRangeMin = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 10;
                defenceRangeMax = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 20;
            }
            REQUIRE(charmeleon4Test->getDefencePoint() >= defenceRangeMin);
            REQUIRE(charmeleon4Test->getDefencePoint() <= defenceRangeMax);
        }
        {
            if (charizard4Test->getCharacter() == DEFENSIVE)
            {
                defenceRangeMin = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong + 10;
                defenceRangeMax = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong + 20;
            }
            else
            {
                defenceRangeMin = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 10;
                defenceRangeMax = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 20;
            }
            REQUIRE(charizard4Test->getDefencePoint() >= defenceRangeMin);
            REQUIRE(charizard4Test->getDefencePoint() <= defenceRangeMax);
        }
    }
    SECTION("HP Test")
    {
        int HPRangeMin = 0;
        int HPRangeMax = 0;
        {
            if (charmander4Test->getCharacter() == TANK)
            {
                HPRangeMin = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong * 2 + 50;
                HPRangeMax = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong * 2 + 60;
            }
            else
            {
                HPRangeMin = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal * 2 + 50;
                HPRangeMax = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal * 2 + 60;
            }
            REQUIRE(charmander4Test->getTotalHP() >= HPRangeMin);
            REQUIRE(charmander4Test->getTotalHP() <= HPRangeMax);
        }
        {
            if (charmeleon4Test->getCharacter() == TANK)
            {
                HPRangeMin = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong * 2 + 50;
                HPRangeMax = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong * 2 + 60;
            }
            else
            {
                HPRangeMin = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal * 2 + 50;
                HPRangeMax = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal * 2 + 60;
            }
            REQUIRE(charmeleon4Test->getTotalHP() >= HPRangeMin);
            REQUIRE(charmeleon4Test->getTotalHP() <= HPRangeMax);
        }
        {
            if (charizard4Test->getCharacter() == TANK)
            {
                HPRangeMin = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong * 2 + 50;
                HPRangeMax = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncStrong * 2 + 60;
            }
            else
            {
                HPRangeMin = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal * 2 + 50;
                HPRangeMax = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal * 2 + 60;
            }
            REQUIRE(charizard4Test->getTotalHP() >= HPRangeMin);
            REQUIRE(charizard4Test->getTotalHP() <= HPRangeMax);
        }
    }
    SECTION("IntervalIncrease Test")
    {
        int IIRangeMin = 0;
        int IIRangeMax = 0;
        {
            if (charmander4Test->getCharacter() == QUICK)
            {
                IIRangeMin = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 2;
                IIRangeMax = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 4;
            }
            else
            {
                IIRangeMin = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncWeak + 2;
                IIRangeMax = (charmander4Test->getLevel() - 1) *
                        UpGradeAttrIncWeak + 4;
            }
            REQUIRE(charmander4Test->getIntervalIncrease() >= IIRangeMin);
            REQUIRE(charmander4Test->getIntervalIncrease() <= IIRangeMax);
        }
        {
            if (charmeleon4Test->getCharacter() == QUICK)
            {
                IIRangeMin = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 2;
                IIRangeMax = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 4;
            }
            else
            {
                IIRangeMin = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncWeak + 2;
                IIRangeMax = (charmeleon4Test->getLevel() - 1) *
                        UpGradeAttrIncWeak + 4;
            }
            REQUIRE(charmeleon4Test->getIntervalIncrease() >= IIRangeMin);
            REQUIRE(charmeleon4Test->getIntervalIncrease() <= IIRangeMax);
        }
        {
            if (charizard4Test->getCharacter() == QUICK)
            {
                IIRangeMin = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 2;
                IIRangeMax = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncNormal + 4;
            }
            else
            {
                IIRangeMin = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncWeak + 2;
                IIRangeMax = (charizard4Test->getLevel() - 1) *
                        UpGradeAttrIncWeak + 4;
            }
            REQUIRE(charizard4Test->getIntervalIncrease() >= IIRangeMin);
            REQUIRE(charizard4Test->getIntervalIncrease() <= IIRangeMax);
        }
    }
    SECTION("Critical Point Test")
    {
        int critical =10 + 5 * (charmander4Test->getLevel() / 3);
        REQUIRE(charmander4Test->getCriticalPoint() == critical);
        critical =10 + 5 * (charmeleon4Test->getLevel() / 3);
        REQUIRE(charmeleon4Test->getCriticalPoint() == critical);
        critical =10 + 5 * (charizard4Test->getLevel() / 3);
        REQUIRE(charizard4Test->getCriticalPoint() == critical);
    }
    SECTION("PokemonFactory Create Excuted--Right kind && Right level") {
        REQUIRE( charmander4Test->getKind() == CHARMANDER );
        REQUIRE( charmander4Test->getLevel() == 4 );
        REQUIRE( charmeleon4Test->getKind() == CHARMELEON );
        REQUIRE( charmeleon4Test->getLevel() == 10 );
        REQUIRE( charizard4Test->getKind() == CHARIZARD );
        REQUIRE( charizard4Test->getLevel() == 14 );
    }
    SECTION("PokemonFactory Create Excuted--Kind meet level") {
        REQUIRE( Helper::isKindMeetLevel(charmander4Test->getKind(), charmander4Test->getLevel()) == true);
        REQUIRE( Helper::isKindMeetLevel(charmeleon4Test->getKind(), charmeleon4Test->getLevel()) == true);
        REQUIRE( Helper::isKindMeetLevel(charizard4Test->getKind(), charizard4Test->getLevel()) == true);
    }
    SECTION("Attack Excuted--HP decrease(critical is unpredictable)") {
        charmeleon4Test->Attack(charmander4Test);
        int damage = charmeleon4Test->getAttackPoint() - charmander4Test->getDefencePoint();
        int criticalDamage = 2* charmeleon4Test->getAttackPoint() - charmander4Test->getDefencePoint();
        if (damage < 0)
            damage = 0;
        if (criticalDamage < 0)
            criticalDamage = 0;
        bool judgeBool = ((charmander4Test->getTotalHP() - charmander4Test->getCurrentHP()) == damage)||
                ((charmander4Test->getTotalHP() - charmander4Test->getCurrentHP()) == criticalDamage);
        REQUIRE(judgeBool);
    }
    //特殊攻击测试部分 只能测试火属性不能造成其他属性特效
    SECTION("Special Attack Excuted--State not beyond attack Pokemon can do(EnSick Possiblity is unpredictable)") {
        charmander4Test->SpecialAttack(charizard4Test);
        bool judgeBool = (charizard4Test->getState() == HEALTHY)||(charizard4Test->getState()==BURNED);
        REQUIRE(judgeBool);
    }
    SECTION("Upgrade Excuted--Level up") {
        int levelB4Upgrade = charmander4Test->getLevel();
        charmander4Test->Upgrade();
        REQUIRE(charmander4Test->getLevel() == levelB4Upgrade + 1);
    }
    SECTION("Evolution Excuted--Pokemon kind evolute") {
        int kindB4Upgrade = charmander4Test->getKind();
        charmander4Test->Upgrade();
        charmander4Test->Upgrade();
        REQUIRE(charmander4Test->getKind() == kindB4Upgrade + 1);
    }
    delete pokemonFactory4Test;
    delete charmander4Test;
    delete charmeleon4Test;
    delete charizard4Test;
}

#endif // CATCHUNITTEST_H
