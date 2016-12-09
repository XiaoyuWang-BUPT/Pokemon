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

using json = nlohmann::json;

using namespace Poor_ORM;

bool SortByRank(const struct PlayerInfo& p1, const struct PlayerInfo& p2)
{
    return p1.rank < p2.rank;
}

std::string GetSendStr(int pid, Helper* helper)
{
    json recvJ = json::parse(helper->getRecvStrHelper());
    helper->setRecvStrHelper("");
    json sendJ;
    std::string symbol = recvJ["symbol"];
    if (symbol == "signin")
    {
        sendJ["symbol"] = "signin";
        sendJ["useravailable"] = false;
        sendJ["passwordcorrect"] = false;
        sendJ["end"] = "end";
        std::string nameRecv = recvJ["username"];
        std::string pwRecv = recvJ["password"];
        Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
        PlayerInfo qHelper;
        auto query = playerMapper.Query(qHelper)
                .ToVector();
        for (auto q : query)
        {
            if (q.name == nameRecv)
            {
                sendJ["useravailable"] = true;
                if (q.password == pwRecv)
                {
                    sendJ["passwordcorrect"] = true;
                    onlinePlayer[pid] = q.name;
                }
                else
                {
                    sendJ["passwordcorrect"] = false;
                }
                break;
            }
        }
    }
    if (symbol == "signon")
    {
        sendJ["symbol"] = "signon";
        sendJ["signonsuccess"] = false;
        sendJ["useravailable"] = false;
        sendJ["end"] = "end";
        std::string nameRecv = recvJ["username"];
        std::string pwRecv = recvJ["password"];
        Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
        PlayerInfo qHelper;
        auto query = playerMapper.Query(qHelper)
                .ToVector();
        bool userNotLogged = true;
        for (auto q : query)
        {
            if (q.name == nameRecv)
            {
                userNotLogged = false;
                break;
            }
        }
        if (userNotLogged)
        {
            sendJ["useravailable"] = true;
            QDateTime qdt = QDateTime::currentDateTime();
            std::string s = qdt.toString("yyyyMMddhhmm").toStdString();
            std::cout << "QDateTime:" << s << std::endl;
            struct PlayerInfo p = {nameRecv, pwRecv, 0, CAPACITY, 0, 0, 0, 0, s, "000"};
            sendJ["signonsuccess"] = playerMapper.Insert(p);
            onlinePlayer[pid] = p.name;
        }
    }
    if (symbol == "onlinePlayer")
    {
        sendJ["symbol"] = "onlinePlayer";
        sendJ["amount"] = 0;
        sendJ["end"] = "end";
        std::string player = recvJ["name"];
        int rank;
        double rate;
        int amount = 0;

        Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
        PlayerInfo qHelper;
        auto query = playerMapper.Query(qHelper)
                .ToVector();

        for (int i = 0; onlinePlayer[i] != nullPlayerPair; i++)
        {
            if (onlinePlayer[i] != player)
            {
                for (int j = 0; j < query.size(); j++)
                {
                    if (query[j].name == onlinePlayer[i])
                    {
                        rank = query[j].rank;
                        rate = query[j].rate;
                        break;
                    }
                }

                std::string tmpKeyStr = "";
                std::stringstream stream;
                std::string indexStr;
                stream << amount;
                stream >> indexStr;
                tmpKeyStr = "name" + indexStr;
                sendJ[tmpKeyStr] = onlinePlayer[i];
                tmpKeyStr = "rate" + indexStr;
                sendJ[tmpKeyStr] = rate;
                tmpKeyStr = "rank" + indexStr;
                sendJ[tmpKeyStr] = rank;
                amount++;
            }
        }
        sendJ["amount"] = amount;
    }
    if (symbol == "myinfo")
    {
        sendJ["symbol"] = "myinfo";
        sendJ["end"] = "end";
        std::string name = recvJ["name"];
        Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
        PlayerInfo qHelper;
        struct PlayerInfo playerinfo;
        auto query = playerMapper.Query(qHelper)
                .ToVector();
        for (auto q : query)
        {
            if (q.name == name)
            {
                playerinfo = q;
                break;
            }
        }
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
    if (symbol == "hunt")
    {
        sendJ["symbol"] = "hunt";
        sendJ["end"] = "end";
        std::string kindStr = recvJ["kind"];
        std::string name = recvJ["name"];
        std::string owner = recvJ["owner"];
        int level = 1;
        Kind kind;
        for (int i = 0; i < (sizeof(kindOfString)/sizeof(kindOfString[0])); i++)
        {
            if (kindStr == kindOfString[i])
            {
                kind = (Kind)i;
            }
        }
        PokemonFactory *pokemonFactory = new PokemonFactory();
        Pokemon* caughtPokemon = pokemonFactory->CreatePokemon(kind, level, name, owner);
        struct PokemonInfo pokemoninfo = caughtPokemon->ToPokeStruInfo();

        //Get player information
        std::string playerName = onlinePlayer[pid];
        struct PlayerInfo playerinfo;
        Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
        PlayerInfo qHelper;
        auto query = playerMapper.Query(qHelper)
                .ToVector();
        for (auto q : query)
        {
            if (q.name == playerName)
            {
                playerinfo = q;
                break;
            }
        }

        playerinfo.pokemonNumber += 1;
        //If player's package not full, add pokemon into package.Else into storage
        if (playerinfo.packageCapacity > 0)
        {
            playerinfo.packageCapacity -= 1;
            Poor_ORM::ORMapper<PokemonInfo> pokePackMapper ("pokePackage.db");
            pokePackMapper.Insert(pokemoninfo);
        }
        else
        {
            Poor_ORM::ORMapper<PokemonInfo> pokeStgMapper("pokeStorage.db");
            pokeStgMapper.Insert(pokemoninfo);
        }
        playerMapper.Update(playerinfo);
    }
    if (symbol == "rank")
    {
        sendJ["symbol"] = "rank";
        sendJ["end"] = "end";
        std::string player = recvJ["name"];
        Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
        struct PlayerInfo qHelper;
        auto query = playerMapper.Query(qHelper)
                .ToVector();
        std::sort(query.begin(), query.end(), SortByRank);
        std::stringstream stream;
        std::string numStr;
        std::string nameKey;
        std::string rankKey;
        std::string rateKey;
        int amount = 0;
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
        sendJ["amount"] = amount;
    }
    if (symbol == "playerPoke")
    {
        std::string name = recvJ["name"];
        sendJ["symbol"] = "playerPoke";
        sendJ["end"] = "end";
        Poor_ORM::ORMapper<PokemonInfo> pokePackMapper ("pokePackage.db");
        PokemonInfo qHelper;
        auto query = pokePackMapper.Query(qHelper)
                .ToVector();
        std::stringstream stream;
        std::string indexStr;
        std::string keyStr;
        int index = 0;
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
        Poor_ORM::ORMapper<PokemonInfo> pokeStoreMapper ("pokeStorage.db");
        query = pokeStoreMapper.Query(qHelper)
                .ToVector();
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
    if (symbol == "thumb")
    {
        std::string name = recvJ["name"];
        Poor_ORM::ORMapper<PlayerInfo> playerMapper ("playerinfo.db");
        PlayerInfo qHelper;
        auto query = playerMapper.Query(qHelper)
                .ToVector();
        for (auto q : query)
        {
            if (q.name == name)
            {
                q.thumb += 1;
                playerMapper.Update(q);
                break;
            }
        }
        sendJ["symbol"] = "thumb";
        sendJ["end"] = "end";
    }
    helper->setSendStrHelper(sendJ.dump());
    std::string strSend = helper->getSendStrHelper();
    helper->setSendStrHelper("");
    return strSend;
}

DWORD WINAPI SendThreadFunc(int pid, LPVOID cParam, LPVOID sParam, LPVOID hParam)
{
    SOCKET* clientSocket = (SOCKET*)cParam;
    Helper* helper = (Helper*)hParam;
    std::string strSend = GetSendStr(pid, helper);
    std::cout << "strSend to " << pid << ":" << strSend << std::endl;
    send(*clientSocket, strSend.c_str(), strSend.length(), NULL);
    return 0;
}

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
