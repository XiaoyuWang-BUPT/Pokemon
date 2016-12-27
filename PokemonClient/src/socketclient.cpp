#include "socketClient.h"

void SocketClient::Prepare()
{
    this->InitWinSock();
    this->ResolveAddrPort();
    this->ConnectToServer();
}

void SocketClient::InitWinSock()
{
    //start up socket
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        std::cout << "WSAStarttup failed with error " << iResult << std::endl;
        system("pause");
        return;
    }

    //initialize address information
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
}

void SocketClient::ResolveAddrPort()
{
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        std::cout << "Getaddrinfo failed with error " << iResult << std::endl;
        system("pause");
        return;
    }
}

void SocketClient::ConnectToServer()
{
    //Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        //Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family,
            ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET)
        {
            std::cout << "Socket failed with error " << WSAGetLastError() << std::endl;
            WSACleanup();
            system("pause");
            return;
        }

        //Connect to server
        iResult = connect(ConnectSocket, ptr->ai_addr,
            (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET)
    {
        std::cout << "Unable to connect to server" << std::endl;
        WSACleanup();
        system("pause");
        return;
    }
}

void SocketClient::Try()
{
    //try to connect to server until get permision
    do {
        ClearRecvBuf();
        Receive();
    } while (GetRecvStr() != "permision");
}

void SocketClient::Shutdown()
{
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        std::cout << "Shutdown failed with error " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        system("pause");
        return;
    }
}

void SocketClient::setPlayerName(std::string playerName)
{
    this->playerName = playerName;
}

std::string SocketClient::getPlayerName()
{
    return this->playerName;
}

int SocketClient::Receive()
{
    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0)
        std::cout << "Recved Str: " << GetRecvStr() << std::endl;
    else if (iResult == 0)
        std::cout << "Connection closed" << std::endl;
    else
        std::cout << "recv failed with error " << WSAGetLastError() << std::endl;
    return iResult;
}

void SocketClient::ClearRecvBuf()
{
    for (auto& c : recvbuf) { c = NULL; }
}

void SocketClient::Cleanup()
{
    closesocket(ConnectSocket);
    WSACleanup();
}

std::string SocketClient::GetRecvStr()
{
    std::string recvStr = this->recvbuf;
    return recvStr;
}

SOCKET SocketClient::getConnectSocket() { return ConnectSocket; }
