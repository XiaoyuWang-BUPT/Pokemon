#include "widget.h"
#include <QApplication>

#include "pokemon.h"
#include "pokemonfactory.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    PokemonFactory *pokemonFactory = new PokemonFactory();
    Pokemon *charamander = pokemonFactory->CreatePokemon(CHARMANDER, 3);
    Pokemon *pikachu = pokemonFactory->CreatePokemon(PIKACHU, 4);
    cout << "Kind:" << kindOfString[pikachu->getKind()] << " Level:" << pikachu->getLevel() << "  Attack Point:" << pikachu->getAttackPoint() << "  Defence Point:" << pikachu->getDefencePoint() << "  TotalHP:" << pikachu->getTotalHP() << endl;
    cout << "Kind:" << kindOfString[charamander->getKind()] << " Level:" << charamander->getLevel() << "  Attack Point:" << charamander->getAttackPoint() << "  Defence Point:" << charamander->getDefencePoint() << "  TotalHP:" << charamander->getTotalHP() << endl;
    pikachu->Attack(charamander);
    cout << charamander->getCurrentHP() << "/" << charamander->getTotalHP() << endl;
    charamander->SpecialAttack(pikachu);
    cout << pikachu->getCurrentHP() << "/" << pikachu->getTotalHP() << endl;

    return a.exec();
}
