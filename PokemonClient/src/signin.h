#ifndef SIGNIN_H
#define SIGNIN_H

#pragma once

#include <QWidget>
#include <QMessageBox>
#include <QString>
#include <string>
#include "mainpage.h"
#include "signon.h"

namespace Ui {
class SignIn;
}

class SignIn : public QWidget
{
    Q_OBJECT

public:
    explicit SignIn(QWidget *parent = 0);
    ~SignIn();
private:
    Ui::SignIn *ui;
    bool userAvailabel(std::string name);
private slots:
    void onSignInClicked();
    void receiveSwitch();
signals:
    void switchToSignOn();
    void switchToMainPage();
};

#endif // !SIGNIN_H
