#include "signin.h"
#include "signon.h"
#include "mainpage.h"
#include "hunt.h"
#include "battle.h"
#include "acquaintance.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //get a SocketClient object connect to server
    SocketClient *socketClient = new SocketClient();
    socketClient->Prepare();
    socketClient->Try();
    socketClient->setPlayerName("");

    //get ui objects
    SignIn *signin = new SignIn(socketClient);
    SignOn *signon = new SignOn(socketClient);
    Acquaintance *acquaintance = new Acquaintance(socketClient);
    MainPage *mainpage = new MainPage(socketClient);
    Hunt *hunt = new Hunt(socketClient);
    Battle *battle = new Battle(socketClient);
    signin->show();

    //connect signals and slots between different objects
    QObject::connect(signin, SIGNAL(switchToSignOn()), signon, SLOT(receiveSwitch()));
    QObject::connect(signin, SIGNAL(switchToMainPage()), mainpage, SLOT(receiveSwitch()));
    QObject::connect(signon, SIGNAL(switchToSignIn()), signin, SLOT(receiveSwitch()));
    QObject::connect(signon, SIGNAL(switchToAcq()), acquaintance, SLOT(receiveSwitch()));
    QObject::connect(acquaintance, SIGNAL(SwitchToMainPage()), mainpage, SLOT(receiveSwitch()));
    QObject::connect(hunt, SIGNAL(switchToMainPage()), mainpage, SLOT(receiveSwitch()));
    QObject::connect(mainpage, SIGNAL(SwitchToHunt()), hunt, SLOT(receiveSwitch()));
    QObject::connect(mainpage, SIGNAL(SwitchToBattle(QString,QString)), battle, SLOT(receiveSwitch(QString,QString)));
    QObject::connect(battle, SIGNAL(switchToMainPage()), mainpage, SLOT(receiveSwitch()));

    return a.exec();
}
