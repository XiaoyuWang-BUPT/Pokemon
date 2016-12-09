#define _CRT_RAND_S
#include <stdlib.h>
#include "hunt.h"
#include "ui_hunt.h"

Hunt::Hunt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Hunt)
{
    ui->setupUi(this);
    this->ui->renameWidget->hide();
    this->ui->decorationL->hide();
    this->ui->decorationR->hide();
    this->setWindowTitle("pokemon");
    QIcon LOGO (":/logo");
    this->setWindowIcon(LOGO);
    this->ui->backButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->pokeBallLabel->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->widget->setStyleSheet("#widget{border-image: url(:/forest2);}");

    this->ui->pokeBallLabel->installEventFilter(this);

    SetPokeGif();
    SetPokeBallPng();

    QObject::connect(this->ui->confirmButton, SIGNAL(clicked(bool)), this, SLOT(catchPokemon()));
    QObject::connect(this->ui->backButton, SIGNAL(clicked(bool)), this, SLOT(backClicked()));
}

Hunt::Hunt(SocketClient *sc, QWidget *parent) :
    Hunt(parent)
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
    this->ui->pokeBallLabel->setGeometry(370, 430, 90, 90);
    SetPokeGif();
    this->ui->renameWidget->hide();
    this->ui->decorationL->hide();
    this->ui->decorationR->hide();
    this->show();
}

DWORD WINAPI SendThreadFuncHunt(LPVOID lParam, LPVOID sParam);

void Hunt::catchPokemon()
{
    if (caught)
    {
        if (this->ui->nameLineEdit->text().isEmpty())
        {
            QString info = QString::fromStdString(kindStr) + " wants a name";
            QMessageBox::information(this, "Info", info);
        }
        json j;
        j["symbol"] = "hunt";
        j["owner"] = socketClient->getPlayerName();
        j["kind"] = kindStr;
        j["name"] = this->ui->nameLineEdit->text().toStdString();
        name = this->ui->nameLineEdit->text().toStdString();
        j["end"] = "end";

        std::string sendStr = j.dump();
        std::thread sendThread = std::thread(SendThreadFuncHunt, socketClient, &sendStr);
        sendThread.join();
        socketClient->Receive();
        socketClient->ClearRecvBuf();
        this->ui->backButton->setEnabled(true);
        this->ui->nameLineEdit->clear();
        emit this->ui->backButton->clicked();
    }
    return;
}

