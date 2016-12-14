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
#include "lib/json.hpp"

using json = nlohmann::json;

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
    std::vector<std::string> myPokemonName;
    std::vector<std::string> myPokemonKind;
    std::vector<int> myPokemonTHP;
    std::vector<std::string> enemyPokemonName;
    std::vector<std::string> enemyPokemonKind;
    std::vector<int> enemyPokemonTHP;
    int myPokeIndex = 0;
    int enemyPokeIndex = 0;
    std::string natureStd = "";
    std::string standardStd = "";

signals:
    void switchToMainPage();

    void battleNotQualified();
    void WinOrLoseSignal(bool win);
    void MyPokeAttackSignal(QString attway, int enemypokehp);
    void EnemyPokeAttackSignal(QString attway, int mypokehp);
    void MyPokeHurtSignal(int myhurthp);
    void EnemyPokeHurtSignal(int enemyhurthp);
    void MyPokeChangeSignal(int mypokeindex);
    void EnemyPokeChangeSignal(int enemypokeindex);

private slots:
    void receiveSwitch(QString nature, QString standard);
    void getRecvStr(QString recvStr);

    void onBattleNotQualified();
    void onBackClicked();
    void winOrLose(bool win);
    void onMyPokeAttack(QString attway, int enemypokehp);
    void onEnemyPokeAttack(QString attway, int mypokehp);
    void onMyPokeHurt(int myhurthp);
    void onEnemyPokeHurt(int enemyhurthp);
    void onMyPokeChange(int mypokeindex);
    void onEnemyPokeChange(int enemypokeindex);

    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // BATTLE_H
