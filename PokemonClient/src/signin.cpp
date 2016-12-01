#include "ui_signin.h"
#include <QMessageBox>
#include <iostream>
#include "signin.h"

SignIn::SignIn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignIn)
{
    ui->setupUi(this);
    this->setGeometry(400, 200, 400, 300);
    this->setWindowTitle("Sign In");
    QObject::connect(this->ui->signInButton, SIGNAL(clicked(bool)), this, SLOT(onSignInClicked()));
    QObject::connect(this->ui->topButtonSignOn, SIGNAL(clicked(bool)), this, SLOT(onTopSignOnClicked()));
}

SignIn::SignIn(Helper* h, SocketClient* sc, QWidget *parent) :
    QWidget(parent),
    helper(h),
    socketClient(sc),
    ui(new Ui::SignIn)
{
    ui->setupUi(this);
    this->setGeometry(400, 200, 400, 300);
    this->setWindowTitle("Sign In");
    QObject::connect(this->ui->signInButton, SIGNAL(clicked(bool)), this, SLOT(onSignInClicked()));
    QObject::connect(this->ui->topButtonSignOn, SIGNAL(clicked(bool)), this, SLOT(onTopSignOnClicked()));
}

SignIn::~SignIn()
{
    delete helper;
    delete socketClient;
    delete ui;
}

void SignIn::receiveSwitch()
{
    this->ui->userLineEdit->clear();
    this->ui->pwLineEdit->clear();
    this->setGeometry(400, 200, 400, 300);
    this->show();
}

void SignIn::onTopSignOnClicked()
{
    this->hide();
    emit switchToSignOn();
}

DWORD WINAPI SendThreadFunc(LPVOID lParam, LPVOID hParam);
DWORD WINAPI RecvThreadFunc(LPVOID lParam, LPVOID hParam);

void SignIn::onSignInClicked()
{
    if (this->ui->userLineEdit->text().isEmpty())
        QMessageBox::information(this, "Error", QString::fromStdString("Username can't be empty"));
    else
    {
        if (this->ui->pwLineEdit->text().isEmpty())
            QMessageBox::information(this, "Error", QString::fromStdString("Password can't be empty"));
        else
        {
            json j;
            j["symbol"] = "signin";
            j["username"] = this->ui->userLineEdit->text().toStdString();
            j["password"] = this->ui->pwLineEdit->text().toStdString();
            j["end"] = "end";
            std::string str = j.dump();

            helper->setSendStrHelper(str);
            std::thread sigininSendThread = std::thread(SendThreadFunc, socketClient, helper);
            sigininSendThread.join();

            std::thread signinRecvThread = std::thread(RecvThreadFunc, socketClient, helper);
            signinRecvThread.join();

            std::string recvStr = helper->getRecvStrHelper();
            helper->setRecvStrHelper("");
            json recvJ = json::parse(recvStr);
            bool useravailable = recvJ["useravailable"];
            bool passwordcorrect = recvJ["passwordcorrect"];

            if (!useravailable)
            {
                QMessageBox::information(this, "Info", "User not logged please sign on");
            }
            else if (passwordcorrect)
            {
                this->ui->userLineEdit->clear();
                this->ui->pwLineEdit->clear();
                this->hide();
                emit switchToMainPage();
            }
            else
            {
                QMessageBox::information(this, "Info", "Wrong password");
            }
        }
    }
}

DWORD WINAPI SendThreadFunc(LPVOID lParam, LPVOID hParam)
{
    Helper* helper = (Helper*)hParam;
    SocketClient *socketClient = (SocketClient*)lParam;
    int iResult;
    SOCKET ConnectSocket = socketClient->getConnectSocket();
    std::string sendStr = helper->getSendStrHelper();
    helper->setSendStrHelper("");
    size_t len = sendStr.length();
    char *sendbuf = new char[len];
    strcpy(sendbuf, sendStr.data());
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

DWORD WINAPI RecvThreadFunc(LPVOID lParam, LPVOID hParam)
{
    Helper* helper = (Helper*)hParam;
    SocketClient* socketClient = (SocketClient*)lParam;
    SOCKET ConnectSocket = socketClient->getConnectSocket();
    socketClient->ClearRecvBuf();
    socketClient->iResult = recv(ConnectSocket, socketClient->recvbuf, socketClient->recvbuflen, 0);
    if (socketClient->iResult > 0)
    {
        helper->setRecvStrHelper(socketClient->GetRecvStr());
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
