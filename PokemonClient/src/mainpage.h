/** Copyright : bupt
    Author : RainING1947
    Date : 2016/12/23
    Description : main page
**/

#ifndef MAINPAGE_H
#define MAINPAGE_H

#pragma once

#include <QWidget>
#include <QMovie>
#include <QImage>
#include <QIcon>
#include <QPixmap>
#include <QToolTip>
#include <QTextCursor>
#include <QPropertyAnimation>
#include <QHelpEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFont>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QUrl>
#include <QScrollBar>
#include <QMessageBox>
#include <vector>
#include "lib/json.hpp"
#include "socketClient.h"

using json = nlohmann::json;
//maximum of player can be shown
#define MAXSIZE_PLAYER 10

//row of pokemon table
#define TABLE_ROW 3

//column of pokemon table
#define TABLE_COL 6

//maximum of pokemon can be shown
#define MAXSIZE_POKEMON 15

//pokemun badge standard
const int PokeNumLevel[3] = {5, 10, 20};

//rank badge standard
const int RankLevel[3] = {10, 50, 100};

//rate badge standard
const double RateLevel[3] = {20, 50, 80};

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    SocketClient* socketClient;
    /** Function : MainPage(SocketClient*, QWidget*)
        Description : Construct function
        Calls : None
        Input : A client socket for communicating with server
        Return : None
        Others : None **/
    explicit MainPage(SocketClient* sc, QWidget *parent = 0);

    /** Function : MainPage(SocketClient*, QWidget*)
        Description : Default Construct function
        Calls : None
        Input : None
        Return : None
        Others : None **/
    explicit MainPage(QWidget *parent = 0);
    ~MainPage();

private:
    /** Function : InitUI
        Description : Initialize UI items
        Calls : setWindowtitle() function
        Input : None
        Return : None
        Others : None **/
    void InitUI();

    /** Function : setEventFilter
        Description : install evnetFilters for ui items
        Calls : installEventFilter()
        Input : None
        Return : None
        Others : None **/
    void SetEventFilter();

    /** Function : SetToolTips()
        Description : set tooltips for ui items
        Calls : setToolTip()
        Input : None
        Return : None
        Others : None **/
    void SetToolTips();

    /** Function : setCursors
        Description : set cursors for ui items
        Calls : setCursor();
        Input : None
        Return : None
        Others : None **/
    void SetCursors();

    /** Function : SetStyles
        Description : set styleSheet for ui items
        Calls : setStyleSheet()
        Input : None
        Return : None
        Others : None **/
    void SetStyles();

    /** Function : InitPlayerArea
        Description : Initialize ui items of rank widget and online user widget
        Calls : setStyleSheet(), hide()...
        Input : None
        Return : None
        Others : None **/
    void InitPlayerArea();

    /** Function : InitPokemonArea
        Description : Initialize ui items of package pokemon and storage pokemon widget
        Calls : insatllEventFilter()...
        Input : None
        Return : None
        Others : None **/
    void InitPokemonArea();

    /** Function : InitConnect
        Description : Initialize connections between signals and slots
        Calls : QObject::connect() function
        Input : None
        Return : None
        Others : None **/
    void InitConnect();

    /** Function : loadOnlinePlayer
        Description : display online players in ui
        Calls : setFont()...
        Input : json received from server
        Return : None
        Others : None **/
    void LoadOnlinePlayer(json &recvJ);

    /** Function : RecvAndSendOnlinePlayer
        Description : communicate with server
        Calls : dump(), join()
        Input : json to be sent to server
        Return : None
        Others : None **/
    void RecvAndSendOnlinePlayer(json j);
    Ui::MainPage *ui;

    //received std::string message
    std::string recvString = "";

    //index of QHBoxLayout to be delete when a pokemon is exchanged
    int delHLayIndex = -1;

    //exchange button is clicked or not
    bool exClicked = false;

    //pokemon table displaying other players' pokemons' information
    QTableWidgetItem* pokeTableItem[MAXSIZE_POKEMON];

    //label and button displaying other players' state
    QLabel* headLabel[MAXSIZE_PLAYER];
    QPushButton* playerPokeButton[MAXSIZE_PLAYER];
    QPushButton* thumbButton[MAXSIZE_PLAYER];
    QPushButton* rankPokeButton[MAXSIZE_PLAYER];
    QPushButton* rankThumbButton[MAXSIZE_PLAYER];

    //layout label and button displaying owner's pokemon
    QVBoxLayout* scrollVLayout;
    QHBoxLayout* scrollHLayout[MAXSIZE_POKEMON];
    QLabel* pokePicLabel[MAXSIZE_POKEMON];
    QLabel* pokeTextLabel[MAXSIZE_POKEMON];
    QPushButton* exButton[MAXSIZE_POKEMON];

    //online players' names
    std::string playerNames[MAXSIZE_PLAYER];

    //rank players' names
    std::string rankPlayerNames[MAXSIZE_PLAYER];

    //names of pokemons which are in package
    std::vector<std::string> packPokemon;

    //names of pokemons which are in storage
    std::vector<std::string> stoPokemon;

