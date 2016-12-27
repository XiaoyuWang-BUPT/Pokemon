#define _CRT_RAND_S
#include <stdlib.h>
#include "hunt.h"
#include "ui_hunt.h"

Hunt::Hunt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Hunt)
{
    ui->setupUi(this);
    this->InitUI();
    this->InitConnect();
}

void Hunt::InitUI()
{
    setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("pokemon");
    QIcon LOGO (":/logo");
    this->setWindowIcon(LOGO);

    //hide result widget when load this page
    this->ui->renameWidget->hide();
    this->ui->decorationL->hide();
    this->ui->decorationR->hide();
    this->ui->backButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->pokeBallLabel->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->widget->setStyleSheet("#widget{border-image: url(:/forest2);}");
    this->ui->pokeBallLabel->installEventFilter(this);

    SetPokeGif();
    SetPokeBallPng();
}

void Hunt::InitConnect()
{
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

        //notice if player did not name pokemon
        if (this->ui->nameLineEdit->text().isEmpty())
        {
            QString info = QString::fromStdString(kindStr) + " wants a name";
            QMessageBox::information(this, "Info", info);
        }

        //get the name of pokemon and send to server
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

        //after reported to server, return to main page
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
        //notice if pokemon is caught and did not get a name
        if (name == "")
        {
            QString info = QString::fromStdString(kindStr) + " wants a name";
            QMessageBox::information(this, "Info", info);
        }

        //pokemon is caught and had got a name, return to main page
        else
        {
            name = "";
            kindStr = "";
            this->hide();
            emit switchToMainPage();
        }
        caught = false;
    }

    //return to main page if pokemon is not caught
    else
    {
        this->hide();
        emit switchToMainPage();
    }
    return;
}

//Hight-precision random function
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

//get a random number between min and max
int GetRanIndex(int MIN, int MAX) {
    return MIN + Random(MAX - MIN);
}

void Hunt::GetPokeKind()
{
    //get a embryo pokemon kind in random
    kindStr = kindOfString[EmbryoPokemon[GetRanIndex(1, 12) - 1]];
}

int Hunt::GetZ()
{
    //get a distance in Z-direction in random
    return GetRanIndex(ZMin, ZMax);
}

QMovie* Hunt::GetMovie()
{
    //get wild pokemon gif resource according to the kind which is got previously
    GetPokeKind();
    QString kind = QString::fromStdString(kindStr).toLower();
    QString movieName = ":/" + kind + ".gif";
    QMovie* movie = new QMovie(movieName);
    return movie;
}

void Hunt::SetPokeGif()
{
    //set the geometry of the pokemon gif resource according to distance in Z-direction
    this->ui->wordLabel->setStyleSheet("#wordLabel{border-image: url(:/word);}");
    this->ui->pokeLabel->setGeometry(330, 130, 200, 200);
    this->ui->pokeLabel->setAlignment(Qt::AlignCenter);
    movie = GetMovie();
    int rateZ = GetZ();
    int w = 75 * ZMin / rateZ;
    int h = 75 * ZMin / rateZ;

    this->ui->pokeLabel->setGeometry(390, 200, w, h);
    this->ui->pokeLabel->setScaledContents(true);

    //let pokemon move
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
    //watch pokeball
    if (watched == this->ui->pokeBallLabel)
    {
        //pokeball is clickable
        if (event->type() == QEvent::HoverEnter)
        {
            this->ui->pokeBallLabel->setCursor(QCursor(Qt::PointingHandCursor));
        }

        //pokeball is ready to launch
        if (event->type() == QEvent::MouseButtonPress)
        {
            //pokeball is clicked first time, pokeball is available
            if (!ballPressed)
            {
                this->ui->wordLabel->hide();
                this->ui->pokeBallLabel->setCursor(QCursor(Qt::PointingHandCursor));
                ballPressed = true;
            }
        }

        //animate when pokeball is release
        if (event->type() == QEvent::MouseButtonRelease)
        {
            if (ballPressed)
            {
                //get the position of mouse cursor
                this->ui->pokeBallLabel->setCursor(QCursor(Qt::ArrowCursor));
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                QPointF endP = mouseEvent->windowPos();

                int yInc = 430 - endP.y() - 50;
                int newY = endP.y() + yInc / 4;
                int z = 2 * yInc;

                //conclude whether pokemon is caught or not according to position of cursor
                if (z > ZMin && z < ZMax)
                {
                    caught = true;
                    std::cout << "caught" << std::endl;
                }
                else
                {
                    std::cout << "failed" << std::endl;
                }

                //animate the trail of pokeball and pokemon
                QPropertyAnimation* animation = new QPropertyAnimation(this->ui->pokeBallLabel, "geometry");
                animation->setStartValue(QRect(370, 430, 90, 90));
                animation->setEasingCurve(QEasingCurve::OutCirc);

                //pokemon gets in pokeball
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

                //pokeball is wasted and disappear
                else
                {
                    animation->setKeyValueAt(0.3, QRect(370, ((int)endP.y() - 45), 80, 80));
                    animation->setKeyValueAt(0.75, QRect(370, newY, 70, 70));
                    animation->setEndValue(QRect(370, newY, 0, 0));
                    animation->setDuration(2000);
                }
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                showWord(caught);

                //notice player to name pokemon if pokemon is caught
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
    //get a sentence in random
    QString sheet;
    int i = GetRanIndex(1, 3);
    std::string indexStr;
    std::stringstream stream;
    stream << i;
    stream >> indexStr;

    //show a caught sentence if pokemon is caught
    if (caught)
    {
        sheet = "#wordLabel{border-image: url(:/catch" + QString::fromStdString(indexStr) + ");}";
    }

    //show a failed sentence if pokemon is not caught
    else
    {
        sheet = "#wordLabel{border-image: url(:/fail" + QString::fromStdString(indexStr) + ");}";
    }
    this->ui->wordLabel->setStyleSheet(sheet);
    this->ui->wordLabel->show();
    return;
}

//send to server in a new thread
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
