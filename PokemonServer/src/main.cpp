#include <QCoreApplication>

#include "session.h"
#include "pokemonfactory.h"
#include "playerfactory.h"
#include "catchunittest.h"

using json = nlohmann::json;

std::thread threads[MAXSIZE_POOL];

#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"

/**
 * Function : PrintPokeData
 * Description : print Pokemon* object
 * Input : Pokemon*
 * Return : None
 * Other : None
**/
void PrintPokeData(Pokemon *pokemon) {
    cout << "Kind:" << kindOfString[pokemon->getKind()] <<  "\tName:" << pokemon->getName() << "\tAlive:" << aliveOfString[pokemon->getAlive()] <<
            "\tNature:" << natureOfString[pokemon->getNature()] << "\nCharacter:" << characterOfString[pokemon->getCharacter()] <<
            "\tLevel:" << pokemon->getLevel() << "\tExperiencePoint:"  << pokemon->getExperiencePoint() <<
            "\tAttackPoint:" << pokemon->getAttackPoint() << "\nDefencePoint:" << pokemon->getDefencePoint() <<
            "\tHP(C/T):" << pokemon->getCurrentHP() << "/" << pokemon->getTotalHP() <<
            "\tIntervalIncrease:" << pokemon->getIntervalIncrease() << "\tCriticalPoint:" << pokemon->getCriticalPoint() <<
            "\nState:" << stateOfString[pokemon->getState()] <<
            "\tSickPoint:" << pokemon->getSickPoint() << "\tSickCounter:" << pokemon->getSickCounter();
    cout << "\tCounterVec:";
    for (auto c : pokemon->getCounterVec())
    {
        cout << natureOfString[c] << " ";
    }
    cout << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
}

/**
 * Function : PrintPlayer
 * Description : print Player* object
 * Input : Player*
 * Return : None
 * Other : None
**/
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
    Catch::Session().run( argc, argv );

    PokemonFactory *pokemonFactory = new PokemonFactory();
    cout << "Please name your pokemon: ";
    string name;
    cin >> name;
    Pokemon *charamander = pokemonFactory->CreatePokemon(CHARMELEON, 6, name, "chuck");
    cout << "Please name your pokemon: ";
    cin >> name;
    Pokemon *pikachu = pokemonFactory->CreatePokemon(RAICHU, 15, name, "Taylor");
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
            "RainING", "RainING Pokemon",0, 1, 0, 7, 900, 120,
            38, 678, 467, 34, 25, 2, 1, 2, 1 };
        Pokemon *RPokemon = pokemonFactory->CreatePokemon(pokemonInfo);
        PrintPokeData(RPokemon);

        Poor_ORM::ORMapper<PokemonInfo> pokePackMapper ("pokePackage.db");
        Poor_ORM::ORMapper<PokemonInfo> pokeStgMapper("pokeStorage.db");
        pokePackMapper.CreateTable();
        pokeStgMapper.CreateTable();
        //pokePackMapper.Insert(pokemonInfo);

        PokemonInfo pHelper;
        auto pokemonQuery = pokePackMapper.Query(pHelper)
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
            CAPACITY,
            0,
            0,
            0,
            0,
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
                    " " << c.pokemonNumber << " " << c.packageCapacity <<
                    " "<< c.rank << " " << c.beginDateTime <<
                    " " << c.gameTime << endl << endl;

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

    //initialize cSock array
    memset(cSock, INVALID_SOCKET, sizeof(cSock));
    for (auto& o : onlinePlayer) o = nullPlayer;
    SocketServer *socketServer = new SocketServer();
    //prepare for socket connect
    socketServer->Prepare();

    //waiting for client to connect
    while (true)
    {
        if (socketServer->existingClientCount < MAXSIZE_POOL)
            std::cout << "Waiting Client to connect" << std::endl;
        SOCKET tmpSock = accept(socketServer->ListenSocket, NULL, NULL);
        //if socket pool is not full, permit client socket to connect
        if (socketServer->existingClientCount < MAXSIZE_POOL)
        {
            Helper *helper = new Helper();
            int j = send(tmpSock, Permision.c_str(), Permision.length(), NULL);
            //find a appropriate cSock item to store client socket
            for (int i = 0; i < MAXSIZE_POOL; i++)
            {
                if (cSock[i] == INVALID_SOCKET)
                {
                    cSock[i] = std::move(tmpSock);
                    //a new thread handle connection with client
                    threads[i] =
                        std::thread(ProcessClientRequests, i, &cSock[i], socketServer, helper);
                    threads[i].detach(); //A thread returns and release resources BY ITSELF
                    socketServer->existingClientCount++;
                    break;
                }
            }
        }
        //if socke pool is full, deny client's request
        else
        {
            send(tmpSock, Deny.c_str(), Deny.length(), NULL);
        }
    }
    //after progress shutdown socket and clean up resource
    socketServer->ShutDown();
    delete socketServer;

    return a.exec();
}