private slots:
    /** Function : receiveSwitch
        Description : receive switch siganl
        Calls : hide(), setGeometry()...
        Input : None
        Return : None
        Others : None **/
    void receiveSwitch();

    /** Function : onOnlinePlayerClicked
        Description : change state of online player widget, and communicate with server
        Calls : RecvAndSendOnlinePlayer()
        Input : None
        Return : None
        Others : None **/
    void onOnlinePlayerClicked();

    /** Function : onOnlinePlayerReloadClicked
        Description : Communicate with server and reload online player widget
        Calls : RecvAndSendOnlinePlayer()
        Input : None
        Return : None
        Others : None **/
    void onOnlinePlayerReloadClicked();

    /** Function : onPlayerPokeClicked
        Description :
        Calls : report to server which player is chosen to look through
        Input : index of chosen player in online player array
        Return : None
        Others : None **/
    void onPlayerPokeClicked(int i);

    /** Function : onPlayerThumbClicked
        Description : report to server which player is chosen to give a thumb
        Calls : RecvAndSendOnlinePlayer()
        Input : index of chosen player in online player array
        Return : None
        Others : None **/
    void onPlayerThumbClicked(int i);

    /** Function : onMyInfoClicked
        Description : report and fetch information about player him/her self
        Calls : RecvAndSendOnlinePlayer()
        Input : json to be sent to sever
        Return : None
        Others : None **/
    void onMyInfoClicked();

    /** Function : onRankClicked
        Description : report and fetch rank information
        Calls : RecvAndSendOnlinePlayer()
        Input : None
        Return : None
        Others : None **/
    void onRankClicked();

    /** Function : onRankPokeClicked
        Description : report to server which player is chosen to look through
        Calls : RecvAndSendOnlinePlayer()
        Input : index of chosen player in rank player array
        Return : None
        Others : None **/
    void onRankPokeClicked(int i);

    /** Function : onRankThumbClicked
        Description : report to server which player is chosen to give a thumb
        Calls : RecvAndSendOnlinePlayer()
        Input : index of chosen player in rank player array
        Return : None
        Others : None **/
    void onRankThumbClicked(int i);

    /** Function : onPackageClicked
        Description : report and fetch package information
        Calls : RecvAndSendOnlinePlayer()
        Input : None
        Return : None
        Others : None **/
    void onPackageClicked();

    /** Function : onStorageClicked
        Description : report and fetch storage information
        Calls : RecvAndSendOnlinePlayer()
        Input : None
        Return : None
        Others : None **/
    void onStorageClicked();

    /** Function : onUpDownClicked
        Description : change the state of battle choice widget
        Calls : height(), setToolTip()...
        Input : None
        Return : None
        Others : using QPropertyAnimation **/
    void onUpDownClicked();

    /** Function : onEXButtonClicked
        Description : report to server which pokemon and int which container is chosen to exchange
        Calls : RecvAndSendOnlinePlayer()
        Input : container, index of pokemon in corresponding container
        Return : None
        Others : None **/
    void onExButtonClicked(QObject* obj, int i);

    /** Function : getRecvStr
        Description : process after received information from server
        Calls :
        Input : received std::string
        Return : None
        Others : None **/
    Q_INVOKABLE bool getRecvStr(QString str);

    /** Function : eventFilter
        Description : overwrite eventFilter to monitor ui items
        Calls :
        Input : watched object, event
        Return : None
        Others : None **/
    bool eventFilter(QObject *watched, QEvent *event);

    /** Function : SetOnlinePlayerIcon
        Description : set pic in online player widget
        Calls : setGeometry(), show()
        Input : index of player in container
        Return : None
        Others : None **/
    void SetOnlinePlayerIcon(int i);

    /** Function : SetRankIcons
        Description : set pic in rank widget
        Calls : setGeometry(), show()
        Input : index of player in container
        Return : None
        Others : None **/
    void SetRankIcons(int i);

    /** Function : SetPakcageScrollArea
        Description : set package and storage pokemons in scroll area
        Calls : setToolTip(), setScaledSize()...
        Input : package or storage, pokemon's kind, pokemon's name, pokemon's other information, index in container
        Return : None
        Others : None **/
    void SetPackageScrollArea(QString symbol, QString kind, QString name, QString tip, int index);

    /** Function : ClearScrollArea
        Description : Clear scroll area and array when pokemon information widget is closed
        Calls : hide(), clear()...
        Input : package or storage
        Return : None
        Others : None **/
    void ClearScrollArea(QString packorsto);

    /** Function : SetMyInfo
        Description : set my information in my information widget
        Calls : append(), setToolTip()...
        Input : number of pokemon, rank, rate, other information
        Return : None
        Others : None **/
    void SetMyInfo(int pokenum, int rank, double rate, QString info);

    /** Function : openInChrome
        Description : link to github
        Calls : ShellExcuteA()
        Input : url
        Return : None
        Others : calls shell function to open link in default explorer **/
    void OpenInChrome(const QUrl& url);

    /** Function : ExchangePokemon
        Description : exchange poekmon in container
        Calls : h
        Input : None
        Return : None
        Others : None **/
    void ExchangePokemon(QString symbol, bool success);

    /** Function : SwitchClear
        Description : re-initialize memory when switch to other widget
        Calls : RecvAndSendOnlinePlayer()
        Input : None
        Return : None
        Others : None **/
    void SwitchClear();

