#ifndef BATTLE_H
#define BATTLE_H

#include "socketClient.h"
#include <QWidget>
#include <QString>
#include <QEvent>
#include <QPixmap>
#include "lib/json.hpp"

using json = nlohmann::json;

namespace Ui {
class Battle;
}

class Battle : public QWidget
{
    Q_OBJECT

public:
    explicit Battle(SocketClient *sc, QWidget *parent = 0);
    explicit Battle(QWidget *parent = 0);
    ~Battle();

private:
    Ui::Battle *ui;
    SocketClient *socketClient;
    void SendAndRecvFunc(json j);
    std::vector<std::string> myPokemonName;
    std::vector<std::string> myPokemonKind;
    std::vector<int> myPokemonTHP;
    std::vector<std::string> enemyPokemonName;
    std::vector<std::string> enemyPokemonKind;
    std::vector<int> enemyPokemonTHP;

signals:
    void switchToMainPage();

    void MyPokeAttackSignal(QString attway, int enemypokehp);
    void EnemyPokeAttackSignal(QString attway, int mypokehp);
    void MyPokeHurtSignal(int myhurthp);
    void EnemyPokeHurtSignal(int enemyhurthp);
    void MyPokeChangeSignal(int mypokeindex);
    void EnemyPokeChangeSignal(int enemypokeindex);

private slots:
    void receiveSwitch(QString nature, QString standard);
    void getRecvStr(QString recvStr);

    void onMyPokeAttack(QString attway, int enemypokehp);
    void onEnemyPokeAttack(QString attway, int mypokehp);
    void onMyPokeHurt(int myhurthp);
    void onEnemyPokeHurt(int enemyhurthp);
    void onMyPokeChange(int mypokeindex);
    void onEnemyPokeChange(int enemypokeindex);

    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // BATTLE_H
