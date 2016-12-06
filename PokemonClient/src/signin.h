#ifndef SIGNIN_H
#define SIGNIN_H

#pragma once
#include "helper.h"
#include "socketClient.h"
#include <QWidget>
#include <QMessageBox>
#include <QString>
#include "mainpage.h"
#include "signon.h"
#include "lib/json.hpp"


using json = nlohmann::json;

namespace Ui {
class SignIn;
}

class SignIn : public QWidget
{
    Q_OBJECT

public:
    SocketClient *socketClient;
    explicit SignIn(SocketClient* sc, QWidget *parent = 0);
    explicit SignIn(QWidget *parent = 0);
    ~SignIn();

private:
    Ui::SignIn *ui;
    std::string recvString = "";

private slots:
    void onSignInClicked();
    void onTopSignOnClicked();
    void receiveSwitch();
    Q_INVOKABLE bool setRecvStrSignIn(QString s);
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void switchToSignOn();
    void switchToMainPage();
};

#endif // !SIGNIN_H
