#ifndef SESSION_H
#define SESSION_H

#include "lib/helper.h"
#include "socketServer.h"
#include <thread>
#include "pokemon.h"
#include "player.h"
#include "lib/json.hpp"
#include <iostream>

using json = nlohmann::json;

std::string GetSendStr(Helper* helper)
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
                }
                else
                {
                    sendJ["passwordcorrect"] = false;
                }
                break;
            }
        }
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
    std::string strSend = GetSendStr(helper);
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
    delete helper;

    return 0;
}

#endif // SESSION_H
