#ifndef SESSION_H
#define SESSION_H

#include "lib/helper.h"
#include "socketServer.h"
#include <thread>
#include "pokemon.h"
#include "pokemonfactory.h"
#include "player.h"
#include "lib/json.hpp"
#include <iostream>
#include <sstream>
#include <QDateTime>
#include <QString>

//get attack way in random
int AttGenFunc(int MIN, int MAX) {
    return MIN + Random(MAX - MIN);
}

using json = nlohmann::json;

using namespace Poor_ORM;

//sort standard
bool SortByRank(const struct PlayerInfo& p1, const struct PlayerInfo& p2)
{
    return p1.rank < p2.rank;
}

/**
 * Function : GetSignInSendStr
 * Description : get sign in message sent to client
 * Input : process id, recved json, json sent to client
 * Return : None
 * Other : None
**/
void GetSignInSendStr(int pid, json recvJ, json& sendJ);

/**
 * Function : GetSignOnSendStr
 * Description : get sign on message sent to client
 * Input : process id, recved json, json sent to client
 * Return : None
 * Other : None
**/
void GetSignOnSendStr(int pid, json recvJ, json& sendJ);

/**
 * Function : GetAcquaintanceSendStr
 * Description : get acquaintance message sent to client
 * Input : process id, recved json, json sent to client
 * Return : None
 * Other : None
**/
void GetAcquaintanceSendStr(json recvJ, json& sendJ);

/**
 * Function : GetOnlinePlayerSendStr
 * Description : get online player message sent to client
 * Input : process id, recved json, json sent to client
 * Return : None
 * Other : None
**/
void GetOnlinePlayerSendStr(json recvJ, json& sendJ);

/**
 * Function : GetMyInfoSendStr
 * Description : get player's information message sent to client
 * Input : process id, recved json, json sent to client
 * Return : None
 * Other : None
**/
void GetMyinfoSendStr(json recvJ, json& sendJ);

/**
 * Function : GetHuntSendStr
 * Description : get hunt message sent to client
 * Input : process id, recved json, json sent to client
 * Return : None
 * Other : None
**/
void GetHuntSendStr(int pid, json recvJ, json& sendJ);

/**
 * Function : GetRankSendStr
 * Description : get rank message sent to client
 * Input : recved json, json sent to client
 * Return : None
 * Other : None
**/
void GetRankSendStr(json recvJ, json& sendJ);

/**
 * Function : GetMyPokeSendStr
 * Description : get player's pokemon message sent to client
 * Input : package or storage, recved json, json sent to client
 * Return : None
 * Other : None
**/
void GetMyPokeSendStr(std::string symbol, json recvJ, json& sendJ);

/**
 * Function : GetExchangeSendStr
 * Description : get sign in message sent to client
 * Input : package or storage, recved json, json sent to client
 * Return : None
 * Other : None
**/
void GetExchangeSendStr(std::string symbol, json recvJ, json& sendJ);

/**
 * Function : GetPlayerPokeSendStr
 * Description : get player's pokemon message sent to client
 * Input : recved json, json sent to client
 * Return : None
 * Other : None
**/
void GetPlayerPokeSendStr(json recvJ, json& sendJ);

/**
 * Function : GetThumbSendStr
 * Description : get thumb message sent to client
 * Input : recved json, json sent to client
 * Return : None
 * Other : None
**/
void GetThumbSendStr(json recvJ, json& sendJ);

/**
 * Function : GetBattleSendStr
 * Description : get battle message sent to client
 * Input : recved json, json sent to client
 * Return : None
 * Other : None
**/
void GetBattleSendStr(json recvJ, json& sendJ);

/**
 * Function : GetSendSendStr
 * Description : get message sent to client
 * Input : process id, helper
 * Return : None
 * Other : None
**/
std::string GetSendStr(int pid, Helper* helper)
{
    json recvJ = json::parse(helper->getRecvStrHelper());
    helper->setRecvStrHelper("");
    json sendJ;
    std::string symbol = recvJ["symbol"];
    //call different progress according to symbol
    if (symbol == "signin")
    {
        GetSignInSendStr(pid, recvJ, sendJ);
    }
    if (symbol == "signon")
    {
        GetSignOnSendStr(pid, recvJ, sendJ);
    }
    if (symbol == "acquaintance")
    {
        GetAcquaintanceSendStr(recvJ, sendJ);
    }
    if (symbol == "onlinePlayer")
    {
        GetOnlinePlayerSendStr(recvJ, sendJ);
    }
    if (symbol == "myinfo")
    {
        GetMyinfoSendStr(recvJ, sendJ);
    }
    if (symbol == "hunt")
    {
        GetHuntSendStr(pid, recvJ, sendJ);
    }
    if (symbol == "rank")
    {
        GetRankSendStr(recvJ, sendJ);
    }
    if (symbol == "package" || symbol == "storage")
    {
        GetMyPokeSendStr(symbol, recvJ, sendJ);
    }
    if (symbol == "packout" || symbol == "stoout")
    {
        GetExchangeSendStr(symbol, recvJ, sendJ);
    }
    if (symbol == "playerPoke")
    {
        GetPlayerPokeSendStr(recvJ, sendJ);
    }
    if (symbol == "thumb")
    {
        GetThumbSendStr(recvJ, sendJ);
    }
    if (symbol == "battle")
    {
        GetBattleSendStr(recvJ, sendJ);
    }
    helper->setSendStrHelper(sendJ.dump());
    std::string strSend = helper->getSendStrHelper();
    helper->setSendStrHelper("");
    return strSend;
}