void Hunt::backClicked()
{
    if (caught)
    {
        if (name == "")
        {
            QString info = QString::fromStdString(kindStr) + " wants a name";
            QMessageBox::information(this, "Info", info);
        }
        else
        {
            name = "";
            kindStr = "";
            this->hide();
            emit switchToMainPage();
        }
        caught = false;
    }
    else
    {
        this->hide();
        emit switchToMainPage();
    }
    return;
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

void Hunt::GetPokeKind()
{
    //return kindOfString[GetRanIndex(0, 35)];
    int x = GetRanIndex(1, 4);
    switch (x)
    {
    case 1:
        kindStr = "Pichu";
        break;
    case 2:
        kindStr = "Charmander";
        break;
    case 3:
        kindStr = "Bulbasaur";
        break;
    case 4:
        kindStr = "Squirtle";
        break;
    default:
        break;
    }
    return;
}

int Hunt::GetZ()
{
    return GetRanIndex(ZMin, ZMax);
}

QMovie* Hunt::GetMovie()
{
    GetPokeKind();
    QString kind = QString::fromStdString(kindStr).toLower();
    QString movieName = ":/" + kind + ".gif";
    QMovie* movie = new QMovie(movieName);
    return movie;
}

void Hunt::SetPokeGif()
{
    this->ui->wordLabel->setStyleSheet("#wordLabel{border-image: url(:/word);}");
    this->ui->pokeLabel->setGeometry(330, 130, 200, 200);
    movie = GetMovie();
    int w = 197 * ZMin / GetZ();
    int h = 192 * ZMin / GetZ();
    movie->setScaledSize(QSize(w, h));
    this->ui->pokeLabel->setMovie(movie);
    movie->start();
    return;
}

void Hunt::SetPokeBallPng()
{
    this->ui->pokeBallLabel->setStyleSheet("#pokeBallLabel{border-image: url(:/pokemonball);}");
    return;
}

bool Hunt::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->ui->pokeBallLabel)
    {
        if (event->type() == QEvent::HoverEnter)
        {
            this->ui->pokeBallLabel->setCursor(QCursor(Qt::PointingHandCursor));
        }
        if (event->type() == QEvent::MouseButtonPress)
        {
            if (!ballPressed)
            {
                this->ui->wordLabel->hide();
                this->ui->pokeBallLabel->setCursor(QCursor(Qt::PointingHandCursor));
                ballPressed = true;
            }
        }
        if (event->type() == QEvent::MouseButtonRelease)
        {
            if (ballPressed)
            {
                this->ui->pokeBallLabel->setCursor(QCursor(Qt::ArrowCursor));
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                QPointF endP = mouseEvent->windowPos();

                int yInc = 430 - endP.y() - 50;
                int newY = endP.y() + yInc / 4;
                int z = 2 * yInc;
                if (z > ZMin && z < ZMax)
                {
                    caught = true;
                    std::cout << "caught" << std::endl;
                }
                else
                {
                    std::cout << "failed" << std::endl;
                }

                QPropertyAnimation* animation = new QPropertyAnimation(this->ui->pokeBallLabel, "geometry");
                animation->setStartValue(QRect(370, 430, 90, 90));
                animation->setEasingCurve(QEasingCurve::OutCirc);
                if (caught)
                {
                    animation->setKeyValueAt(0.4, QRect(370, ((int)endP.y() - 45), 80, 80));
                    animation->setEndValue(QRect(370, newY, 70, 70));
                    animation->setDuration(1500);
                    QPropertyAnimation* animation2 = new QPropertyAnimation(this->ui->pokeLabel, "geometry");
                    animation2->setStartValue(QRect(330, 130, 200, 200));
                    animation2->setEndValue(QRect(370, newY, 0, 0));
                    animation2->setEasingCurve(QEasingCurve::InExpo);
                    animation2->setDuration(750);
                    animation2->start(QAbstractAnimation::DeleteWhenStopped);
                    movie->stop();
                }
                else
                {
                    animation->setKeyValueAt(0.3, QRect(370, ((int)endP.y() - 45), 80, 80));
                    animation->setKeyValueAt(0.75, QRect(370, newY, 70, 70));
                    animation->setEndValue(QRect(370, newY, 0, 0));
                    animation->setDuration(2000);
                }
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                showWord(caught);
                if (caught)
                {
                    this->ui->renameWidget->show();
                    this->ui->decorationL->show();
                    this->ui->decorationR->show();
                }
                ballPressed = false;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void Hunt::showWord(bool caught)
{
    QString sheet;
    int i = GetRanIndex(1, 3);
    std::string indexStr;
    std::stringstream stream;
    stream << i;
    stream >> indexStr;
    if (caught)
    {
        sheet = "#wordLabel{border-image: url(:/catch" + QString::fromStdString(indexStr) + ");}";
    }
    else
    {
        sheet = "#wordLabel{border-image: url(:/fail" + QString::fromStdString(indexStr) + ");}";
    }
    this->ui->wordLabel->setStyleSheet(sheet);
    this->ui->wordLabel->show();
    return;
}

DWORD WINAPI SendThreadFuncHunt(LPVOID lParam, LPVOID sParam)
{
    std::string *sendStr = (std::string*)sParam;
    SocketClient *socketClient = (SocketClient*)lParam;
    int iResult;
    SOCKET ConnectSocket = socketClient->getConnectSocket();
    size_t len = (*sendStr).length();
    char *sendbuf = new char[len];
    strcpy(sendbuf, (*sendStr).data());
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR)
    {
        std::cout << "Send failed with error " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        system("pause");
    }
    return 0;
}
