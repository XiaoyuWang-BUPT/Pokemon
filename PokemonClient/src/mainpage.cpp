#include "mainpage.h"
#include "ui_mainpage.h"

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    this->setWindowTitle("MainPage");
    QMovie* movie = new QMovie(":/pikachu.gif");
    this->ui->welcomeLabel->setMovie(movie);
    movie->start();
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

bool MainPage::setChatBox(QString chatContent)
{
    //TODO set chat box content
    std::cout << "In Function <MainPage::setChatBox>" << std::endl;
    return true;
}

DWORD WINAPI CalledThreadFunc(SocketClient* socketClient, MainPage* mainpage)
{
    ::Sleep(1000);
    QMetaObject::invokeMethod(mainpage, "setStyleSheet", Q_ARG(QString, "background-color:rgb(238, 232, 171)"));

    SOCKET ConnectSocket = socketClient->getConnectSocket();
    socketClient->ClearRecvBuf();
    socketClient->iResult = recv(ConnectSocket, socketClient->recvbuf, socketClient->recvbuflen, 0);
    if (socketClient->iResult > 0)
    {
        QMetaObject::invokeMethod(mainpage, "setChatBox", Qt::DirectConnection, Q_ARG(QString, QString::fromStdString(socketClient->GetRecvStr())));
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

void MainPage::receiveSwitch()
{
    //this->setGeometry(100, 100, 640, 480);
    this->show();
    calledThread = std::thread(CalledThreadFunc, socketClient, this);
    calledThread.detach();
}
