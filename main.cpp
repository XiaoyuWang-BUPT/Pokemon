#include "widget.h"
#include <QApplication>

#include "pokemon.h"
#include "pokemonfactory.h"

void PrintPokeData(Pokemon *pokemon) {
    cout << "Kind:" << kindOfString[pokemon->getKind()] <<  "  Alive:" << aliveOfString[pokemon->getAlive()] <<
            "  Nature:" << natureOfString[pokemon->getNature()] << "  Character:" << characterOfString[pokemon->getCharacter()] <<
            "  Level:" << pokemon->getLevel() << "  experiencePoint:"  << pokemon->getExperiencePoint() <<
            "  Attack Point:" << pokemon->getAttackPoint() << "  Defence Point:" << pokemon->getDefencePoint() <<
            "  HP(C/T):" << pokemon->getCurrentHP() << "/" << pokemon->getTotalHP() <<
            "  intervalIncrease:" << pokemon->getIntervalIncrease() << "  Critical Point:" << pokemon->getCriticalPoint() <<
            "  State:" << stateOfString[pokemon->getState()] <<
            "  Sick Point:" << pokemon->getSickPoint() << "  Sick Counter:" << pokemon->getSickCounter();

    int counterSetSize = pokemon->getCounterSet().size();
    cout << "  Counter Set:";
    set<Nature>::iterator it = pokemon->getCounterSet().begin();
    for (int i = 0; i< counterSetSize; i++) {
        cout << natureOfString[*it];
        if (i < counterSetSize- 1)
            cout << "-";
        it++;
    }
    cout << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    //w.show();//调试先不显示窗口 第三阶段显示窗口

    PokemonFactory *pokemonFactory = new PokemonFactory();
    Pokemon *charamander = pokemonFactory->CreatePokemon(CHARMANDER, 6);
    Pokemon *pikachu = pokemonFactory->CreatePokemon(PIKACHU, 15);

    PrintPokeData(charamander);
    PrintPokeData(pikachu);
    pikachu->Attack(charamander);
    charamander->DeadJudge();
    PrintPokeData(charamander);

    charamander->SpecialAttack(pikachu);
    charamander->DeadJudge();
    PrintPokeData(pikachu);

    delete pikachu;
    delete charamander;
    delete pokemonFactory;

    return a.exec();
}
