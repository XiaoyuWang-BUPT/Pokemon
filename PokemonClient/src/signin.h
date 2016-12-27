/** Copyright : bupt
    Author : RainING1947
    Date : 2016/12/23
    Description : sign in page
**/

#ifndef SIGNIN_H
#define SIGNIN_H

#pragma once
#include "socketClient.h"
#include <QWidget>
#include <QMessageBox>
#include <QString>
#include <QMovie>
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

    /** Function : Acquaintance(SocketClient*, QWidget*)
        Description : Construct function
        Calls : None
        Input : A client socket for communicating with server
        Return : None
        Others : None **/
    explicit SignIn(SocketClient* sc, QWidget *parent = 0);

    /** Function : Acquaintance(QWidget*)
        Description : Default Construct function
        Calls : None
        Input : None
        Return : None
        Others : None **/
    explicit SignIn(QWidget *parent = 0);
    ~SignIn();

private:
    /** Function : InitUI
        Description : Initialize UI items
        Calls : setWindowtitle() function
        Input : None
        Return : None
        Others : None **/
    void InitUI();

    /** Function : SetGosankeGif
        Description : Initialize Gosanke gif resource on page
        Calls : setMoive()...
        Input : None
        Return : None
        Others : None **/
    void SetGosankeGif();

    /** Function : SetEventFilter
        Description : install eventFilter for ui items
        Calls : installEventFilter
        Input : None
        Return : None
        Others : None **/
    void SetEventFilter();

    /** Function : InitConnect
        Description : Initialize connect between slots and signals
        Calls : QObject::connect()
        Input : None
        Return : None
        Others : None **/
    void InitConnect();
    Ui::SignIn *ui;

    //received information
    std::string recvString = "";

private slots:
    /** Function : onSignInClicked
        Description : process when sign in button is clicked
        Calls :
        Input : None
        Return : None
        Others : None **/
    void onSignInClicked();

    /** Function : onTopSignOnClicked
        Description : turn to sign on operation
        Calls : hide()
        Input : None
        Return : None
        Others : None **/
    void onTopSignOnClicked();

    /** Function : receiveSwitch
        Description : receive switch signal
        Calls : show()
        Input : None
        Return : None
        Others : None **/
    void receiveSwitch();

    /** Function : setRecvStrSignIn
        Description : process after get information form server
        Calls :
        Input : None
        Return : None
        Others : None **/
    Q_INVOKABLE bool setRecvStrSignIn(QString s);

    /** Function : eventFilter
        Description : overwrite eventFilter to watch ui items
        Calls :
        Input : watched object, event
        Return : None
        Others : None **/
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    /** Signal : switchToSignOn
        Description : switch to sign on page
        Calls :
        Input : None
        Return : None
        Others : None **/
    void switchToSignOn();

    /** Signal : switchToMainPage
        Description : switch to main page
        Calls :
        Input : None
        Return : None
        Others : None **/
    void switchToMainPage();
};

#endif // !SIGNIN_H