void GetSignInSendStr(int pid, json recvJ, json& sendJ)
{
    sendJ["symbol"] = "signin";
    //user is not available in default
    sendJ["useravailable"] = false;
    //password is not correct in default
    sendJ["passwordcorrect"] = false;
    //end flag
    sendJ["end"] = "end";
    //user name recved from client
    std::string nameRecv = recvJ["username"];
    //password recved from client
    std::string pwRecv = recvJ["password"];
    //user is not online in default
    sendJ["onlined"] = false;
    bool onlined = false;
    //check through online players
    for (int i = 0; i < MAXSIZE_POOL; i++)
    {
        if (onlinePlayer[i] == nameRecv)
        {
            sendJ["onlined"] = true;
            onlined = true;
            break;
        }
    }
    //progress continues if player is not online
    if (!onlined)
    {
        //get all players' information from database
        Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
        PlayerInfo qHelper;
        auto query = playerMapper.Query(qHelper)
                .ToVector();
        //find player whose name matches
        for (auto q : query)
        {
            if (q.name == nameRecv)
            {
                sendJ["useravailable"] = true;
                //compare password
                if (q.password == pwRecv)
                {
                    sendJ["passwordcorrect"] = true;
                    //player is now online, if password is correct
                    onlinePlayer[pid] = q.name;
                }
                //user exists but password wrong
                else
                {
                    sendJ["passwordcorrect"] = false;
                }
                break;
            }
        }
    }
}

void GetSignOnSendStr(int pid, json recvJ, json& sendJ)
{
    sendJ["symbol"] = "signon";
    //sign on excution is failed in default
    sendJ["signonsuccess"] = false;
    //user is not available in default
    sendJ["useravailable"] = false;
    //end flag
    sendJ["end"] = "end";
    //name recved from client
    std::string nameRecv = recvJ["username"];
    //password recved from client
    std::string pwRecv = recvJ["password"];
    //get players' information from database
    Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
    PlayerInfo qHelper;
    auto query = playerMapper.Query(qHelper)
            .ToVector();
    //user is not logged in default
    bool userNotLogged = true;
    for (auto q : query)
    {
        //user is logged if its name matches
        if (q.name == nameRecv)
        {
            userNotLogged = false;
            break;
        }
    }
    if (userNotLogged)
    {
        sendJ["useravailable"] = true;
        //get current date timea as player's begin time
        QDateTime qdt = QDateTime::currentDateTime();
        std::string s = qdt.toString("yyyyMMddhhmm").toStdString();
        //create a struct playerinfo and insert into database
        struct PlayerInfo p = {nameRecv, pwRecv, 0, CAPACITY, 0, 0, 0, 0, s, "000"};
        sendJ["signonsuccess"] = playerMapper.Insert(p);
        //add player's name into online player array
        onlinePlayer[pid] = p.name;
    }
}

void GetAcquaintanceSendStr(json recvJ, json& sendJ)
{
    sendJ["symbol"] = "acquaintance";
    sendJ["end"] = "end";
    //owner-player's name-from client
    std::string owner = recvJ["owner"];
    //pokemon chosen by player
    std::string pokemon = recvJ["pokemon"];
    //pokemon distributed is on level 1 in default
    int level = 1;
    Kind kind;
    for (int i = 0; i < (sizeof(kindOfString)/sizeof(kindOfString[0])); i++)
    {
        if (pokemon == kindOfString[i])
        {
            kind = (Kind)i;
        }
    }
    std::string name = owner + "1stPokemon";
    //create a Pokemon object
    PokemonFactory *pokemonFactory = new PokemonFactory();
    Pokemon* caughtPokemon = pokemonFactory->CreatePokemon(kind, level, name, owner);

    //get pokemon info of Pokemon object just created
    struct PokemonInfo pokemoninfo = caughtPokemon->ToPokeStruInfo();
    //insert pokemon info into pokemon database
    Poor_ORM::ORMapper<PokemonInfo> pokePackMapper ("pokePackage.db");
    pokePackMapper.Insert(pokemoninfo);
    //get players' information form database
    struct PlayerInfo playerinfo;
    Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
    PlayerInfo qHelper;
    auto query = playerMapper.Query(qHelper)
            .ToVector();    
    //find player info whose name matches
    for (auto q : query)
    {
        if (q.name == owner)
        {
            playerinfo = q;
            break;
        }
    }
    //player's pokemon number increase while package capacity decrease
    playerinfo.pokemonNumber += 1;
    playerinfo.packageCapacity -= 1;
    //update player's information
    playerMapper.Update(playerinfo);
}

void GetOnlinePlayerSendStr(json recvJ, json& sendJ)
{
    sendJ["symbol"] = "onlinePlayer";
    //online player is 0 before looking up
    sendJ["amount"] = 0;
    sendJ["end"] = "end";
    //get player's name who wanna fetch online player information
    std::string player = recvJ["name"];
    int rank;
    double rate;
    int amount = 0;

    //get players' information from database
    Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
    PlayerInfo qHelper;
    auto query = playerMapper.Query(qHelper)
            .ToVector();

    //for each online player who is not the applicant fetching online player info
    for (int i = 0; onlinePlayer[i] != nullPlayer; i++)
    {
        if (onlinePlayer[i] != player)
        {
            //get each online player's detailed information from database
            for (int j = 0; j < query.size(); j++)
            {
                if (query[j].name == onlinePlayer[i])
                {
                    rank = query[j].rank;
                    rate = query[j].rate;
                    break;
                }
            }

            //get send message for each online player
            std::string tmpKeyStr = "";
            std::stringstream stream;
            std::string indexStr;
            stream << amount;
            stream >> indexStr;
            tmpKeyStr = "name" + indexStr;
            //get online player's name
            sendJ[tmpKeyStr] = onlinePlayer[i];
            tmpKeyStr = "rate" + indexStr;
            //get online player's rate
            sendJ[tmpKeyStr] = rate;
            tmpKeyStr = "rank" + indexStr;
            //get online player's rank
            sendJ[tmpKeyStr] = rank;
            //number of online player increase
            amount++;
        }
    }
    //the number of online player sent to client
    sendJ["amount"] = amount;
}

