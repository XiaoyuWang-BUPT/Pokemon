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

#define ZMin 400
#define ZMax 600

using json = nlohmann::json;

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
    explicit Hunt(QWidget *parent = 0);
    explicit Hunt(SocketClient* sc, QWidget *parent = 0);
    ~Hunt();

private:
    Ui::Hunt *ui;
    void GetPokeKind();
    int GetZ();
    QMovie* GetMovie();
    void SetPokeGif();
    void SetPokeBallPng();
    std::string GetSendStr();
    bool ballPressed = false;
    bool caught = false;
    QMovie* movie;
    std::string kindStr;
    std::string name;

private slots:
    void receiveSwitch();
    void backClicked();
    void showWord(bool caught);
    void catchPokemon();
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void switchToMainPage();
};

#endif // HUNT_H
