#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <Windows.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "advapi32.lib")

#pragma warning(disable:4996)

#define DEFAULT_BUFLEN 10240
#define DEFAULT_PORT "27015"

class SocketClient
{
public:
    SocketClient() {}
    ~SocketClient() { this->Cleanup(); }
	void Prepare();
	void InitWinSock();
	void ResolveAddrPort();
	void ConnectToServer();
	void Try();
	void Shutdown();
    void setPlayerName(std::string playerName);
    std::string getPlayerName();
	int Receive();
	void Cleanup();
	void ClearRecvBuf();
	std::string GetRecvStr();
	SOCKET getConnectSocket();
    char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
private:
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;
    std::string playerName = "";
};

#endif //!SOCKET_CLIENT_H
