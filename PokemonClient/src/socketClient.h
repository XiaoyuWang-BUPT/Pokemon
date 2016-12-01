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

#define DEFAULT_BUFLEN 128
#define DEFAULT_PORT "27015"

DWORD WINAPI SendThreadFunc(LPVOID lParam);
DWORD WINAPI RecvThreadFunc(LPVOID lParam);

class SocketClient
{
public:
	SocketClient() {}
	~SocketClient() {}
	void Prepare();
	void InitWinSock();
	void ResolveAddrPort();
	void ConnectToServer();
	void Try();
	void Run();
	void Shutdown();
	int Receive();
	void Cleanup();
	void ClearRecvBuf();
	std::string GetRecvStr();
	SOCKET getConnectSocket();
    char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	std::string sendStr = "";
	std::thread sendThread;
	std::thread recvThread;
private:
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;
};

void SocketClient::Prepare()
{
	this->InitWinSock();
	this->ResolveAddrPort();
	this->ConnectToServer();
}

void SocketClient::InitWinSock()
{
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cout << "WSAStarttup failed with error " << iResult << std::endl;
		system("pause");
		return;
	}

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
	do {
		ClearRecvBuf();
		Receive();
	} while (GetRecvStr() != "permision");
}

void SocketClient::Run()
{
	sendThread = std::thread(SendThreadFunc, this);
	recvThread = std::thread(RecvThreadFunc, this);
    sendThread.join();
    recvThread.join();
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

DWORD WINAPI SendThreadFunc(LPVOID lParam)
{
	SocketClient *socketClient = (SocketClient*)lParam;
	int iResult;
	SOCKET ConnectSocket = socketClient->getConnectSocket();
	do {
		if (socketClient->sendStr == "")
			continue;
		size_t len = socketClient->sendStr.length();
		char *sendbuf = new char[len];
		strcpy(sendbuf, socketClient->sendStr.data());
		iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
		if (iResult == SOCKET_ERROR)
		{
			std::cout << "Send failed with error " << WSAGetLastError() << std::endl;
			closesocket(ConnectSocket);
			WSACleanup();
			system("pause");
			return 0;
		}
        socketClient->sendStr = "";
        break;
    } while (true);
	return 0;
}

DWORD WINAPI RecvThreadFunc(LPVOID lParam)
{
	SocketClient* socketClient = (SocketClient*)lParam;
	memset(socketClient->recvbuf, NULL, socketClient->recvbuflen);
	SOCKET ConnectSocket = socketClient->getConnectSocket();
	while (true)
	{
		socketClient->iResult = recv(ConnectSocket, socketClient->recvbuf, socketClient->recvbuflen, 0);
		if (socketClient->iResult > 0)
		{
			//TODO 收到服务器的数据后如何处理
            extern std::string recvStrHelper;
            recvStrHelper = socketClient->GetRecvStr();
            break;
        }
		else if (socketClient->iResult == 0)
		{
			std::cout << "Connection closed" << std::endl;
			break;
		}
		else
		{
			std::cout << "recv failed with error " << WSAGetLastError() << std::endl;
			break;
		}
		memset(socketClient->recvbuf, NULL, DEFAULT_BUFLEN);
	}
	return 0;
}

#endif //!SOCKET_CLIENT_H
