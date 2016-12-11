#include "battle.h"
#include "ui_battle.h"

Battle::Battle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Battle)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->ui->backButton->installEventFilter(this);
}

Battle::Battle(SocketClient *sc, QWidget *parent) :
    Battle(parent)
{
    socketClient = sc;
}

Battle::~Battle()
{
    delete socketClient;
    delete ui;
}

DWORD WINAPI SendThreadFuncBattle(LPVOID lParam, LPVOID sParam);
DWORD WINAPI RecvThreadFuncBattle(SocketClient* socketClient, Battle* battle);

void Battle::SendAndRecvFunc(json j)
{
    std::string str = j.dump();
    std::thread battleSendThread = std::thread(SendThreadFuncBattle, socketClient, &str);
    battleSendThread.join();
    std::thread battleRecvThread = std::thread(RecvThreadFuncBattle, socketClient, this);
    battleRecvThread.join();
    return;
}

void Battle::receiveSwitch(QString nature, QString standard)
{
    json j;
    j["symbol"] = "battlepre";
    j["nature"] = nature.toStdString();
    j["standard"] = standard.toStdString();
    j["name"] = socketClient->getPlayerName();
    j["end"] = "end";
    SendAndRecvFunc(j);
    this->show();
}

void Battle::getRecvStr(QString recvStr)
{

}

bool Battle::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->ui->backButton)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            this->hide();
            emit switchToMainPage();
        }
    }
    return QWidget::eventFilter(watched, event);
}

DWORD WINAPI RecvThreadFuncBattle(SocketClient* socketClient, Battle* battle)
{
    SOCKET ConnectSocket = socketClient->getConnectSocket();
    socketClient->ClearRecvBuf();
    socketClient->iResult = recv(ConnectSocket, socketClient->recvbuf, socketClient->recvbuflen, 0);
    if (socketClient->iResult > 0)
    {
        QMetaObject::invokeMethod(battle, "getRecvStr", Qt::DirectConnection, Q_ARG(QString, QString::fromStdString(socketClient->GetRecvStr())));
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
DWORD WINAPI SendThreadFuncBattle(LPVOID lParam, LPVOID sParam)
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
