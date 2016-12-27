#ifndef POKEMONFACTORY_H
#define POKEMONFACTORY_H

#include "pokemon.h"

class PokemonFactory
{
public:
    PokemonFactory(){}
    ~PokemonFactory(){}

    /**
     * Function : CreatePokemon
     * Description : create pokemon according to its kind, level, name, owner
     * Input : kind, level, name, owner
     * Return : Pokemon* objbect
     * Other : None
    **/
    Pokemon *CreatePokemon(Kind kind, int level, string name, string owner);

    /**
     * Function : CreatePokemon
     * Description : create pokemon objetc according to struct PokemonInfo
     * Input : struct PokemonInfo
     * Return : Pokemon* object
     * Other : None
    **/
    Pokemon *CreatePokemon(struct PokemonInfo pokemonInfo);
};

#endif // POKEMONFACTORY_H
