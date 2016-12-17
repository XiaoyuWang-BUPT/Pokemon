#ifndef ACQUAINTANCE_H
#define ACQUAINTANCE_H

#include <QWidget>
#include "socketClient.h"
#include "lib/json.hpp"

using json = nlohmann::json;

namespace Ui {
class Acquaintance;
}

class Acquaintance : public QWidget
{
    Q_OBJECT

public:
    explicit Acquaintance(SocketClient *sc, QWidget *parent = 0);
    explicit Acquaintance(QWidget *parent = 0);
    ~Acquaintance();
    SocketClient *socketClient;

private:
    Ui::Acquaintance *ui;

private slots:
    void receiveSwitch();
    void onChose(QObject* obj);
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void SwitchToMainPage();
    void choseSignal(QObject* obj);
};

#endif // ACQUAINTANCE_H
