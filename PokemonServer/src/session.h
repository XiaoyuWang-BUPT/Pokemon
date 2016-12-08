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
                    onlinePlayer[pid] = std::make_pair(q.name, q.rank);
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
            struct PlayerInfo p = {nameRecv, pwRecv, 0, CAPACITY, 9999, 0, s, "000"};
            sendJ["signonsuccess"] = playerMapper.Insert(p);
            onlinePlayer[pid] = std::make_pair(p.name, p.rank);
        }
    }
    if (symbol == "onlinePlayer")
    {
        sendJ["symbol"] = "onlinePlayer";
        sendJ["amount"] = 0;
        sendJ["end"] = "end";
        for (int i = 0; onlinePlayer[i] != nullPlayerPair; i++)
        {
            std::string tmpKeyStr = "";
            std::stringstream stream;
            std::string indexStr;
            stream << (i + 1);
            stream >> indexStr;
            sendJ["amount"] = i + 1;
            tmpKeyStr = "name" + indexStr;
            sendJ[tmpKeyStr] = onlinePlayer[i].first;
            tmpKeyStr = "rank" + indexStr;
            sendJ[tmpKeyStr] = onlinePlayer[i].second;
        }
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
        std::string playerName = onlinePlayer[pid].first;
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
    if (symbol == "playerPoke")
    {
        std::string name = recvJ["name"];
        sendJ["symbol"] = "playerPoke";
        sendJ["end"] = "end";
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
    onlinePlayer[pid] = std::make_pair("", NULL);
    delete helper;

    return 0;
}

#endif // SESSION_H