void GetMyinfoSendStr(json recvJ, json& sendJ)
{
    sendJ["symbol"] = "myinfo";
    sendJ["end"] = "end";
    //user name recved from client
    std::string name = recvJ["name"];
    //players' information from database
    Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
    PlayerInfo qHelper;
    struct PlayerInfo playerinfo;
    auto query = playerMapper.Query(qHelper)
            .ToVector();
    //get player's information whose name matches
    for (auto q : query)
    {
        if (q.name == name)
        {
            playerinfo = q;
            break;
        }
    }
    //create a Player object according to struct player info
    //get player's information sent to client
    Player* player = new Player(playerinfo);
    player->setGameTime(player->getBeginDT());
    playerinfo = player->ToPlayerInfo();
    sendJ["name"] = playerinfo.name;
    sendJ["pokemonNumber"] = playerinfo.pokemonNumber;
    sendJ["rate"] = playerinfo.rate;
    sendJ["rank"] = playerinfo.rank;
    sendJ["thumb"] = playerinfo.thumb;
    sendJ["begintime"] = playerinfo.beginDateTime;
    sendJ["gametime"] = playerinfo.gameTime;
}

void GetHuntSendStr(int pid, json recvJ, json& sendJ)
{
    sendJ["symbol"] = "hunt";
    sendJ["end"] = "end";
    //kind of pokemon caught
    std::string kindStr = recvJ["kind"];
    //name of pokemon caught
    std::string name = recvJ["name"];
    //owner of pokemon caught
    std::string owner = recvJ["owner"];
    //pokemon caught is on level 1 in default
    int level = 1;
    Kind kind;
    for (int i = 0; i < (sizeof(kindOfString)/sizeof(kindOfString[0])); i++)
    {
        if (kindStr == kindOfString[i])
        {
            kind = (Kind)i;
        }
    }
    //create Pokemon object with kind, level, name, owner
    PokemonFactory *pokemonFactory = new PokemonFactory();
    Pokemon* caughtPokemon = pokemonFactory->CreatePokemon(kind, level, name, owner);
    struct PokemonInfo pokemoninfo = caughtPokemon->ToPokeStruInfo();
    //get players' information
    std::string playerName = onlinePlayer[pid];
    struct PlayerInfo playerinfo;
    Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
    PlayerInfo qHelper;
    auto query = playerMapper.Query(qHelper)
            .ToVector();
    //find applicant who caugh pokemon
    for (auto q : query)
    {
        if (q.name == playerName)
        {
            playerinfo = q;
            break;
        }
    }

    playerinfo.pokemonNumber += 1;
    //if player's package not full, add pokemon into package
    if (playerinfo.packageCapacity > 0)
    {
        playerinfo.packageCapacity -= 1;
        Poor_ORM::ORMapper<PokemonInfo> pokePackMapper ("pokePackage.db");
        pokePackMapper.Insert(pokemoninfo);
    }
    //if package is full, add pokemon into storage
    else
    {
        Poor_ORM::ORMapper<PokemonInfo> pokeStgMapper("pokeStorage.db");
        pokeStgMapper.Insert(pokemoninfo);
    }
    //update player's information
    playerMapper.Update(playerinfo);
}

void GetRankSendStr(json recvJ, json& sendJ)
{
    sendJ["symbol"] = "rank";
    sendJ["end"] = "end";
    std::string player = recvJ["name"];
    //get players' information from database
    Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
    struct PlayerInfo qHelper;
    auto query = playerMapper.Query(qHelper)
            .ToVector();
    //sort players by rank score
    std::sort(query.begin(), query.end(), SortByRank);
    std::stringstream stream;
    std::string numStr;
    std::string nameKey;
    std::string rankKey;
    std::string rateKey;
    int amount = 0;
    //get players' information sent to client except applicant him/her self
    for (int i = 0; i < query.size() && i < 10; i++)
    {
        if (player != query[i].name)
        {
            stream.clear();
            stream << amount;
            stream >> numStr;
            nameKey = "name" + numStr;
            rankKey = "rank" + numStr;
            rateKey = "rate" + numStr;
            sendJ[nameKey] = query[i].name;
            sendJ[rankKey] = query[i].rank;
            sendJ[rateKey] = query[i].rate;
            amount++;
        }
    }
    //the number of rank players
    sendJ["amount"] = amount;
}

void GetMyPokeSendStr(std::string symbol, json recvJ, json& sendJ)
{
    sendJ["end"] = "end";
    PokemonInfo qHelper;
    std::vector<PokemonInfo> query;
    std::string owner = recvJ["name"];
    //look up package database if applicant wanna check his/her package
    if (symbol == "package")
    {
        sendJ["symbol"] = "package";
        Poor_ORM::ORMapper<PokemonInfo> pokePackMapper ("pokePackage.db");
        query = pokePackMapper.Query(qHelper)
                        .ToVector();
    }
    //look up storage database if applicant wanna check hisher storage
    if (symbol == "storage")
    {
        sendJ["symbol"] = "storage";
        Poor_ORM::ORMapper<PokemonInfo> pokeStoMapper ("pokeStorage.db");
        query = pokeStoMapper.Query(qHelper)
                        .ToVector();
    }
    int amount = 0;
    std::stringstream stream;
    std::string indexStr;
    std::string keyStr;
    //look through each pokemon in pokemons' information
    for (int i = 0; i < query.size(); i++)
    {
        //get pokemon's information if its owner is the applicant
        if (query[i].owner == owner)
        {
            stream.clear();
            stream << amount;
            stream >> indexStr;
            keyStr = "kind" + indexStr;
            sendJ[keyStr] = kindOfString[query[i].kind];
            keyStr = "name" + indexStr;
            sendJ[keyStr] = query[i].name;
            keyStr = "character" + indexStr;
            sendJ[keyStr] = characterOfString[query[i].character];
            keyStr = "level" + indexStr;
            sendJ[keyStr] = query[i].level;
            keyStr = "exp" + indexStr;
            sendJ[keyStr] = query[i].experiencePoint;
            keyStr = "att" + indexStr;
            sendJ[keyStr] = query[i].attackPoint;
            keyStr = "def" + indexStr;
            sendJ[keyStr] = query[i].defencePoint;
            keyStr = "HP" + indexStr;
            sendJ[keyStr] = query[i].totalHP;
            keyStr = "speed" + indexStr;
            sendJ[keyStr] = query[i].intervalIncrease;
            keyStr = "critical" + indexStr;
            sendJ[keyStr] = query[i].criticalPoint;
            amount++;
        }
    }
    sendJ["amount"] = amount;
}

