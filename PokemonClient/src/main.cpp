#include "signin.h"
#include "signon.h"
#include "mainpage.h"
#include "hunt.h"
#include "battle.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SocketClient *socketClient = new SocketClient();
    socketClient->Prepare();
    socketClient->Try();
    socketClient->setPlayerName("");

    SignIn *signin = new SignIn(socketClient);
    SignOn *signon = new SignOn(socketClient);
    MainPage *mainpage = new MainPage(socketClient);
    Hunt *hunt = new Hunt(socketClient);
    Battle *battle = new Battle(socketClient);
    signin->show();
    QObject::connect(signin, SIGNAL(switchToSignOn()), signon, SLOT(receiveSwitch()));
    QObject::connect(signin, SIGNAL(switchToMainPage()), mainpage, SLOT(receiveSwitch()));
    QObject::connect(signon, SIGNAL(switchToSignIn()), signin, SLOT(receiveSwitch()));
    QObject::connect(signon, SIGNAL(switchToMainPage()), mainpage, SLOT(receiveSwitch()));
    QObject::connect(hunt, SIGNAL(switchToMainPage()), mainpage, SLOT(receiveSwitch()));
    QObject::connect(mainpage, SIGNAL(switchToHunt()), hunt, SLOT(receiveSwitch()));
    QObject::connect(mainpage, SIGNAL(SwitchToBattle(QString,QString)), battle, SLOT(receiveSwitch(QString,QString)));
    QObject::connect(battle, SIGNAL(switchToMainPage()), mainpage, SLOT(receiveSwitch()));

    return a.exec();
}
