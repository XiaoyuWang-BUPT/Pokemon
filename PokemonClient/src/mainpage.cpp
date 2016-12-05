#include "mainpage.h"
#include "ui_mainpage.h"
#include <sstream>

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowTitle("pokemon");
    QIcon LOGO (":/logo");
    this->setWindowIcon(LOGO);
    this->ui->pokeballButton->setStyleSheet("#pokeballButton{border-image: url(:/pokeball);}");
    this->ui->rankButton->setStyleSheet("#rankButton{border-image: url(:/rank);}");
    this->ui->onlinePlayerBtn->setStyleSheet("#onlinePlayerBtn{border-image: url(:/onlinePlayer);}");
    this->ui->myInfoButton->setStyleSheet("#myInfoButton{border-image: url(:/player);}");
    this->ui->packageButton->setStyleSheet("#packageButton{border-image: url(:/package);}");
    this->ui->storageButton->setStyleSheet("#storageButton{border-image: url(:/storage);}");

    this->ui->myInfoButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->onlinePlayerBtn->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->packageButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->pokeballButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->rankButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->storageButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->huntPicContainer->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->battlePicContainer->setCursor(QCursor(Qt::PointingHandCursor));

    this->ui->pokeballButton->installEventFilter(this);
    this->ui->huntPicContainer->installEventFilter(this);
    this->ui->battlePicContainer->installEventFilter(this);

    this->ui->pokeballButton->setToolTip("click to konw more");
    this->ui->huntPicContainer->setToolTip("click to hunt");
    this->ui->battlePicContainer->setToolTip("click to battle");
    this->ui->rankButton->setToolTip("rank");
    this->ui->onlinePlayerBtn->setToolTip("online players");
    this->ui->myInfoButton->setToolTip("about me");
    this->ui->packageButton->setToolTip("package");
    this->ui->storageButton->setToolTip("storage");

    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/resource/tyranitar.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);

    QMovie* movie = new QMovie(":/pikachu.gif");
    this->ui->welcomeLabel->setMovie(movie);
    movie->start();
    QObject::connect(this->ui->onlinePlayerBtn, SIGNAL(clicked(bool)), this, SLOT(onOnlinePlayerClicked()));
}

MainPage::MainPage(SocketClient* sc, QWidget *parent) :
    MainPage(parent)
{
    socketClient = sc;
}

MainPage::~MainPage()
{
    delete socketClient;
    delete ui;
}

DWORD WINAPI RecvdThreadFunc(SocketClient* socketClient, MainPage* mainpage);
DWORD WINAPI SendThreadFuncMainpage(LPVOID lParam, LPVOID sParam);

void MainPage::receiveSwitch()
{
    this->show();
    calledThread = std::thread(RecvdThreadFunc, socketClient, this);
    calledThread.detach();
}

void MainPage::LoadOnlinePlayer(json &recvJ)
{
    int amount = recvJ["amount"];
    for (int i = 0; i < amount; i++)
    {
        std::stringstream stream;
        std::string indexStr;
        stream << (i + 1);
        stream >> indexStr;
        std::string tmpKeyStr = "name" + indexStr;
        std::cout << recvJ[tmpKeyStr] << ":";
        tmpKeyStr = "rank" + indexStr;
        std::cout << recvJ[tmpKeyStr] << std::endl;
    }
}

bool MainPage::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->ui->huntPicContainer)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            this->hide();
            emit switchToHunt();
        }
    }
    if (watched == this->ui->battlePicContainer)
    {

    }
    if (watched == this->ui->pokeballButton)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            this->ui->pokeballButton->setGeometry(364, 149, 120, 120);
            QPropertyAnimation* animation = new QPropertyAnimation(this->ui->pokeballButton, "size");
            animation->setStartValue(QSize(120, 120));
            animation->setEndValue(QSize(118, 118));
            animation->setDuration(100);
            animation->start(QPropertyAnimation::DeleteWhenStopped);

            animation = new QPropertyAnimation(this->ui->pokeballButton, "size");
            animation->setStartValue(QSize(118, 118));
            animation->setEndValue(QSize(120, 120));
            animation->setDuration(100);
            animation->start(QPropertyAnimation::DeleteWhenStopped);
        }

        if (event->type() == QEvent::HoverEnter)
        {
            this->ui->pokeballButton->setGeometry(364, 149, 120, 120);
            QPropertyAnimation* animation = new QPropertyAnimation(this->ui->pokeballButton, "size");
            animation->setStartValue(QSize(120, 120));
            animation->setEndValue(QSize(122, 122));
            animation->setDuration(100);
            animation->start(QPropertyAnimation::DeleteWhenStopped);
        }

        if (event->type() == QEvent::HoverLeave)
        {
            this->ui->pokeballButton->setGeometry(364, 149, 122, 122);
            QPropertyAnimation* animation = new QPropertyAnimation(this->ui->pokeballButton, "size");
            animation->setStartValue(QSize(122, 122));
            animation->setEndValue(QSize(120, 120));
            animation->setDuration(100);
            animation->start(QPropertyAnimation::DeleteWhenStopped);
        }
    }
    return QWidget::eventFilter(watched, event);
}

bool MainPage::getRecvStr(QString str)
{
    recvString = str.toStdString();
    json recvJ = json::parse(recvString);
    recvString = "";

    std::string symbol = recvJ["symbol"];
    if (symbol == "onlinePlayer")
    {
        LoadOnlinePlayer(recvJ);
    }
    return true;
}

void MainPage::onOnlinePlayerClicked()
{
    json j;
    j["symbol"] = "onlinePlayer";
    j["end"] = "end";
    std::string str = j.dump();

    std::thread mainpageSendThread = std::thread(SendThreadFuncMainpage, socketClient, &str);
    mainpageSendThread.join();
}

DWORD WINAPI RecvdThreadFunc(SocketClient* socketClient, MainPage* mainpage)
{
    SOCKET ConnectSocket = socketClient->getConnectSocket();
    socketClient->ClearRecvBuf();
    socketClient->iResult = recv(ConnectSocket, socketClient->recvbuf, socketClient->recvbuflen, 0);
    if (socketClient->iResult > 0)
    {
        QMetaObject::invokeMethod(mainpage, "getRecvStr", Qt::DirectConnection, Q_ARG(QString, QString::fromStdString(socketClient->GetRecvStr())));
    }
    else if (socketClient->iResult == 0)
    {
        std::cout << "Connection closed" << std::endl;
    }
    else
    {
        std::cout << "recv failed with error " << WSAGetLastError() << std::endl;
    }
    return 0;
}

DWORD WINAPI SendThreadFuncMainpage(LPVOID lParam, LPVOID sParam)
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