void GetExchangeSendStr(std::string symbol, json recvJ, json& sendJ)
{
    PokemonInfo qOutHelper;
    PokemonInfo qInHelper;
    std::vector<PokemonInfo> queryOut;
    std::vector<PokemonInfo> queryIn;
    PokemonInfo pokemonIn;
    std::string name = recvJ["name"];
    std::string owner = recvJ["owner"];
    Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
    PlayerInfo pHelper;
    PlayerInfo player;
    auto pquery = playerMapper.Query(pHelper)
            .ToVector();
    for (auto p : pquery)
    {
        if (p.name == owner)
        {
            player = p;
            break;
        }
    }
    //take pokemon out from package to storage
    if (symbol == "packout")
    {
        sendJ["symbol"] = "packout";
        //get pokemons' information from package database
        Poor_ORM::ORMapper<PokemonInfo> pokePackMapper ("pokePackage.db");
        queryOut = pokePackMapper.Query(qOutHelper)
                    .ToVector();
        Poor_ORM::ORMapper<PokemonInfo> pokeStoMapper ("pokeStorage.db");
        //find pokemon whose name matches
        for (auto& q : queryOut)
        {
            if (q.name == name)
            {
                pokemonIn = q;
                break;
            }
        }
        //delete pokemon from package database
        pokePackMapper.Delete(pokemonIn);
        //inser pokemon into storage database
        pokeStoMapper.Insert(pokemonIn);
        //player's package capacity increase
        player.packageCapacity += 1;
        playerMapper.Update(player);
    }
    if (symbol == "stoout")
    {
        sendJ["symbol"] = "stoout";
        Poor_ORM::ORMapper<PokemonInfo> pokePackMapper ("pokePackage.db");
        queryIn = pokePackMapper.Query(qInHelper)
                    .ToVector();
        //allow take pokemon from storage to package if package is not full
        if (player.packageCapacity > 0)
        {
            //get plokemons' information from storage database
            Poor_ORM::ORMapper<PokemonInfo> pokeStoMapper ("pokeStorage.db");
            queryOut = pokeStoMapper.Query(qOutHelper)
                        .ToVector();
            //find pokemon whose name matches
            for (auto& q : queryOut)
            {
                if (q.name == name)
                {
                    pokemonIn = q;
                    break;
                }
            }
            //insert pokemon into package database
            pokePackMapper.Insert(pokemonIn);
            //delete pokemon from storage database
            pokeStoMapper.Delete(pokemonIn);
            //player's package capacity decrease
            player.packageCapacity -= 1;
            playerMapper.Update(player);
            sendJ["success"] = true;
        }
        //exchange excution failed if package is full
        else
            sendJ["success"] = false;
    }
    sendJ["end"] = "end";
}

void GetPlayerPokeSendStr(json recvJ, json& sendJ)
{
    std::string name = recvJ["name"];
    sendJ["symbol"] = "playerPoke";
    sendJ["end"] = "end";
    //get pokemons' information from package databse
    Poor_ORM::ORMapper<PokemonInfo> pokePackMapper ("pokePackage.db");
    PokemonInfo qHelper;
    auto query = pokePackMapper.Query(qHelper)
            .ToVector();
    std::stringstream stream;
    std::string indexStr;
    std::string keyStr;
    int index = 0;
    //find pokemon whose owner matches
    for (int i = 0; i < query.size();i++)
    {
        if (query[i].owner == name)
        {
            stream.clear();
            stream << index;
            stream >> indexStr;
            keyStr = "name" + indexStr;
            sendJ[keyStr] = query[i].name;
            keyStr = "kind" + indexStr;
            sendJ[keyStr] = kindOfString[query[i].kind];
            keyStr = "level" + indexStr;
            sendJ[keyStr] = query[i].level;
            index++;
        }
    }
    query.clear();
    //get pokemon information from storage database
    Poor_ORM::ORMapper<PokemonInfo> pokeStoreMapper ("pokeStorage.db");
    query = pokeStoreMapper.Query(qHelper)
            .ToVector();
    //find pokemon whose name matches
    for (int i = 0; i < query.size();i++)
    {
        if (query[i].owner == name)
        {
            stream.clear();
            stream << index;
            stream >> indexStr;
            keyStr = "name" + indexStr;
            sendJ[keyStr] = query[i].name;
            keyStr = "kind" + indexStr;
            sendJ[keyStr] = kindOfString[query[i].kind];
            keyStr = "level" + indexStr;
            sendJ[keyStr] = query[i].level;
            index++;
        }
    }
    sendJ["amount"] = index;
}

void GetThumbSendStr(json recvJ, json& sendJ)
{
    std::string name = recvJ["name"];
    //get players' information from player info database
    Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
    PlayerInfo qHelper;
    auto query = playerMapper.Query(qHelper)
            .ToVector();    
    //find player whose name matches
    for (auto q : query)
    {
        if (q.name == name)
        {
            //player's thumb number increase and update
            q.thumb += 1;
            playerMapper.Update(q);
            break;
        }
    }
    sendJ["symbol"] = "thumb";
    sendJ["end"] = "end";
}

/**
 * Function : GetMyPokeCrew
 * Description : get player's pokemons which going to battle
 * Input : player's name-pokemons' owner
 * Return : std::vector<Pokemon*>
 * Other : None
**/
std::vector<Pokemon*> GetMyPokeCrew(std::string owner);

/**
 * Function : GetEnemyPokeCrew
 * Description : get boot pokemons which going to battle
 * Input : number of pokemons, nature of pokemons, battle standard
 * Return : std::vector<Pokemon*>
 * Other : None
**/
std::vector<Pokemon*> GetEnemyPokeCrew(int pokeNum, std::string nature, std::string standard);

/**
 * Function : GetCrewInfo
 * Description : get player's and boot pokemon crews detailed information
 * Input : number of pokemons, player's pokemon crew, boot pokemon crew, json to be sent
 * Return : None
 * Other : None
**/
void GetCrewInfo(int pokeNum, std::vector<Pokemon*> myPokemon, std::vector<Pokemon*> enemyPokemon, json& sendJ);

