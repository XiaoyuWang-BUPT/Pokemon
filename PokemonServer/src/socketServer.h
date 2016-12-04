#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <memory.h>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 128
#define MAXSIZE_POOL 3

SOCKET cSock[MAXSIZE_POOL] = { INVALID_SOCKET };
std::pair<std::string, int> onlinePlayer[MAXSIZE_POOL] = {std::make_pair("", NULL)};
const std::pair<std::string, int> nullPlayerPair = std::make_pair("", NULL);
const std::string Permision = "permision";
const std::string Deny = "Thread pool is full, please wait";

class SocketServer
{
public:
	SocketServer(){}
	~SocketServer(){}
	void Prepare();
	void InitWinSock();
	void InitCheckListenSock();
	void BindSock();
	void ListenSock();
	void ShutDown();
	int existingClientCount = 0;
	SOCKET ListenSocket = INVALID_SOCKET;
private:
	WSADATA wsaData;
	int iResult;
	struct addrinfo *result = NULL;
	struct addrinfo hints;
};

void SocketServer::Prepare()
{
	InitWinSock();
	InitCheckListenSock();
	BindSock();
	ListenSock();
}

//Initialize WinSock
void SocketServer::InitWinSock()
{
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cout << "WSAStartup Failed " << iResult << std::endl;
		WSACleanup();
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		std::cout << "Getaddrinfo failed with error " << iResult << std::endl;
		WSACleanup();
		//system("pause");
		return;
	}
}

//Initialize ListenSocket and double check
void SocketServer::InitCheckListenSock()
{
	ListenSocket = socket(result->ai_family,
		result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		std::cout << "Error at socket() " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
}

//Bind socket to IP address or port
void SocketServer::BindSock()
{
	/*iResult = bind(ListenSocket, result->ai_addr,
		(int)result->ai_addrlen);*/
	bind(ListenSocket, result->ai_addr,
		(int)result->ai_addrlen);

	if (iResult == SOCKET_ERROR)
	{
		std::cout << "Bind failed with error " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	/*Once bind function is called, the address information returned by the 
	*getadddrinfo function is no longer needed.The freeaddrinfo function is called to
	*free the memory allocated*/
	freeaddrinfo(result);
}

//Listen on a socket
void SocketServer::ListenSock()
{
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Listen failed with error: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
}


void SocketServer::ShutDown()
{
	for (auto& ClientSocket : cSock)
	{
		iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR)
		{
			std::cout << "Shutdown failed with error" << WSAGetLastError() << std::endl;
			closesocket(ClientSocket);
			WSACleanup();
			return;
		}
		closesocket(ClientSocket);
		WSACleanup();
	}
	system("pause");
}

#endif //!SOCKET_SERVER_H
