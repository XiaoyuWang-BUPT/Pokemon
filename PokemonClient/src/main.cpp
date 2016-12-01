#include "mainpage.h"
#include "signin.h"
#include "signon.h"
#include "socketClient.h"
#include <thread>
#include <QApplication>
#include <QDebug>

std::string sendStrHelper = "";
std::string recvStrHelper = "";

DWORD WINAPI sessionFunc();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);    

    std::thread sessionThread;
    sessionThread = std::thread(sessionFunc);
    sessionThread.detach();

    SignIn *signin = new SignIn();
    SignOn *signon = new SignOn();
    MainPage *mainpage = new MainPage();
    signin->setGeometry(400, 200, 400, 300);
    signin->show();
    QObject::connect(signin, SIGNAL(switchToSignOn()), signon, SLOT(receiveSwitch()));
    QObject::connect(signin, SIGNAL(switchToMainPage()), mainpage, SLOT(receiveSwitch()));
    QObject::connect(signon, SIGNAL(switchToSignIn()), signin, SLOT(receiveSwitch()));
    QObject::connect(signon, SIGNAL(switchToMainPage()), mainpage, SLOT(receiveSwitch()));

    return a.exec();
}

DWORD WINAPI sessionFunc()
{
    while (true)
    {
        if (sendStrHelper != "")
        {
            SocketClient *socketClient = new SocketClient();
            socketClient->Prepare();
            socketClient->Try();
            socketClient->sendStr = sendStrHelper;
            socketClient->Run();
            sendStrHelper = "";
            socketClient->Cleanup();
            delete socketClient;
        }
    }
    return 0;
}