/**
 * Function : ResolveBarConflict
 * Description : resolve the conflict when both sides reaches initiative bar
 * Input : bar of player's pokemon, bar of boot pokemon, initiative bar
 * Return : None
 * Other : None
**/
void ResolveBarConflict(int& myBar, int& enemyBar, int InitiativeBar);

/**
 * Function : Defeat
 * Description : get defeat message sent to client
 * Input : json to be sent, owner, player's pokemon crew, boot crew
 * Return : None
 * Other : None
**/
void Defeat(json& sendJ, std::string owner, std::vector<Pokemon*> myPokemon, std::vector<Pokemon*> enemyPokemon);

/**
 * Function : Win
 * Description : get victory message sent to client
 * Input : json to be sent, owner, player's pokemon crew, boot crew
 * Return : None
 * Other : None
**/
void Win(json& sendJ, std::string owner, std::vector<Pokemon*> myPokemon, std::vector<Pokemon*> enemyPokemon);

void GetBattleSendStr(json recvJ, json& sendJ)
{
    //get pokemons' information from package database
    Poor_ORM::ORMapper<PokemonInfo> pokePackMapper ("pokePackage.db");
    std::string owner = recvJ["name"];
    //get pokemon crew whose owner is the appplicant
    std::vector<Pokemon*> myPokemon = GetMyPokeCrew(owner);
    int pokeNum = myPokemon.size();
    //package is empty to battle
    if (pokeNum == 0)
    {
        sendJ["symbol"] = "nobattle";
        sendJ["end"] = "end";
    }
    //package is not empty
    else
    {
        //get boot pokemon crew
        std::string nature = recvJ["nature"];
        std::string standard = recvJ["standard"];
        std::vector<Pokemon*> enemyPokemon = GetEnemyPokeCrew(pokeNum, nature, standard);
        sendJ["symbol"] = "battle";
        sendJ["end"] = "end";
        sendJ["amount"] = pokeNum;
        //get crews' information sent to client on both sides
        GetCrewInfo(pokeNum, myPokemon, enemyPokemon, sendJ);
        //player's pokemon and boot pokemon start at begin of bar
        int myBar = 0;
        int enemyBar = 0;
        //player's fighting pokemon
        Pokemon* myFightingPokemon;
        //boot fighting pokemon
        Pokemon* enemyFightingPokemon;
        //player's pokemons are not all dead
        bool myPokeAllDead = false;
        //boot pokemons are not all dead
        bool enemyPokeAllDead = false;
        //index of player's pokemon which is fighting
        int myPokeIndex = 0;
        //index of boot pokemon which is fighting
        int enemyPokeIndex = 0;
        //player's fighting pokemon is 1st one
        myFightingPokemon = myPokemon[0];
        //boot fighting pokemon is 1st one
        enemyFightingPokemon = enemyPokemon[0];
        //round count
        int roundCnt = 0;
        std::stringstream stream;
        std::string roundStr;
        std::string keyStr;
        //battle terminates until pokemon of one side are all dead
        while (!(myPokeAllDead || enemyPokeAllDead))
        {
            stream.clear();
            stream << roundCnt;
            stream >> roundStr;
            //pokemons' bar increase until one or more reach initiative bar
            while ((myBar < InitiativeBar) &&
                   (enemyBar < InitiativeBar))
            {
                myBar += myFightingPokemon->getIntervalIncrease();
                enemyBar += enemyFightingPokemon->getIntervalIncrease();
            }
            //if both sides reach initiative bar, resolve the conflict
            if (myBar >= InitiativeBar && enemyBar >= InitiativeBar)
            {
                ResolveBarConflict(myBar, enemyBar, InitiativeBar);
            }
            //player's pokemon turn
            if (myBar >= InitiativeBar)
            {
                myBar -= InitiativeBar;
                keyStr = "round" + roundStr + "attack";
                sendJ[keyStr] = "mypokemon";
                //pokemon get hurt before attacking if in unhealthy state
                int myBeforeHP = myFightingPokemon->getCurrentHP();
                myFightingPokemon->Hurt();
                int myAfterHP = myFightingPokemon->getCurrentHP();
                if (myAfterHP < 0)
                    myAfterHP = 0;
                //pokemon is not hurt
                if (myAfterHP == myBeforeHP)
                {
                    keyStr = "round" + roundStr + "myhurt";
                    sendJ[keyStr] = false;
                }
                //pokemon is hurt by sick state
                else
                {
                    keyStr = "round" + roundStr + "myhurt";
                    sendJ[keyStr] = true;
                    keyStr = "round" + roundStr + "hurtnature";
                    std::string hurtNature = stateOfString[(int)myFightingPokemon->getState()];
                    sendJ[keyStr] = hurtNature;
                    //currentHP after get hurt
                    keyStr = "round" + roundStr + "myhurthp";
                    sendJ[keyStr] = myAfterHP;
                }
                //judge whether pokemon is still alive after get hurt
                myFightingPokemon->DeadJudge();
                //pokemon attacks if it's alive
                if (myFightingPokemon->getAlive())
                {
                    keyStr = "round" + roundStr + "attnature";
                    sendJ[keyStr] = natureOfString[(int)myFightingPokemon->getNature()];

                    //normal attack
                    if ((AttGenFunc(0, 99) % 2) == 0)
                    {
                        myFightingPokemon->Attack(enemyFightingPokemon);
                        keyStr = "round" + roundStr + "attway";
                        sendJ[keyStr] = "att";
                    }
                    //special attack
                    else
                    {
                        myFightingPokemon->SpecialAttack(enemyFightingPokemon);
                        keyStr = "round" + roundStr + "attway";
                        sendJ[keyStr] = "specialatt";
                    }
                    //enemy pokemon current HP after attacked
                    keyStr = "round" + roundStr + "enemypokemonhp";
                    enemyFightingPokemon->DeadJudge();
                    sendJ[keyStr] = enemyFightingPokemon->getCurrentHP();
                    //boot pokemon is dead
                    if (!(enemyFightingPokemon->getAlive()))
                    {
                        //next boot pokemon fights
                        if ((enemyPokeIndex + 1) < enemyPokemon.size())
                        {
                            enemyPokeIndex++;
                            enemyFightingPokemon = enemyPokemon[enemyPokeIndex];
                        }
                        //boot pokemon are all dead, win
                        else
                        {
                            enemyPokeAllDead = true;
                        }
                    }
                    //else enemy pokemon survived
                }
                //pokemon is dead
                else
                {
                    //next pokemon fights
                    if ((myPokeIndex + 1) < myPokemon.size())
                    {
                        myPokeIndex++;
                        myFightingPokemon = myPokemon[myPokeIndex];
                    }
                    //all of player's pokemon dead, lose
                    else
                    {
                        myPokeAllDead = true;
                    }
                }
            }

            //boot pokemon turn
            if (enemyBar >= InitiativeBar)
            {
                enemyBar -= InitiativeBar;
                keyStr = "round" + roundStr + "attack";
                sendJ[keyStr] = "enemypokemon";
                //pokemon get hurt before attacking
                int enemyBeforeHP = enemyFightingPokemon->getCurrentHP();
                enemyFightingPokemon->Hurt();
                int enemyAfterHP = enemyFightingPokemon->getCurrentHP();
                if (enemyAfterHP < 0)
                    enemyAfterHP = 0;
                //boot pokemon is not hurt
                if (enemyAfterHP == enemyBeforeHP)
                {
                    keyStr = "round" + roundStr + "enemyhurt";
                    sendJ[keyStr] = false;
                }
                //boot pokemon is hurt by sick state
                else
                {
                    keyStr = "round" + roundStr + "enemyhurt";
                    sendJ[keyStr] = true;
                    keyStr = "round" + roundStr + "hurtnature";
                    std::string hurtNature = natureOfString[enemyFightingPokemon->getState()];
                    sendJ[keyStr] = hurtNature;
                    //currentHP after attacked
                    keyStr = "round" + roundStr + "enemyhurthp";
                    sendJ[keyStr] = enemyAfterHP;
                }
                //judget whether boot pokemon is still alive after get hurt
                enemyFightingPokemon->DeadJudge();
                //boot pokemon is still alive, it attacks
                if (enemyFightingPokemon->getAlive())
                {
                    keyStr = "round" + roundStr + "attnature";
                    sendJ[keyStr] = natureOfString[(int)enemyFightingPokemon->getNature()];

                    //normal attack
                    if ((AttGenFunc(0, 99) % 2) == 0)
                    {
                        enemyFightingPokemon->Attack(myFightingPokemon);
                        keyStr = "round" + roundStr + "attway";
                        sendJ[keyStr] = "att";
                    }
                    //special attack
                    else
                    {
                        enemyFightingPokemon->SpecialAttack(myFightingPokemon);
                        keyStr = "round" + roundStr + "attway";
                        sendJ[keyStr] = "specialatt";
                    }
                    //my pokemon current hp after attacked
                    keyStr = "round" + roundStr + "mypokemonhp";
                    myFightingPokemon->DeadJudge();
                    sendJ[keyStr] = myFightingPokemon->getCurrentHP();
                    //player's pokemon is dead
                    if (!(myFightingPokemon->getAlive()))
                    {
                        //next player's pokemon fights
                        if ((myPokeIndex + 1) < myPokemon.size())
                        {
                            myPokeIndex++;
                            myFightingPokemon = myPokemon[myPokeIndex];
                        }
                        //all of player's pokemon dead, lose
                        else
                        {
                            myPokeAllDead = true;
                        }
                    }
                }
                //boot pokemon is dead
                else
                {
                    //next boot pokemon fights
                    if ((enemyPokeIndex + 1) < enemyPokemon.size())
                    {
                        enemyPokeIndex++;
                        enemyFightingPokemon = enemyPokemon[enemyPokeIndex];
                    }
                    //all of boot pokemon are dead, win
                    else
                    {
                        enemyPokeAllDead = true;
                    }
                }
            }
            roundCnt++;
        }
        sendJ["round"] = roundCnt;        
        if (myPokeAllDead)
        {
            Defeat(sendJ, owner, myPokemon, enemyPokemon);
        }
        else
        {
            Win(sendJ, owner, myPokemon, enemyPokemon);
        }
    }
}

