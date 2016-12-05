#ifndef HUNT_H
#define HUNT_H

#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include <QMovie>
#include <QRect>
#include <QString>
#include <string>
#include "lib/json.hpp"
#include "helper.h"
#include "socketClient.h"

#define ZMin 20
#define ZMax 30

const std::string kindOfString[] = {"Charamander", "Charmeleon", "Charizard",
                         "Torchic", "Combusken", "Blaziken",
                         "Chimchar", "Monferno", "Infernape",
                        "Squirtle", "Watortle", "Blastoise",
                         "Mudkip", "Marshtomp", "Swampert",
                         "Piplup", "Prinplup", "Empoleon",
                        "Bulbasaur", "Ivysaur", "Venusaur",
                         "Treecko", "Grovyle", "Sceptile",
                         "Turtwig", "Grotle", "Torterra",
                        "Pichu", "Pikachu", "Raichu",
                         "Shinx", "Luxio", "Luxray",
                         "Marrep", "Flaaffy", "Ampharos"};

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
    std::string GetPokeKind();
    int GetZ();
    QMovie* GetMovie();
    void SetPokeGif();

private slots:
    void receiveSwitch();
    void backClicked();

signals:
    void switchToMainPage();
};

#endif // HUNT_H
