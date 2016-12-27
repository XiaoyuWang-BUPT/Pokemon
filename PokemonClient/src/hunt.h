/** Copyright : bupt
    Author : RainING1947
    Date : 2016/12/23
    Description : Hunt module, player can catch pokemon
**/
#ifndef HUNT_H
#define HUNT_H

#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include <QMovie>
#include <QRect>
#include <QString>
#include <QPoint>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QRect>
#include <QCursor>
#include <QMessageBox>
#include <string>
#include <sstream>
#include "lib/json.hpp"
#include "socketClient.h"

//simulate 3D illusion
#define ZMin 400
#define ZMax 600

using json = nlohmann::json;

//index of embryo pokemon in kindOfString array
const int EmbryoPokemon[] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33};
//kind of all pokemon
const std::string kindOfString[] = {"Charmander", "Charmeleon", "Charizard",
                         "Torchic", "Combusken", "Blaziken",
                         "Chimchar", "Monferno", "Infernape",
                        "Squirtle", "Wartortle", "Blastoise",
                         "Mudkip", "Marshtomp", "Swampert",
                         "Piplup", "Prinplup", "Empoleon",
                        "Bulbasaur", "Ivysaur", "Venusaur",
                         "Treecko", "Grovyle", "Sceptile",
                         "Turtwig", "Grotle", "Torterra",
                        "Pichu", "Pikachu", "Raichu",
                         "Shinx", "Luxio", "Luxray",
                         "Mareep", "Flaaffy", "Ampharos"};

namespace Ui {
class Hunt;
}

class Hunt : public QWidget
{
    Q_OBJECT

public:
    SocketClient* socketClient;
    /** Function : Hunt(SocketClient*, QWidget*)
        Description : Construct function
        Calls : None
        Input : A client socket for communicating with server
        Return : None
        Others : None **/
    explicit Hunt(SocketClient* sc, QWidget *parent = 0);

    /** Function : Hunt(QWidget*)
        Description : Default Construct function
        Calls : None
        Input : None
        Return : None
        Others : None **/
    explicit Hunt(QWidget *parent = 0);
    ~Hunt();

private:
    /** Function : InitUI
        Description : Initialize UI items, especially wild pokemon and pokeball
        Calls : setWindowtitle(), SetPokeGif(), SetPokeBallPng()
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

    /** Function : GetPokeKind
        Description : get wild pokemon's kind in std::string
        Calls : None
        Input : None
        Return : None
        Others : None **/
    void GetPokeKind();

    /** Function : GetZ
        Description : get distance in Z-Direction
        Calls : GetRanIndex
        Input : None
        Return : distance in Z-Direction
        Others : None **/
    int GetZ();

    /** Function : GetMovie
        Description : get wild pokemon's gif resource according to its kind
        Calls : toLower()...
        Input : None
        Return : wild pokemon's gif resource
        Others : None **/
    QMovie* GetMovie();

    /** Function : SetPokeGif
        Description : Set wild pokemon's gif resource
        Calls : GetMovie(), GetZ()...
        Input : None
        Return : None
        Others : None **/
    void SetPokeGif();

    /** Function : SetPokeBallPng
        Description : Set pokeball png resource
        Calls : setStyleSheet()
        Input : None
        Return : None
        Others : None **/
    void SetPokeBallPng();

    /** Function : GetSendStr
        Description :
        Calls : QObject::connect() function
        Input : None
        Return : None
        Others : None **/
    Ui::Hunt *ui;

    //pokeball is pressed or not
    bool ballPressed = false;

    //pokemon is caught or not
    bool caught = false;

    //wild pokemon's gif resource
    QMovie* movie;

    //wild pokemon's kind
    std::string kindStr;

    //newly-caught pokemon's name player gave
    std::string name;

private slots:
    /** Function : receiveSwitch
        Description : receive siwtch signal
        Calls : show()
        Input : None
        Return : None
        Others : None **/
    void receiveSwitch();

    /** Function : backClicked
        Description : button back to main page is clicked
        Calls : None
        Input : None
        Return : None
        Others : None **/
    void backClicked();

    /** Function : showWord
        Description : Display wild pokemon's word
        Calls : None
        Input : wild pokemon is caught or not
        Return : None
        Others : None **/
    void showWord(bool caught);

    /** Function : catchPokemon
        Description : send hunt result to server
        Calls : None
        Input : None
        Return : None
        Others : None **/
    void catchPokemon();

    /** Function : eventFilter
        Description : overwrite eventFilter to achieve throwing pokeball animation
        Calls : None
        Input : watched object, event
        Return : None
        Others : None **/
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    /** Signal : switchToMainPage
        Description : swicth to mainpage
        Calls : None
        Input : None
        Return : None
        Others : None **/
    void switchToMainPage();
};

#endif // HUNT_H
