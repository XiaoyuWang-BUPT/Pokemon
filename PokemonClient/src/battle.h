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
const int CAPACITY = 3;

namespace Ui {
class Battle;
}

class Battle : public QWidget
{
    Q_OBJECT

public:
    SocketClient *socketClient;
    explicit Battle(SocketClient *sc, QWidget *parent = 0);
    explicit Battle(QWidget *parent = 0);
    ~Battle();
    std::string getNatureStd() { return this->natureStd; }
    std::string getStandardStd() { return this->standardStd; }

private:
    Ui::Battle *ui;
    QHBoxLayout* endHLayout = new QHBoxLayout();
    QVBoxLayout* endVLayout[CAPACITY];
    QLabel* endPokeLabel[CAPACITY];
    QLabel* endInfoLabel[CAPACITY];
    std::vector<std::string> myPokemonName;
    std::vector<std::string> myPokemonKind;
    std::vector<std::string> myEvoKind;
    std::vector<int> myPokemonTHP;
    std::vector<std::string> enemyPokemonName;
    std::vector<std::string> enemyPokemonKind;
    std::vector<int> enemyPokemonTHP;
    int myPokeIndex = 0;
    int enemyPokeIndex = 0;
    int myPokeCurrentHP = 0;
    int enemyPokeCurrentHP = 0;
    std::string natureStd = "";
    std::string standardStd = "";

signals:
    void switchToMainPage();

    void battleNotQualified();
    void WinOrLoseSignal(bool win, int expGot);
    void MyPokeAttackSignal(QString attway, QString myNature);
    void EnemyPokeAttackSignal(QString attway, QString enemyNature);
    void MyPokeBeAttacked(int mypokehp);
    void EnemyPokeBeAttacked(int enemypokehp);
    void MyPokeHurtSignal(int myhurthp, QString enemyNature);
    void EnemyPokeHurtSignal(int enemyhurthp, QString myNature);
    void ClearHurtSignal();
    void ClearHPDecSignal();
    void ClearSpecialAttSignal();
    void MyPokeChangeSignal(int mypokeindex);
    void EnemyPokeChangeSignal(int enemypokeindex);

private slots:
    void receiveSwitch(QString nature, QString standard);
    void getRecvStr(QString recvStr);

    void onBattleNotQualified();
    void winOrLose(bool win, int expGot);
    void onMyPokeAttack(QString attway, QString myNature);
    void onEnemyPokeAttack(QString attway, QString enemyNature);
    void onMyPokeBeAttacked(int mypokehp);
    void onEnemyPokeBeAttacked(int enemypokehp);
    void onMyPokeHurt(int myhurthp, QString enemyNature);
    void onEnemyPokeHurt(int enemyhurthp, QString myNature);
    void ClearHurt();
    void ClearHPDec();
    void ClearSpecialAtt();
    void onMyPokeChange(int mypokeindex);
    void onEnemyPokeChange(int enemypokeindex);
    void clearBattle();
};

#endif // BATTLE_H
