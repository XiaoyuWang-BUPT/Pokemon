#include "pokemonfactory.h"
#include <string>

//判断Kind是否在口袋妖怪形态分级数组
bool isInStanding(Kind kind, const int standard[]) {
    for (int i = 0; i < 36; i++) {
        if (kind == standard[i])
            return true;
    }
    return false;
}

//用于判断create时等级和形态是否对应
bool isKindAgreeLevel(Kind kind, int level) {
    if ((level < 6 && isInStanding(kind, EmbryoPokemon))||
            (level >= 6 && level < 13 && isInStanding(kind, IntermediatePokemon))||
            (level >= 13 && isInStanding(kind, LastPokemon)))
        return true;
    return false;
}

Pokemon *PokemonFactory::CreatePokemon(Kind kind, int level, string name) {
    Pokemon *pokemonCreated;
    if (kind >= CHARMANDER && kind <= INFERNAPE) {
        if (isKindAgreeLevel(kind, level))
            pokemonCreated = new Fire(kind, level, name);
        else
            cout << "Kind does NOT meet level" << endl;
    }
    else if (kind >= SQUIRTLE && kind <= EMPOLEON) {
        if (isKindAgreeLevel(kind, level))
            pokemonCreated = new Water(kind, level, name);
        else
            cout << "Kind does NOT meet level" << endl;
    }
    else if (kind >= BULBASAUR && kind <= TORTERRA) {
        if (isKindAgreeLevel(kind, level))
            pokemonCreated = new Bush(kind, level, name);
        else
            cout << "Kind does NOT meet level" << endl;
    }
    else if (kind >= PICHU && kind <= AMPHAROS) {
        if (isKindAgreeLevel(kind, level))
            pokemonCreated = new Electricity(kind, level, name);
        else
            cout << "Kind does NOT meet level" << endl;
    }
    else {
        cout << "Can not find kind: \"" << kind << "\" in enum Kind" << endl;
    }
    return pokemonCreated;
}
