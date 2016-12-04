#include "mainpage.h"
#include "ui_mainpage.h"
#include <sstream>

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    this->setWindowTitle("MainPage");
    QMovie* movie = new QMovie(":/pikachu.gif");
    this->ui->welcomeLabel->setMovie(movie);
    movie->start();
    QObject::connect(this->ui->reloadButton, SIGNAL(clicked(bool)), this, SLOT(onReloadClicked()));
}

MainPage::MainPage(SocketClient* sc, QWidget *parent) :
    MainPage(parent)
{
    socketClient = sc;
}

MainPage::~MainPage()
{
    delete socketClient;
    delete ui;
}

DWORD WINAPI RecvdThreadFunc(SocketClient* socketClient, MainPage* mainpage);
DWORD WINAPI SendThreadFuncMainpage(LPVOID lParam, LPVOID sParam);

void MainPage::receiveSwitch()
{
    this->show();
    calledThread = std::thread(RecvdThreadFunc, socketClient, this);
    calledThread.detach();
}

void MainPage::ReloadOnlinePlayer(json &recvJ)
{
    int amount = recvJ["amount"];
    for (int i = 0; i < amount; i++)
    {
        std::stringstream stream;
        std::string indexStr;
        stream << (i + 1);
        stream >> indexStr;
        std::string tmpKeyStr = "name" + indexStr;
        std::cout << recvJ[tmpKeyStr] << ":";
        tmpKeyStr = "rank" + indexStr;
        std::cout << recvJ[tmpKeyStr] << std::endl;
    }
}

bool MainPage::getRecvStr(QString str)
{
    recvString = str.toStdString();
    json recvJ = json::parse(recvString);
    recvString = "";

    std::string symbol = recvJ["symbol"];
    if (symbol == "onlinePlayer")
    {
        ReloadOnlinePlayer(recvJ);
    }
    return true;
}

void MainPage::onReloadClicked()
{
    json j;
    j["symbol"] = "onlinePlayer";
    j["end"] = "end";
    std::string str = j.dump();

    std::thread mainpageSendThread = std::thread(SendThreadFuncMainpage, socketClient, &str);
    mainpageSendThread.join();
}

DWORD WINAPI RecvdThreadFunc(SocketClient* socketClient, MainPage* mainpage)
{
    ::Sleep(1000);
    QMetaObject::invokeMethod(mainpage, "setStyleSheet", Q_ARG(QString, "background-color:rgb(238, 232, 171)"));

    SOCKET ConnectSocket = socketClient->getConnectSocket();
    socketClient->ClearRecvBuf();
    socketClient->iResult = recv(ConnectSocket, socketClient->recvbuf, socketClient->recvbuflen, 0);
    if (socketClient->iResult > 0)
    {
        QMetaObject::invokeMethod(mainpage, "getRecvStr", Qt::DirectConnection, Q_ARG(QString, QString::fromStdString(socketClient->GetRecvStr())));
    }
    else if (socketClient->iResult == 0)
    {
        std::cout << "Connection closed" << std::endl;
    }
    else
    {
        std::cout << "recv failed with error " << WSAGetLastError() << std::endl;
    }
    return 0;
}

DWORD WINAPI SendThreadFuncMainpage(LPVOID lParam, LPVOID sParam)
{
    std::string *sendStr = (std::string*)sParam;
    SocketClient *socketClient = (SocketClient*)lParam;
    int iResult;
    SOCKET ConnectSocket = socketClient->getConnectSocket();
    size_t len = (*sendStr).length();
    char *sendbuf = new char[len];
    strcpy(sendbuf, (*sendStr).data());
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR)
    {
        std::cout << "Send failed with error " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        system("pause");
    }
    return 0;
}
