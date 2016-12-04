#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QMovie>
#include <QImage>
#include "lib/json.hpp"
#include "helper.h"
#include "socketClient.h"

using json = nlohmann::json;

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    SocketClient* socketClient;
    explicit MainPage(SocketClient* sc, QWidget *parent = 0);
    explicit MainPage(QWidget *parent = 0);
    ~MainPage();
private:
    Ui::MainPage *ui;
    std::thread calledThread;
    std::string recvString = "";
    void ReloadOnlinePlayer(json& recvJ);
private slots:
    void receiveSwitch();
    void onReloadClicked();
    Q_INVOKABLE bool getRecvStr(QString str);
};

#endif // !MAINPAGE_H