std::vector<Pokemon*> GetMyPokeCrew(std::string owner)
{
    //get pokemons' information from package database
    std::vector<Pokemon*> myPokemon;
    PokemonFactory* pokemonFactory = new PokemonFactory();
    Poor_ORM::ORMapper<PokemonInfo> pokePackMapper ("pokePackage.db");
    PokemonInfo qHelper;
    auto query = pokePackMapper.Query(qHelper)
                    .ToVector();
    //find pokemons whose names matche
    for (auto& q : query)
    {
        if (q.owner == owner)
        {
            Pokemon* pokemon = pokemonFactory->CreatePokemon(q);
            myPokemon.push_back(pokemon);
        }
    }
    return myPokemon;
}

std::vector<Pokemon*> GetEnemyPokeCrew(int pokeNum, std::string nature, std::string standard)
{
    //create boot crew with attributes player has instructed
    std::vector<Pokemon*> enemyPokemon;
    PokemonFactory* pokemonFactory = new PokemonFactory();
    if (nature == "fire")
    {
        if (standard == "gold")
        {
            for (int i = 0; i < pokeNum; i++)
            {
                Kind kind = (Kind)(2 + i * 3);
                int level = GoldPokeLevel;
                std::string bootname = "boot " + kindOfString[(int)kind];
                Pokemon* ep = pokemonFactory->CreatePokemon(kind, level, bootname, "BOOT");
                enemyPokemon.push_back(ep);
            }
        }
        if (standard == "silver")
        {
            for (int i = 0; i < pokeNum; i++)
            {
                Kind kind = (Kind)(1 + i * 3);
                int level = SilverPokeLevel;
                std::string bootname = "boot " + kindOfString[(int)kind];
                Pokemon* ep = pokemonFactory->CreatePokemon(kind, level, bootname, "BOOT");
                enemyPokemon.push_back(ep);
            }
        }
        if (standard == "bronze")
        {
            for (int i = 0; i < pokeNum; i++)
            {
                Kind kind = (Kind)(0 + i * 3);
                int level = BronzePokeLevel;
                std::string bootname = "boot " + kindOfString[(int)kind];
                Pokemon* ep = pokemonFactory->CreatePokemon(kind, level, bootname, "BOOT");
                enemyPokemon.push_back(ep);
            }
        }
    }
    if (nature == "water")
    {
        if (standard == "gold")
        {
            for (int i = 0; i < pokeNum; i++)
            {
                Kind kind = (Kind)(11 + i * 3);
                int level = GoldPokeLevel;
                std::string bootname = "boot " + kindOfString[(int)kind];
                Pokemon* ep = pokemonFactory->CreatePokemon(kind, level, bootname, "BOOT");
                enemyPokemon.push_back(ep);
            }
        }
        if (standard == "silver")
        {
            for (int i = 0; i < pokeNum; i++)
            {
                Kind kind = (Kind)(10 + i * 3);
                int level = SilverPokeLevel;
                std::string bootname = "boot " + kindOfString[(int)kind];
                Pokemon* ep = pokemonFactory->CreatePokemon(kind, level, bootname, "BOOT");
                enemyPokemon.push_back(ep);
            }
        }
        if (standard == "bronze")
        {
            for (int i = 0; i < pokeNum; i++)
            {
                Kind kind = (Kind)(9 + i * 3);
                int level = BronzePokeLevel;
                std::string bootname = "boot " + kindOfString[(int)kind];
                Pokemon* ep = pokemonFactory->CreatePokemon(kind, level, bootname, "BOOT");
                enemyPokemon.push_back(ep);
            }
        }
    }
    if (nature == "bush")
    {
        if (standard == "gold")
        {
            for (int i = 0; i < pokeNum; i++)
            {
                Kind kind = (Kind)(20 + i * 3);
                int level = GoldPokeLevel;
                std::string bootname = "boot " + kindOfString[(int)kind];
                Pokemon* ep = pokemonFactory->CreatePokemon(kind, level, bootname, "BOOT");
                enemyPokemon.push_back(ep);
            }
        }
        if (standard == "silver")
        {
            for (int i = 0; i < pokeNum; i++)
            {
                Kind kind = (Kind)(19 + i * 3);
                int level = SilverPokeLevel;
                std::string bootname = "boot " + kindOfString[(int)kind];
                Pokemon* ep = pokemonFactory->CreatePokemon(kind, level, bootname, "BOOT");
                enemyPokemon.push_back(ep);
            }
        }
        if (standard == "bronze")
        {
            for (int i = 0; i < pokeNum; i++)
            {
                Kind kind = (Kind)(18 + i * 3);
                int level = BronzePokeLevel;
                std::string bootname = "boot " + kindOfString[(int)kind];
                Pokemon* ep = pokemonFactory->CreatePokemon(kind, level, bootname, "BOOT");
                enemyPokemon.push_back(ep);
            }
        }
    }
    if (nature == "electricity")
    {
        if (standard == "gold")
        {
            for (int i = 0; i < pokeNum; i++)
            {
                Kind kind = (Kind)(29 + i * 3);
                int level = GoldPokeLevel;
                std::string bootname = "boot " + kindOfString[(int)kind];
                Pokemon* ep = pokemonFactory->CreatePokemon(kind, level, bootname, "BOOT");
                enemyPokemon.push_back(ep);
            }
        }
        if (standard == "silver")
        {
            for (int i = 0; i < pokeNum; i++)
            {
                Kind kind = (Kind)(28 + i * 3);
                int level = SilverPokeLevel;
                std::string bootname = "boot " + kindOfString[(int)kind];
                Pokemon* ep = pokemonFactory->CreatePokemon(kind, level, bootname, "BOOT");
                enemyPokemon.push_back(ep);
            }
        }
        if (standard == "bronze")
        {
            for (int i = 0; i < pokeNum; i++)
            {
                Kind kind = (Kind)(27 + i * 3);
                int level = BronzePokeLevel;
                std::string bootname = "boot " + kindOfString[(int)kind];
                Pokemon* ep = pokemonFactory->CreatePokemon(kind, level, bootname, "BOOT");
                enemyPokemon.push_back(ep);
            }
        }
     }
    return enemyPokemon;
}

