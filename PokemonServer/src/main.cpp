#include <QCoreApplication>

#include "pokemon.h"
#include "pokemonfactory.h"

#include "player.h"
#include "playerfactory.h"

#include "catchunittest.h"
#include "./lib/sqlite3.h"

#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"

void PrintPokeData(Pokemon *pokemon) {
    cout << "Kind:" << kindOfString[pokemon->getKind()] <<  "\tName:" << pokemon->getName() << "\tAlive:" << aliveOfString[pokemon->getAlive()] <<
            "\tNature:" << natureOfString[pokemon->getNature()] << "\nCharacter:" << characterOfString[pokemon->getCharacter()] <<
            "\tLevel:" << pokemon->getLevel() << "\tExperiencePoint:"  << pokemon->getExperiencePoint() <<
            "\tAttackPoint:" << pokemon->getAttackPoint() << "\nDefencePoint:" << pokemon->getDefencePoint() <<
            "\tHP(C/T):" << pokemon->getCurrentHP() << "/" << pokemon->getTotalHP() <<
            "\tIntervalIncrease:" << pokemon->getIntervalIncrease() << "\tCriticalPoint:" << pokemon->getCriticalPoint() <<
            "\nState:" << stateOfString[pokemon->getState()] <<
            "\tSickPoint:" << pokemon->getSickPoint() << "\tSickCounter:" << pokemon->getSickCounter();

    int counterSetSize = pokemon->getCounterSet().size();
    cout << "\tCounterSet:";
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

void PrintPlayer(Player *player)
{
    cout << "Name: " << player->getName() << "\tPassword: (" << player->getPassword() << ") "
         << "\tPokemon Number: " << player->getPMNumber() << "\nRank: " << player->getRank()
         << "\tStart Time: " << player->getBeginDT().toString("yyyy/MM/dd hh:mm").toStdString()
         << "\tGame Time: " << player->getGameTime().substr(0, player->getGameTime().length() - 2) << "Hour "
         << player->getGameTime().substr(player->getGameTime().length() - 2, player->getGameTime().length()) << "Minute" << endl
         << "Pokemon Captured: ";
    for (Pokemon* gp : player->getGotPokemon())
    {
        cout << kindOfString[gp->getKind()] << "-" << gp->getName() << " ";
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /* Unit Test */
    int result = Catch::Session().run( argc, argv );

    PokemonFactory *pokemonFactory = new PokemonFactory();
    cout << "Please name your pokemon: ";
    string name;
    cin >> name;
    Pokemon *charamander = pokemonFactory->CreatePokemon(CHARMELEON, 6, name);
    cout << "Please name your pokemon: ";
    cin >> name;
    Pokemon *pikachu = pokemonFactory->CreatePokemon(RAICHU, 15, name);
    /* An apparent visual test
     * including Attack specialAttack database display */
    {
        PrintPokeData(charamander);
        PrintPokeData(pikachu);
        pikachu->Attack(charamander);
        charamander->DeadJudge();
        PrintPokeData(charamander);
        charamander->SpecialAttack(pikachu);
        charamander->DeadJudge();
        PrintPokeData(pikachu);
        struct PokemonInfo pokemonInfo = {
            "RainING Pokemon",0, 1, 0, 7, 900, 120,
            38, 678, 467, 34, 25, 2, 1, 2, 1 };
        Pokemon *RPokemon = pokemonFactory->CreatePokemon(pokemonInfo);
        PrintPokeData(RPokemon);

        Poor_ORM::ORMapper<PokemonInfo> pokemonMapper ("pokemoninfo.db");
        pokemonMapper.CreateTable();
        pokemonMapper.Insert(pokemonInfo);

        pokemonInfo = {
            "Father Pokemon",0, 1, 0, 7, 1200, 120,
            38, 678, 467, 34, 25, 2, 0, 0, 1 };
        pokemonMapper.Insert(pokemonInfo);

        PokemonInfo pHelper;
        auto pokemonQuery = pokemonMapper.Query(pHelper)
                .ToVector();
        for (auto c : pokemonQuery)
            cout << "PokemonInfo from DB:" << natureOfString[c.nature] <<
                    " " << kindOfString[c.kind] << " " << c.name <<
                    " " << characterOfString[c.character] << " " << c.level <<
                    " " << c.experiencePoint << " " << c.attackPoint <<
                    " " << c.defencePoint << " " << c.totalHP <<
                    " " << c.currentHP << " " << c.intervalIncrease <<
                    " " << c.criticalPoint << " " << stateOfString[c.state] <<
                    " " << c.sickCounter << " " << c.sickPoint <<
                    " " << aliveOfString[c.alive] << endl;
        delete RPokemon;
    }

    /* An apparent visual test for Player */
    {
        PlayerFactory *playerFactory = new PlayerFactory();
        struct PlayerInfo playerInfo = {
            "Lee",
            "2014232",
            0,
            9999,
            "201611171230", //201611171230 2016-11-17 12:30
            "122300" //1223hours 00minutes
        };
        Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
        playerMapper.CreateTable();
        playerMapper.Insert(playerInfo);
        PlayerInfo qHelper;
        auto query = playerMapper.Query(qHelper)
                .ToVector();
        for (auto c : query)
            cout << "PlayerInfo from DB:" << c.name << " " << c.password <<
                    " " << c.pokemonNumber << " " << c.rank <<
                    " " << c.beginDateTime << " " << c.gameTime << endl << endl;

        Player *p = playerFactory->CreatePlayer(playerInfo);
        p->addPokemon(pikachu);
        p->addPokemon(charamander);
        PrintPlayer(p);
        delete p;
        delete playerFactory;
    }

    delete pikachu;
    delete charamander;
    delete pokemonFactory;

    return a.exec();
}
