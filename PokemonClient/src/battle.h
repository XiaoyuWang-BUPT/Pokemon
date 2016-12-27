/** Copyright : bupt
    Author : RainING1947
    Date : 2016/12/23
    Description : Battle module
**/

#ifndef BATTLE_H
#define BATTLE_H

#include "socketClient.h"
#include <QWidget>
#include <QString>
#include <QEvent>
#include <QPixmap>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QRect>
#include <QSize>
#include <QSizeF>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "lib/json.hpp"

using json = nlohmann::json;

//Capacity of package, a player can only carry 3 pokemon on at most once
const int CAPACITY = 3;

namespace Ui {
class Battle;
}

class Battle : public QWidget
{
    Q_OBJECT

public:
    SocketClient *socketClient;
    /** Function : Battle(SocketClient*, QWidget *parent)
        Description : Construct function
        Calls : None
        Input : A client socket for communicating with server
        Return : None
        Others : None **/
    explicit Battle(SocketClient *sc, QWidget *parent = 0);

    /** Function : Battle(QWidget*)
        Description : Default Construct function
        Calls : None
        Input : None
        Return : None
        Others : None **/
    explicit Battle(QWidget *parent = 0);
    ~Battle();

    /** Function : getNuatureStd
        Description : return boots' nature that player chosed
        Calls : None
        Input : None
        Return : nature of std::string
        Others : None **/
    std::string getNatureStd() { return this->natureStd; }

    /** Function : getStandardStd
        Description : return boots' standard that player chosed
        Calls : None
        Input : None
        Return : standard of std::string
        Others : None **/
    std::string getStandardStd() { return this->standardStd; }

private:
    /** Function : InitConnect
        Description : Initialize connections between signals and slots
        Calls : QObject::connect() function
        Input : None
        Return : None
        Others : None **/
    void InitConnect();

    /** Function : InitUI
        Description : Initialize UI items
        Calls : setWindowtitle() function
        Input : None
        Return : None
        Others : None **/
    void InitUI();
    Ui::Battle *ui;

    //Result of battle's horizonal layout containing at most 3 vertical layouts
    QHBoxLayout* endHLayout = new QHBoxLayout();

    //vertical layout displaying each fighting pokemon's information
    QVBoxLayout* endVLayout[CAPACITY];

    //label containing each fighting pokemon's pic
    QLabel* endPokeLabel[CAPACITY];

    //label containing each pokemon's note
    QLabel* endInfoLabel[CAPACITY];

    //my fighting pokemons' name
    std::vector<std::string> myPokemonName;

    //my pokemons' kind (eg. "charmander", "pikachu")
    std::vector<std::string> myPokemonKind;

    //my pokemons' kind after battle (eg. "charmeleon", "raichu")
    std::vector<std::string> myEvoKind;

    //my pokemons' total HP
    std::vector<int> myPokemonTHP;

    //boots' name
    std::vector<std::string> enemyPokemonName;

    //boots' kind (eg. "squirtle", "bulbasaur")
    std::vector<std::string> enemyPokemonKind;

    //boots' total HP
    std::vector<int> enemyPokemonTHP;

    //my fighting pokemon's index in my pokemon vector
    int myPokeIndex = 0;

    //boot fighting pokemon index in enemy pokemon vector
    int enemyPokeIndex = 0;

    //my fighting pokemon's HP right now
    int myPokeCurrentHP = 0;

    //boot fighting pokemon's HP right now
    int enemyPokeCurrentHP = 0;

    //boots' nature player chosed to fight
    std::string natureStd = "";

    //boots' standard player chosed to fight
    std::string standardStd = "";

signals:
    /** Signal : SwitchToMainPage
        Description : switch to mainpage
        Others : None **/
    void switchToMainPage();

    /** Signal : battleNotQualified
        Description : package is empty, not qualified to battle
        Others : None **/
    void battleNotQualified();

    /** Signal : WinOrLoseSignal
        Description : display result after battle
        Others : None **/
    void WinOrLoseSignal(bool win, int expGot);

    /** Signal : MyAttackSignal
        Description : my pokemon's turn to attack
        Others : None **/
    void MyPokeAttackSignal(QString attway, QString myNature);

    /** Signal : EnemyAttackSignal
        Description : boot pokemon's turn to attack
        Others : None **/
    void EnemyPokeAttackSignal(QString attway, QString enemyNature);

    /** Signal : MyPokeBeAttacked
        Description : my pokemon is attacked
        Others : None **/
    void MyPokeBeAttacked(int mypokehp);

    /** Signal : EnemyPokeBeAttacked
        Description : boot pokemon is attacked
        Others : None **/
    void EnemyPokeBeAttacked(int enemypokehp);

    /** Signal : MyPokeHurtSignal
        Description : my pokemon is hurt by uhhealthy state before attacking
        Others : None **/
    void MyPokeHurtSignal(int myhurthp, QString enemyNature);