signals:
    /** Signal : SwitchToHunt
        Description : switch to hunt widget
        Calls : hide()
        Input : None
        Return : None
        Others : None **/
    void SwitchToHunt();

    /** Signal : SwitchToBattle
        Description : switch to battle widget
        Calls : hide()
        Input : boot's nature player chosed, boot's standard player chosed
        Return : None
        Others : None **/
    void SwitchToBattle(QString nature, QString standard);

    /** Signal : SetOnlinePlayerIconSignal
        Description : set online players' pics
        Calls :
        Input : index of player in online player array
        Return : None
        Others : None **/
    void SetOnlinePlayerIconSignal(int i);

    /** Signal : SetRankIconSignal
        Description : set rank players' pics
        Calls :
        Input : index of player in rank array
        Return : None
        Others : None **/
    void SetRankIconSignal(int i);

    /** Signal : SetPackageScrollAreaSignal
        Description : set package scroll area
        Calls :
        Input : package or storage, pokemon's kind, pokemon's name,
                pokemon's other information, index of pokemon in container
        Return : None
        Others : None **/
    void SetPackegeScrollAreaSignal(QString symbol, QString kind, QString name, QString tip, int index);

    /** Signal : SetMyInfoSignal
        Description : set my information
        Calls :
        Input : number of pokemon, rank, rate, other information
        Return : None
        Others : None **/
    void SetMyInfoSignal(int pokenum, int rank, double rate, QString info);

    /** Signal : ClearScrollAreaSignal
        Description : clear scroll area after container widget is closed
        Calls :
        Input : package or storage
        Return : None
        Others : None **/
    void ClearScrollAreaSignal(QString packorsto);

    /** Signal : PlayerPokeClicked
        Description : online player chosen to look through
        Calls :
        Input : index of player in online player array
        Return : None
        Others : None **/
    void PlayerPokeClicked(int i);

    /** Signal : PlayerThumbClicked
        Description : online player chosen to be give a thumb
        Calls :
        Input : index of player in online player array
        Return : None
        Others : None **/
    void PlayerThumbClicked(int i);

    /** Signal : RankPokeClicked
        Description : rank player chosen to look through
        Calls :
        Input : index of player in rank player array
        Return : None
        Others : None **/
    void RankPokeClicked(int i);

    /** Signal : RankThumbClicked
        Description : rank player chosen to be give a thumb
        Calls :
        Input : index of player in rank player array
        Return : None
        Others : None **/
    void RankThumbClicked(int i);

    /** Signal : ExButtonClicked
        Description : which pokemon and in which container is chosen to exchange
        Calls :
        Input : index of pokemon in corresponding container
        Return : None
        Others : None **/
    void ExButtonClicked(QObject* obj, int i);

    /** Signal : ExchangePokeSignal
        Description : exchange pokemon
        Calls :
        Input : package or storage, exchange success or not
        Return : None
        Others : None **/
    void ExchangePokeSignal(QString symbol, bool success);
};

#endif // !MAINPAGE_H
