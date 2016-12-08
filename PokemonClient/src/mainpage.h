#ifndef MAINPAGE_H
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
#include <QFont>
#include <QLabel>
#include <QPushButton>
#include "lib/json.hpp"
#include "socketClient.h"

using json = nlohmann::json;
#define MAXSIZE_PLAYER 10//maximum of player can be shown

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
    QLabel* headLabel[MAXSIZE_PLAYER];
    QPushButton* playerPokeButton[MAXSIZE_PLAYER];
    QPushButton* thumbButton[MAXSIZE_PLAYER];
    void LoadOnlinePlayer(json &recvJ);
    std::string playerNames[MAXSIZE_PLAYER];
    void RecvAndSendOnlinePlayer(json j);

private slots:
    void receiveSwitch();
    void onOnlinePlayerClicked();
    void onOnlinePlayerReloadClicked();
    void onPlayerPokeClicked(int i);
    void onPlayerThumbClicked(int i);
    Q_INVOKABLE bool getRecvStr(QString str);
    bool eventFilter(QObject *watched, QEvent *event);
    void setOnlinePlayerIcon(int i);

signals:
    void switchToHunt();
    void setOnlinePlayerIconSignal(int i);
    void playerPokeClicked(int i);
    void playerThumbClicked(int i);
};

#endif // !MAINPAGE_H