    /** Signal : EnemyPokeHurtSignal
        Description : boot pokemon is hurt by uhhealthy state before attacking
        Others : None **/
    void EnemyPokeHurtSignal(int enemyhurthp, QString myNature);

    /** Signal : ClearHurtSignal
        Description : Clear hurt pic after some time
        Others : None **/
    void ClearHurtSignal();

    /** Signal : ClearHPDecSignal
        Description : Clear HP-decreasing animation after some time
        Others : None **/
    void ClearHPDecSignal();

    /** Signal : ClearSpecialAttSignal
        Description : Clear special attack pic after some time
        Others : None **/
    void ClearSpecialAttSignal();

    /** Signal : MyPokeChangeSignal
        Description : time to change pokemon because the former is dead
        Others : None **/
    void MyPokeChangeSignal(int mypokeindex);

    /** Signal :EnemMyPokeChangeSignal
        Description : time to change pokemon because the former is dead
        Others : None **/
    void EnemyPokeChangeSignal(int enemypokeindex);

private slots:
    /** Function : receiveSwitch
        Description : receive switch siganl
        Calls : hide(), resize(), clear()...
        Input : boot's nature player chosed, boot's standard player chosed
        Return : None
        Others : None **/
    void receiveSwitch(QString nature, QString standard);

    /** Function : getRecvStr
        Description : Handle process after recv from server
        Calls : None
        Input : received string
        Return : None
        Others : None **/
    void getRecvStr(QString recvStr);

    /** Function : onBattleNotQualified
        Description : package is empty, not qualified to battle
        Calls : hide()
        Input : None
        Return : None
        Others : None **/
    void onBattleNotQualified();

    /** Function : WinOrLose
        Description : display result of battle
        Calls : clear(), show(), setStyleSheet()...
        Input : the result of battle, experience point pokemons've got
        Return : None
        Others : None **/
    void WinOrLose(bool win, int expGot);

    /** Function : onMyPokeAttack
        Description : my pokemon's turn to attack
        Calls : setPixmap()...
        Input : attack way (eg. normal attack, special attack), my fighting pokemon's nature
        Return : None
        Others : using QPropertyAnimation **/
    void onMyPokeAttack(QString attway, QString myNature);

    /** Function : onEnemyPokeAttack
        Description : boot pokemon's turn to attack
        Calls : setPixmap()...
        Input : attack way (eg. normal attack, special attack), boot fighting pokemon's nature
        Return : None
        Others : using QPropertyAnimation **/
    void onEnemyPokeAttack(QString attway, QString enemyNature);

    /** Function : onMyPokeBeAttacked
        Description : my pokemon is attacked
        Calls : show(), setText(), width()...
        Input : my fighting pokemon's HP after be attacked
        Return : None
        Others : using QPropertyAnimation **/
    void onMyPokeBeAttacked(int mypokehp);

    /** Function : onEnemyPokeBeAttacked
        Description : boot pokemon is attacked
        Calls : show(), setText(), width()...
        Input : boot fighting pokemon's HP after be attacked
        Return : None
        Others : using QPropertyAnimation **/
    void onEnemyPokeBeAttacked(int enemypokehp);

    /** Function : onMyPokeHurt
        Description : my pokemon is hurt before attack
        Calls : show(), setText(), width()...
        Input :my fighting pokemon's HP after be hurt
        Return : None
        Others : using QPropertyAnimation **/
    void onMyPokeHurt(int myhurthp, QString enemyNature);

    /** Function : onEnemyPokeBeHurt
        Description : boot pokemon is hurt before attack
        Calls : show(), setText(), width()...
        Input : boot fighting pokemon's HP after be hurt
        Return : None
        Others : using QPropertyAnimation **/
    void onEnemyPokeHurt(int enemyhurthp, QString myNature);

    /** Function : ClearHurt
        Description : Clear hurt pic after some time
        Calls : hide()
        Input : None
        Return : None
        Others : None **/
    void ClearHurt();

    /** Function : ClearHPDec
        Description : Clear HP-decreasing pic after some time
        Calls : hide()
        Input : None
        Return : None
        Others : None **/
    void ClearHPDec();

    /** Function : ClearSpecialAtt
        Description : Clear special attack pic after some time
        Calls : hide()
        Input : None
        Return : None
        Others : None **/
    void ClearSpecialAtt();

    /** Function : onMyPokeChange
        Description : changing my fighting pokemon when the former is dead
        Calls : resize(), setStyleSheet()...
        Input : index of my pokemon which is to fight next
        Return : None
        Others : None **/
    void onMyPokeChange(int mypokeindex);

    /** Function : onEnemyPokeChange
        Description : changing boot fighting pokemon when the former is dead
        Calls : resize(), setStyleSheet()...
        Input : index of boot pokemon which is to fight next
        Return : None
        Others : None **/
    void onEnemyPokeChange(int enemypokeindex);

    /** Function : ClearBattle
        Description : Clear all items after battle finished
        Calls : hide()...
        Input : None
        Return : None
        Others : None **/
    void ClearBattle();
};

#endif // BATTLE_H
