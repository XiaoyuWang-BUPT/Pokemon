#include "battle.h"
#include "ui_battle.h"

Battle::Battle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Battle)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->ui->backButton->installEventFilter(this);
}

Battle::Battle(SocketClient *sc, QWidget *parent) :
    Battle(parent)
{
    socketClient = sc;
}

Battle::~Battle()
{
    delete socketClient;
    delete ui;
}

void Battle::receiveSwitch(QString nature, QString standard)
{
    std::cout << nature.toStdString() << standard.toStdString();
    this->show();
}

bool Battle::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->ui->backButton)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            this->hide();
            emit switchToMainPage();
        }
    }
    return QWidget::eventFilter(watched, event);
}
