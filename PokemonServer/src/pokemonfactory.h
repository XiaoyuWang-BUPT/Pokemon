#ifndef POKEMONFACTORY_H
#define POKEMONFACTORY_H

#include "pokemon.h"

class PokemonFactory
{
public:
    PokemonFactory(){}
    ~PokemonFactory(){}
    Pokemon *CreatePokemon(Kind kind, int level, string name, string owner);
    Pokemon *CreatePokemon(struct PokemonInfo pokemonInfo);
};

#endif // POKEMONFACTORY_H