void GetCrewInfo(int pokeNum, std::vector<Pokemon*> myPokemon, std::vector<Pokemon*> enemyPokemon, json& sendJ)
{
    //get crew information sent to client
    std::stringstream s;
    std::string iStr;
    for (int i = 0; i < pokeNum; i++)
    {
        s.clear();
        s << i;
        s >> iStr;
        std::string key = "mypokename" + iStr;
        sendJ[key] = myPokemon[i]->getName();
        key = "mypokekind" + iStr;
        sendJ[key] = kindOfString[myPokemon[i]->getKind()];
        key = "mypokehp" + iStr;
        sendJ[key] = myPokemon[i]->getTotalHP();
        key = "enemypokename" + iStr;
        sendJ[key] = enemyPokemon[i]->getName();
        key = "enemypokekind" + iStr;
        sendJ[key] = kindOfString[enemyPokemon[i]->getKind()];
        key = "enemypokehp" + iStr;
        sendJ[key] = enemyPokemon[i]->getTotalHP();
    }
}

void ResolveBarConflict(int& myBar, int& enemyBar, int InitiativeBar)
{
    //chose one side to fight, another step back 1 point, in random
    int ranNum = AttGenFunc(0, 99);
    if (ranNum % 2 == 0)
        myBar = InitiativeBar - 1;
    else
        enemyBar = InitiativeBar - 1;
}

