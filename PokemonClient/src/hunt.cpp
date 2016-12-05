#define _CRT_RAND_S
#include <stdlib.h>
#include "hunt.h"
#include "ui_hunt.h"

Hunt::Hunt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Hunt)
{
    ui->setupUi(this);
    this->setWindowTitle("pokemon");
    QIcon LOGO (":/logo");
    this->setWindowIcon(LOGO);
    this->ui->backButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->widget->setStyleSheet("#widget{border-image: url(:/forest2);}");

    SetPokeGif();

    QObject::connect(this->ui->backButton, SIGNAL(clicked(bool)), this, SLOT(backClicked()));
}

Hunt::Hunt(SocketClient *sc, QWidget *parent)
    : Hunt(parent)
{
    socketClient = sc;    
}

Hunt::~Hunt()
{
    delete socketClient;
    delete ui;
}

void Hunt::receiveSwitch()
{
    this->show();
}

void Hunt::backClicked()
{
    this->hide();
    emit switchToMainPage();
}

unsigned int Random(int max) {
    errno_t err;
        unsigned int number;
        err = rand_s(&number);
        if(err != 0)
        {
          return 0;
        }
        return (unsigned int)((double)number / ((double)UINT_MAX + 1) * double(max)) + 1;
}

int GetRanIndex(int MIN, int MAX) {
    return MIN + Random(MAX - MIN);
}

std::string Hunt::GetPokeKind()
{
    //return kindOfString[GetRanIndex(0, 35)];
    return "Blastoise";
}

int Hunt::GetZ()
{
    return GetRanIndex(ZMin, ZMax);
}

QMovie* Hunt::GetMovie()
{
    //QMovie* movie = new QMovie(":/blastoise.gif");
    QString kind = QString::fromStdString(GetPokeKind()).toLower();
    QString movieName = ":/" + kind + ".gif";
    QMovie* movie = new QMovie(movieName);
    return movie;
}

void Hunt::SetPokeGif()
{
    QMovie* movie = GetMovie();
    int w = 197 * ZMin / GetZ();
    int h = 192 * ZMin / GetZ();
    movie->setScaledSize(QSize(w, h));
    this->ui->pokeLabel->setMovie(movie);
    movie->start();
}
