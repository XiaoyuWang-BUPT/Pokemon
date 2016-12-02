#include "signin.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Helper* helper = new Helper();
    SocketClient *socketClient = new SocketClient();
    socketClient->Prepare();
    socketClient->Try();

    SignIn *signin = new SignIn(helper, socketClient);
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