void Defeat(json& sendJ, std::string owner, std::vector<Pokemon*> myPokemon, std::vector<Pokemon*> enemyPokemon)
{
    //get pokemon in package database and player information in player database
    Poor_ORM::ORMapper<PokemonInfo> pokePackMapper ("pokePackage.db");
    Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
    PlayerInfo pHelper;
    PlayerInfo player;
    auto pquery = playerMapper.Query(pHelper)
            .ToVector();
    for (auto q : pquery)
    {
        if (q.name == owner)
        {
            player = q;
            break;
        }
    }
    sendJ["win"] = false;
    std::string keyStr;
    int expGot;
    int pokeCnt = 0;
    std::stringstream pokeStream;
    std::string pokeCntStr;
    //pokemons gain experience
    for (Pokemon* mp : myPokemon)
    {
        pokeStream.clear();
        pokeStream << pokeCnt;
        pokeStream >> pokeCntStr;
        expGot = ExpGrade[enemyPokemon[0]->getLevel()] / 10;
        mp->setExperiencePoint(mp->getExperiencePoint() + expGot);
        //pokemons level up and evolute while experience is plentyful
        while ((mp->getExperiencePoint() >= ExpGrade[mp->getLevel() + 1]) && (mp->getLevel() < 15))
            mp->Upgrade();
        mp->setAlive(true);
        mp->setCurrentHP(mp->getTotalHP());
        mp->setSickCounter(0);
        mp->setSickPoint(0);
        mp->setState(HEALTHY);
        keyStr = "myevokind" + pokeCntStr;
        sendJ[keyStr] = kindOfString[(int)mp->getKind()];
        pokeCnt++;
        //get pokemon info struct and update
        PokemonInfo pi = mp->ToPokeStruInfo();
        pokePackMapper.Update(pi);
    }
    sendJ["expGot"] = expGot;
    //update player's rate, rank and games
    double winGames = (double)(player.rate * player.games);
    player.rate = (winGames) / (player.games + 1);
    player.games += 1;
    playerMapper.Update(player);
}


void Win(json& sendJ, std::string owner, std::vector<Pokemon*> myPokemon, std::vector<Pokemon*> enemyPokemon)
{
    //get pokemon and player information in package and player info database
    Poor_ORM::ORMapper<PokemonInfo> pokePackMapper ("pokePackage.db");
    Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
    PlayerInfo pHelper;
    PlayerInfo player;
    auto pquery = playerMapper.Query(pHelper)
            .ToVector();
    for (auto q : pquery)
    {
        if (q.name == owner)
        {
            player = q;
            break;
        }
    }
    sendJ["win"] = true;
    std::string keyStr;
    int expGot;
    int pokeCnt = 0;
    std::stringstream pokeStream;
    std::string pokeCntStr;
    //pokemons gain experience
    for (Pokemon* mp : myPokemon)
    {
        pokeStream.clear();
        pokeStream << pokeCnt;
        pokeStream >> pokeCntStr;
        expGot = ExpGrade[enemyPokemon[0]->getLevel()] / 5;
        mp->setExperiencePoint(mp->getExperiencePoint() + expGot);
        //pokemons level up and evolute while experience is plentyful
        while ((mp->getExperiencePoint() >= ExpGrade[mp->getLevel() + 1]) && (mp->getLevel() < 15))
        {
            mp->Upgrade();
        }
        mp->setAlive(true);
        mp->setCurrentHP(mp->getTotalHP());
        mp->setSickCounter(0);
        mp->setSickPoint(0);
        mp->setState(HEALTHY);
        keyStr = "myevokind" + pokeCntStr;
        sendJ[keyStr] = kindOfString[(int)mp->getKind()];
        pokeCnt++;
        //get pokemon info struct and update
        PokemonInfo pi = mp->ToPokeStruInfo();
        pokePackMapper.Update(pi);
    }
    sendJ["expGot"] = expGot;
    //update player's rate, rank and games
    double winGames = (double)(player.rate * player.games) + 1;
    player.rate = (winGames) / (player.games + 1);
    player.rank += enemyPokemon[0]->getLevel() / myPokemon[0]->getLevel();
    player.games += 1;
    playerMapper.Update(player);
}

//send message to client in a new thread
DWORD WINAPI SendThreadFunc(int pid, LPVOID cParam, LPVOID sParam, LPVOID hParam)
{
    SOCKET* clientSocket = (SOCKET*)cParam;
    Helper* helper = (Helper*)hParam;
    std::string strSend = GetSendStr(pid, helper);
    std::cout << "strSend to " << pid << ":" << strSend << std::endl;
    send(*clientSocket, strSend.c_str(), strSend.length(), NULL);
    return 0;
}

//recv message from client in a new thread
DWORD WINAPI RecvThreadFunc(int pid, LPVOID cParam, LPVOID sParam, LPVOID hParam)
{
    SOCKET* clientSocket = (SOCKET*)cParam;
    SocketServer *socketServer = (SocketServer*)sParam;
    Helper *helper = (Helper*)hParam;
    char buff[DEFAULT_BUFLEN] = { NULL };
    while (true) {
        for (auto& ch : buff) ch = NULL;
        int result = recv(*clientSocket, buff, DEFAULT_BUFLEN, 0);
        if (result > 0)
        {
            std::string strRecv = buff;
            helper->setRecvStrHelper(strRecv);
            std::cout << "Thread " << pid << ": " << strRecv << std::endl;
            std::thread sendThread = std::thread(SendThreadFunc, pid, clientSocket, socketServer, helper);
            sendThread.join();
            for (auto& b : buff) { b = NULL; }
            //break;
        }
        else if (result == -1) //client window closed, release resources and return
        {
            std::cout << "Client "
                      << pid
                      << "'s window closed, connection terminated"
                      << std::endl;
            break;
        }
    }
    return 0;
}

//process which handle affair with one client
DWORD WINAPI ProcessClientRequests(int pid, LPVOID cParam, LPVOID sParam, LPVOID hParam)
{
    SOCKET* clientSocket = (SOCKET*)cParam;
    SocketServer *socketServer = (SocketServer*)sParam;
    Helper *helper = (Helper*)hParam;
    std::cout << "Thread " << pid << std::endl;
    std::thread recvThread;
    recvThread = std::thread(RecvThreadFunc, pid, clientSocket, socketServer, helper);
    recvThread.join();
    closesocket(*clientSocket);

    socketServer->existingClientCount--;
    cSock[pid] = INVALID_SOCKET;
    onlinePlayer[pid] = "";
    delete helper;

    return 0;
}

#endif // SESSION_H
