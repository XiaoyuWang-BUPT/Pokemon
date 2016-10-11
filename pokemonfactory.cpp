#include "pokemonfactory.h"
#include <string>

Pokemon *PokemonFactory::CreatePokemon(Kind kind, int level) {
    Pokemon *pokemonCreated;
    if (kind >= CHARMANDER && kind <= INFERNAPE) {
        pokemonCreated = new Fire(kind, level);
    }
    else if (kind >= SQUIRTLE && kind <= EMPOLEON) {
        pokemonCreated = new Water(kind, level);
    }
    else if (kind >= BULBASAUR && kind <= TORTERRA) {
        pokemonCreated = new Bush(kind, level);
    }
    else if (kind >= PICHU && kind <= AMPHAROS) {
        pokemonCreated = new Electricity(kind, level);
    }
    else {
        cout << "Can not find kind: \"" << kind << "\" in enum Kind" << endl;
    }
    return pokemonCreated;
}
