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
#define MAXSIZE_PLAYER 10 //maximum of player can be shown
#define TABLE_ROW 3
#define TABLE_COL 6
#define MAXSIZE_POKEMON 15 //maximum of pokemon can be shown in one QTableWidget

const int PokeNumLevel[3] = {5, 10, 20};
const int RankLevel[3] = {10, 50, 100};
const double RateLevel[3] = {20, 50, 80};

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    SocketClient* socketClient;
    explicit MainPage(SocketClient* sc, QWidget *parent = 0);
    explicit MainPage(QWidget *parent = 0);
    ~MainPage();

private:
    Ui::MainPage *ui;
    std::thread calledThread;
    std::string recvString = "";
    int delHLayIndex = -1;
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

    std::string playerNames[MAXSIZE_PLAYER];
    std::string rankPlayerNames[MAXSIZE_PLAYER];

    std::vector<std::string> packPokemon;
    std::vector<std::string> stoPokemon;

    void LoadOnlinePlayer(json &recvJ);
    void RecvAndSendOnlinePlayer(json j);

private slots:
    void receiveSwitch();
    void onOnlinePlayerClicked();
    void onOnlinePlayerReloadClicked();
    void onPlayerPokeClicked(int i);
    void onPlayerThumbClicked(int i);
    void onMyInfoClicked();
    void onRankClicked();
    void onRankPokeClicked(int i);
    void onRankThumbClicked(int i);
    void onPackageClicked();
    void onStorageClicked();
    void onUpDownClicked();
    void onExButtonClicked(QObject* obj, int i);
    Q_INVOKABLE bool getRecvStr(QString str);
    bool eventFilter(QObject *watched, QEvent *event);
    void setOnlinePlayerIcon(int i);
    void setRankIcons(int i);
    void setPackageScrollArea(QString symbol, QString kind, QString name, QString tip, int index);
    void clearScrollArea(QString packorsto);
    void setMyInfo(int pokenum, int rank, double rate, QString info);
    void OpenInChrome(const QUrl& url);
    void clearScrollLayout(QString symbol, bool success);
    void SwitchClear();

signals:
    void switchToHunt();
    void SwitchToBattle(QString nature, QString standard);
    void setOnlinePlayerIconSignal(int i);
    void setRankIconSignal(int i);
    void setPackegeScrollAreaSignal(QString symbol, QString kind, QString name, QString tip, int index);
    void setMyInfoSignal(int pokenum, int rank, double rate, QString info);
    void clearScrollAreaSignal(QString packorsto);
    void playerPokeClicked(int i);
    void playerThumbClicked(int i);
    void rankPokeClicked(int i);
    void rankThumbClicked(int i);
    void exButtonClicked(QObject* obj, int i);
    void clearScrollLayoutSignal(QString symbol, bool success);
};

#endif // !MAINPAGE_H
