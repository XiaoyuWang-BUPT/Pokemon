#include "widget.h"
#include <QApplication>

#include "pokemon.h"
#include "pokemonfactory.h"

#include "catchunittest.h"

void PrintPokeData(Pokemon *pokemon) {
    cout << "Kind:" << kindOfString[pokemon->getKind()] <<  "\tAlive:" << aliveOfString[pokemon->getAlive()] <<
            "\tNature:" << natureOfString[pokemon->getNature()] << "\tCharacter:" << characterOfString[pokemon->getCharacter()] <<
            "\tLevel:" << pokemon->getLevel() << "\texperiencePoint:"  << pokemon->getExperiencePoint() <<
            "\tAttack Point:" << pokemon->getAttackPoint() << "\tDefence Point:" << pokemon->getDefencePoint() <<
            "\tHP(C/T):" << pokemon->getCurrentHP() << "/" << pokemon->getTotalHP() <<
            "\tintervalIncrease:" << pokemon->getIntervalIncrease() << "\tCritical Point:" << pokemon->getCriticalPoint() <<
            "\tState:" << stateOfString[pokemon->getState()] <<
            "\tSick Point:" << pokemon->getSickPoint() << "\tSick Counter:" << pokemon->getSickCounter();

    int counterSetSize = pokemon->getCounterSet().size();
    cout << "\tCounter Set:";
    set<Nature>::iterator it = pokemon->getCounterSet().begin();
    for (int i = 0; i< counterSetSize; i++) {
        cout << natureOfString[*it];
        if (i < counterSetSize- 1)
            cout << "-";
        it++;
    }
    cout << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
//    w.show();//调试先不显示窗口 第三阶段显示窗口

    PokemonFactory *pokemonFactory = new PokemonFactory();
    Pokemon *charamander = pokemonFactory->CreatePokemon(CHARMELEON, 6);
    Pokemon *pikachu = pokemonFactory->CreatePokemon(RAICHU, 15);

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

    int result = Catch::Session().run( argc, argv );

    return a.exec();
}
