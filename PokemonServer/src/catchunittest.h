#ifndef CATCHUNITTEST_H
#define CATCHUNITTEST_H

#define CATCH_CONFIG_RUNNER  // This tells Catch to provide a main() - only do this in one cpp file
#include "./lib/catch.hpp"
#include "pokemonfactory.h"

bool isInArray(Kind kind, const int standard[]) {
    for (int i = 0; i < 36; i++) {
        if (kind == standard[i])
            return true;
    }
    return false;
}

bool isKindCorrespondLevel(Kind kind, int level) {
    if ((level < 6 && isInArray(kind, EmbryoPokemon))||
            (level >= 6 && level < 13 && isInArray(kind, IntermediatePokemon))||
            (level >= 13 && isInArray(kind, LastPokemon)))
        return true;
    return false;
}

TEST_CASE("Pokemon Unit Test") {
    PokemonFactory *pokemonFactory4Test = new PokemonFactory();

    Pokemon *charmander4Test = pokemonFactory4Test->CreatePokemon(CHARMANDER, 4, "charmander4Test");
    Pokemon *charmeleon4Test = pokemonFactory4Test->CreatePokemon(CHARMELEON, 10, "charmeleon4Test");
    Pokemon *charizard4Test = pokemonFactory4Test->CreatePokemon(CHARIZARD, 14, "charizard4Test");

    SECTION("PokemonFactory Create Excuted--Right kind && Right level(Hard to judge others)") {
        REQUIRE( charmander4Test->getKind() == CHARMANDER );
        REQUIRE( charmander4Test->getLevel() == 4 );
        REQUIRE( charmeleon4Test->getKind() == CHARMELEON );
        REQUIRE( charmeleon4Test->getLevel() == 10 );
        REQUIRE( charizard4Test->getKind() == CHARIZARD );
        REQUIRE( charizard4Test->getLevel() == 14 );
    }
    SECTION("PokemonFactory Create Excuted--Kind meet level") {
        REQUIRE( isKindCorrespondLevel(charmander4Test->getKind(), charmander4Test->getLevel()) == true);
        REQUIRE( isKindCorrespondLevel(charmeleon4Test->getKind(), charmeleon4Test->getLevel()) == true);
        REQUIRE( isKindCorrespondLevel(charizard4Test->getKind(), charizard4Test->getLevel()) == true);
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
        //cout <<  "level:"<< charmander4Test->getLevel() << " kind:" << kindOfString[charmander4Test->getKind()] << endl;
        REQUIRE(charmander4Test->getKind() == kindB4Upgrade + 1);
    }
    delete pokemonFactory4Test;
    delete charmander4Test;
    delete charmeleon4Test;
    delete charizard4Test;
}

#endif // CATCHUNITTEST_H
