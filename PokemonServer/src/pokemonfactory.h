#ifndef POKEMONFACTORY_H
#define POKEMONFACTORY_H

#include "pokemon.h"

class PokemonFactory
{
public:
    PokemonFactory(){}
    ~PokemonFactory(){}
    Pokemon *CreatePokemon(Kind kind, int level, string name);
};

#endif // POKEMONFACTORY_H
