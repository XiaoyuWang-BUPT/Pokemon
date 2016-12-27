#include "pokemonfactory.h"
#include <string>

Pokemon *PokemonFactory::CreatePokemon(Kind kind, int level, string name, string owner) {
    Pokemon *pokemonCreated;
    //cerate fire pokemon
    if (kind >= CHARMANDER && kind <= INFERNAPE) {
        if (Helper::isKindMeetLevel(kind, level))
        {
            pokemonCreated = new Fire(kind, level, name, owner);
        }
        else
            cout << "Kind does NOT meet level" << endl;
    }

    //create water pokemon
    else if (kind >= SQUIRTLE && kind <= EMPOLEON) {
        if (Helper::isKindMeetLevel(kind, level))
        {
            pokemonCreated = new Water(kind, level, name, owner);
        }
        else
            cout << "Kind does NOT meet level" << endl;
    }

    //create bush pokemon
    else if (kind >= BULBASAUR && kind <= TORTERRA) {
        if (Helper::isKindMeetLevel(kind, level))
        {
            pokemonCreated = new Bush(kind, level, name, owner);
        }
        else
            cout << "Kind does NOT meet level" << endl;
    }

    //create electricity pokemon
    else if (kind >= PICHU && kind <= AMPHAROS) {
        if (Helper::isKindMeetLevel(kind, level))
        {
            pokemonCreated = new Electricity(kind, level, name, owner);
        }
        else
            cout << "Kind does NOT meet level" << endl;
    }
    else {
        cout << "Can not find kind: \"" << kind << "\" in enum Kind" << endl;
    }
    return pokemonCreated;
}

Pokemon *PokemonFactory::CreatePokemon(PokemonInfo pokemonInfo)
{
    Pokemon *pokemonCreated;
    Kind kind = (Kind)pokemonInfo.kind;
    int level = pokemonInfo.level;
    //create fire pokemon
    if (kind >= CHARMANDER && kind <= INFERNAPE) {
        if (Helper::isKindMeetLevel(kind, level))
        {
            pokemonCreated = new Fire(pokemonInfo);
        }
        else
            cout << "Kind does NOT meet level" << endl;
    }
    //create water pokemon
    else if (kind >= SQUIRTLE && kind <= EMPOLEON) {
        if (Helper::isKindMeetLevel(kind, level))
        {
            pokemonCreated = new Water(pokemonInfo);
        }
        else
            cout << "Kind does NOT meet level" << endl;
    }
    //create bush pokemon
    else if (kind >= BULBASAUR && kind <= TORTERRA) {
        if (Helper::isKindMeetLevel(kind, level))
        {
            pokemonCreated = new Bush(pokemonInfo);
        }
        else
            cout << "Kind does NOT meet level" << endl;
    }
    //create electricity pokemon
    else if (kind >= PICHU && kind <= AMPHAROS) {
        if (Helper::isKindMeetLevel(kind, level))
        {
            pokemonCreated = new Electricity(pokemonInfo);
        }
        else
            cout << "Kind does NOT meet level" << endl;
    }
    else {
        cout << "Can not find kind: \"" << kind << "\" in enum Kind" << endl;
    }
    return pokemonCreated;
}
