/** Copyright : bupt
    Author : RainING1947
    Date : 2016/12/23
    Description : Provide new-comer gosanke to chose
**/

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
    /** Function : Acquaintance(SocketClient*, QWidget*)
        Description : Construct function
        Calls : None
        Input : A client socket for communicating with server
        Return : None
        Others : None **/
    explicit Acquaintance(SocketClient *sc, QWidget *parent = 0);

    /** Function : Acquaintance(QWidget*)
        Description : Default Construct function
        Calls : None
        Input : None
        Return : None
        Others : None **/
    explicit Acquaintance(QWidget *parent = 0);
    ~Acquaintance();
    //Client socket connect with server
    SocketClient *socketClient;

private:
    /** Function : InitUI
        Description : Initialize UI items
        Calls : setWindowtitle() function
        Input : None
        Return : None
        Others : None **/
    void InitUI();

    /** Function : InitConnect
        Description : Initialize connections between signals and slots
        Calls : QObject::connect() function
        Input : None
        Return : None
        Others : None **/
    void InitConnect();

    /** Function : SetEventFilter
        Description : install eventfilter for ui items
        Calls : installEventFilter() function
        Input : None
        Return : None
        Others : None **/
    void SetEventFilter();

    Ui::Acquaintance *ui;

private slots:
    /** Function : receiveSwitch
        Description : Take over reaction after switch to this page
        Calls : show() function
        Input : None
        Return : None
        Others : None **/
    void receiveSwitch();

    /** Function : onChose
        Description : Report to server after player decide his/her 1st pokemon
        Calls : SocketClient::getPlayerName() function, SocketCliet::Receive() function
                SocketClient::ClearRecvBuf() function,  json::dump() function
                hide() function
        Input : None
        Return : None
        Others : using json **/
    void onChose(QObject* obj);

    /** Function : eventFilter
        Description : overwrite eventFilter to monitor ui items
        Calls : None
        Input : None
        Return : None
        Others : None **/
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    /** Signal : SwitchToMainPage
        Description : switch to mainpage
        Others : None **/
    void SwitchToMainPage();

    /** Signal : choseSignal
        Description : player have decided 1st pokemon
        Others : None **/
    void choseSignal(QObject* obj);
};

#endif // ACQUAINTANCE_H
