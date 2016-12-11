#ifndef BATTLE_H
#define BATTLE_H

#include "socketClient.h"
#include <QWidget>
#include <QString>
#include <QEvent>
#include "lib/json.hpp"

using json = nlohmann::json;

namespace Ui {
class Battle;
}

class Battle : public QWidget
{
    Q_OBJECT

public:
    explicit Battle(SocketClient *sc, QWidget *parent = 0);
    explicit Battle(QWidget *parent = 0);
    ~Battle();

private:
    Ui::Battle *ui;
    SocketClient *socketClient;
    void SendAndRecvFunc(json j);

signals:
    void switchToMainPage();

private slots:
    void receiveSwitch(QString nature, QString standard);
    void getRecvStr(QString recvStr);
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // BATTLE_H
