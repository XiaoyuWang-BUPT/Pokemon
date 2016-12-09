﻿#ifndef MAINPAGE_H
#define MAINPAGE_H

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
#include "lib/json.hpp"
#include "socketClient.h"

using json = nlohmann::json;
#define MAXSIZE_PLAYER 10 //maximum of player can be shown
#define TABLE_ROW 3
#define TABLE_COL 6
#define MAXSIZE_POKEMON 15 //maximum of pokemon can be shown in one QTableWidget

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
    QTableWidgetItem* pokeTableItem[MAXSIZE_POKEMON];
    QLabel* headLabel[MAXSIZE_PLAYER];
    QPushButton* playerPokeButton[MAXSIZE_PLAYER];
    QPushButton* thumbButton[MAXSIZE_PLAYER];
    QPushButton* rankPokeButton[MAXSIZE_PLAYER];
    QPushButton* rankThumbButton[MAXSIZE_PLAYER];
    void LoadOnlinePlayer(json &recvJ);
    std::string playerNames[MAXSIZE_PLAYER];
    std::string rankPlayerNames[MAXSIZE_PLAYER];
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
    Q_INVOKABLE bool getRecvStr(QString str);
    bool eventFilter(QObject *watched, QEvent *event);
    void setOnlinePlayerIcon(int i);
    void setRankIcons(int i);
    void OpenInChrome(const QUrl& url);

signals:
    void switchToHunt();
    void setOnlinePlayerIconSignal(int i);
    void setRankIconSignal(int i);
    void playerPokeClicked(int i);
    void playerThumbClicked(int i);
    void rankPokeClicked(int i);
    void rankThumbClicked(int i);
};

#endif // !MAINPAGE_H
