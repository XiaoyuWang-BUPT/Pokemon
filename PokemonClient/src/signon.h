/** Copyright : bupt
    Author : RainING1947
    Date : 2016/12/23
    Description : sign on page
**/

#ifndef SIGNON_H
#define SIGNON_H

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

    /** Function : Acquaintance(SocketClient*, QWidget*)
        Description : Construct function
        Calls : None
        Input : A client socket for communicating with server
        Return : None
        Others : None **/
    explicit SignOn(SocketClient* sc, QWidget *parent = 0);

    /** Function : Acquaintance(QWidget*)
        Description : Default Construct function
        Calls : None
        Input : None
        Return : None
        Others : None **/
    explicit SignOn(QWidget *parent = 0);
    ~SignOn();

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
    Ui::SignOn *ui;

    //username lineedit not clicked, first time change cursor
    bool isUNNotCLK = true;

    //password lineedit not clicked, first time change cursor
    bool isPWNotCLK = true;

    //ensure password lineedit not clicked, first time change cursor
    bool isEPWNotCLK = true;

    //received information
    std::string recvString = "";

private slots:    
    /** Function : onTopSignInClicked
        Description : turn to sign in operation
        Calls : hide()
        Input : None
        Return : None
        Others : None **/
    void onTopSignInClicked();

    /** Function : onSignOnClicked
        Description : process when sign on button is clicked
        Calls :
        Input : None
        Return : None
        Others : None **/
    void signOnButtonClicked();

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
    Q_INVOKABLE bool setRecvStrSignOn(QString s);

    /** Function : eventFilter
        Description : overwrite eventFilter to watch ui items
        Calls :
        Input : watched object, event
        Return : None
        Others : None **/
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    /** Signal : switchToSignIn
        Description : switch to sign on page
        Calls :
        Input : None
        Return : None
        Others : None **/
    void switchToSignIn();

    /** Signal : switchToAcq
        Description : switch to acquaintance page
        Calls :
        Input : None
        Return : None
        Others : None **/
    void switchToAcq();
};

#endif // SIGNON_H
