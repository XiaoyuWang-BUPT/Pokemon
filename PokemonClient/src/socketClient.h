/** Copyright : bupt
    Author : RainING1947
    Date : 2016/12/23
    Description : socket client
**/

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
    /** Function : Prepare
        Description : prepare for recv and send
        Calls : InitWinsock(), ResolveAddrPort(), ConnectToServer()
        Input : None
        Return : None
        Others : None **/
	void Prepare();

    /** Function : InitWinsock
        Description : start up and initialize a socket
        Calls : WSAStartUp()
        Input : None
        Return : None
        Others : None **/
	void InitWinSock();

    /** Function : ResolveAddrPort
        Description : resolve aadrress of the socket
        Calls : getaddrinfo()
        Input : None
        Return : None
        Others : None **/
	void ResolveAddrPort();

    /** Function : ConnectToServer
        Description : try to connect to server until succeed
        Calls : socket(), freeaddrinfo()...
        Input : None
        Return : None
        Others : None **/
	void ConnectToServer();

    /** Function : Try
        Description : try to get permision to transmit until succeed
        Calls : ClearRecvBuf(), Receive()
        Input : None
        Return : None
        Others : None **/
	void Try();

    /** Function : Shutdown
        Description : Shutdown the connect with server
        Calls : shutdown
        Input : None
        Return : None
        Others : None **/
	void Shutdown();

    /** Function : setPlayerName
        Description : socket client owns name of player who's playing
        Calls :
        Input : player's name
        Return : None
        Others : None **/
    void setPlayerName(std::string playerName);

    /** Function : getPlayerName
        Description :
        Calls :
        Input : None
        Return : player's name
        Others : None **/
    std::string getPlayerName();

    /** Function : Receive
        Description : recv from server and do not process after then
        Calls : recv()
        Input : None
        Return : None
        Others : this function is called when client would receive merely a feedback **/
	int Receive();

    /** Function : Cleaup
        Description : close socket and release resource
        Calls : closesocket(), WSACleanup()
        Input : None
        Return : None
        Others : None **/
	void Cleanup();

    /** Function : ClearRecvBuf
        Description : clear recvbuf before recv
        Calls :
        Input : None
        Return : None
        Others : None **/
	void ClearRecvBuf();

    /** Function : GetRecvStr
        Description : return receicved information in std::string
        Calls :
        Input : None
        Return : None
        Others : None **/
	std::string GetRecvStr();

    /** Function : getConnectSocket
        Description : return the connect socket
        Calls :
        Input : None
        Return : connect socket
        Others : None **/
	SOCKET getConnectSocket();

    //received buffer for received information
    char recvbuf[DEFAULT_BUFLEN];

    //return value of socket operation
	int iResult;

    //length of recvbuf
	int recvbuflen = DEFAULT_BUFLEN;
private:
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

    //name of player who's playing with this terminal
    std::string playerName = "";
};

#endif //!SOCKET_CLIENT_H
