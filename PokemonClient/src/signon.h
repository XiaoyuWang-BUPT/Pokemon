#ifndef SIGNON_H
#define SIGNON_H

#include "helper.h"
#include "socketClient.h"
#include <QWidget>
#include <QPalette>
#include <QMovie>
#include <QMouseEvent>
#include <QMessageBox>
#include <iostream>
#include "lib/json.hpp"

using json = nlohmann::json;

namespace Ui {
class SignOn;
}

class SignOn : public QWidget
{
    Q_OBJECT

public:
    SocketClient* socketClient;
    explicit SignOn(QWidget *parent = 0);
    explicit SignOn(SocketClient* sc, QWidget *parent = 0);
    ~SignOn();

private:
    Ui::SignOn *ui;
    bool isUNNotCLK = true; //username lineedit not clicked, first time change cursor
    bool isPWNotCLK = true; //password lineedit not clicked, first time change cursor
    bool isEPWNotCLK = true; //ensure password lineedit not clicked, first time change cursor
    std::string recvString = "";

private slots:    
    void onTopSignInClicked();
    void signOnButtonClicked();
    void receiveSwitch();
    Q_INVOKABLE bool setRecvStrSignOn(QString s);
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void switchToSignIn();
    void switchToMainPage();
};

#endif // SIGNON_H
