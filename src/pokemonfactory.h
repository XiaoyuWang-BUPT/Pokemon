#ifndef POKEMONFACTORY_H
#define POKEMONFACTORY_H

#include "pokemon.h"
#include "widget.h"

class PokemonFactory
{
public:
    PokemonFactory(){}
    ~PokemonFactory(){}
    Pokemon *CreatePokemon(Kind kind, int level);
};

#endif // POKEMONFACTORY_H
