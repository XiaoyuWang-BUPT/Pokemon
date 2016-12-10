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
    this->ui->textBrowser->setOpenLinks(false);
    this->ui->pokeballButton->setStyleSheet("#pokeballButton{border-image: url(:/pokeball);}");//QToolTip{background-color:red;}");
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
    this->ui->closeOPButton->installEventFilter(this);

    this->ui->pokeballButton->setToolTip("click to know more");
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

    for (int i = 0; i < MAXSIZE_PLAYER; i++)
    {
        headLabel[i] = new QLabel(this->ui->headLabelWidget);
        headLabel[i]->setStyleSheet("border-image: url(:/online); background-color : rgba(255, 255, 255, 100);");
        headLabel[i]->hide();
        playerPokeButton[i] = new QPushButton(this->ui->playerPokeButtonWidget);
        playerPokeButton[i]->hide();
        playerPokeButton[i]->setStyleSheet("border-image: url(:/pokehauk);");
        playerPokeButton[i]->setFlat(true);
        playerPokeButton[i]->setCursor(QCursor(Qt::PointingHandCursor));
        playerPokeButton[i]->installEventFilter(this);
        thumbButton[i] = new QPushButton(this->ui->thumbButtonWidget);
        thumbButton[i]->hide();
        thumbButton[i]->setStyleSheet("border-image: url(:/thumb);");
        thumbButton[i]->setFlat(true);
        thumbButton[i]->setCursor(QCursor(Qt::PointingHandCursor));
        thumbButton[i]->installEventFilter(this);

        rankPokeButton[i] = new QPushButton(this->ui->rankPokeWidget);
        rankPokeButton[i]->hide();
        rankPokeButton[i]->setStyleSheet("border-image: url(:/pokehauk);");
        rankPokeButton[i]->setFlat(true);
        rankPokeButton[i]->setCursor(QCursor(Qt::PointingHandCursor));
        rankPokeButton[i]->installEventFilter(this);
        rankThumbButton[i] = new QPushButton(this->ui->rankThumbWidget);
        rankThumbButton[i]->hide();
        rankThumbButton[i]->setStyleSheet("border-image: url(:/thumb);");
        rankThumbButton[i]->setFlat(true);
        rankThumbButton[i]->setCursor(QCursor(Qt::PointingHandCursor));
        rankThumbButton[i]->installEventFilter(this);
    }

    scrollVLayout = new QVBoxLayout(this->ui->scrollWidget);
    for (int i = 0; i < MAXSIZE_POKEMON; i++)
    {
        scrollHLayout[i] = new QHBoxLayout();
        pokePicLabel[i] = new QLabel();
        pokeTextLabel[i] = new QLabel();
        exButton[i] = new QPushButton();
        exButton[i]->setFlat(true);
    }

    QObject::connect(this->ui->onlinePlayerBtn, SIGNAL(clicked(bool)), this, SLOT(onOnlinePlayerClicked()));
    QObject::connect(this->ui->OPReloadButton, SIGNAL(clicked(bool)), this, SLOT(onOnlinePlayerReloadClicked()));
    QObject::connect(this->ui->closeOPButton, SIGNAL(clicked(bool)), this->ui->listWidgetContainer, SLOT(hide()));
    QObject::connect(this->ui->closeOPButton, SIGNAL(clicked(bool)), this->ui->OPListWidget, SLOT(clear()));
    QObject::connect(this, SIGNAL(playerPokeClicked(int)), this, SLOT(onPlayerPokeClicked(int)));
    QObject::connect(this, SIGNAL(playerThumbClicked(int)), this, SLOT(onPlayerThumbClicked(int)));
    QObject::connect(this, SIGNAL(setOnlinePlayerIconSignal(int)), this, SLOT(setOnlinePlayerIcon(int)));    
    QObject::connect(this->ui->myInfoButton, SIGNAL(clicked(bool)), this, SLOT(onMyInfoClicked()));
    QObject::connect(this->ui->myinfoCloseBtn, SIGNAL(clicked(bool)), this, SLOT(onMyInfoClicked()));
    QObject::connect(this->ui->rankButton, SIGNAL(clicked(bool)), this, SLOT(onRankClicked()));
    QObject::connect(this->ui->closeRankButton, SIGNAL(clicked(bool)), this, SLOT(onRankClicked()));
    QObject::connect(this->ui->closeRankButton, SIGNAL(clicked(bool)), this->ui->pokeTableContainer, SLOT(hide()));
    QObject::connect(this, SIGNAL(setRankIconSignal(int)), this, SLOT(setRankIcons(int)));
    QObject::connect(this, SIGNAL(rankPokeClicked(int)), this, SLOT(onRankPokeClicked(int)));
    QObject::connect(this, SIGNAL(rankThumbClicked(int)), this, SLOT(onRankThumbClicked(int)));
    QObject::connect(this->ui->pokeCloseButton, SIGNAL(clicked(bool)), this->ui->pokeTableContainer, SLOT(hide()));
    QObject::connect(this->ui->closeOPButton, SIGNAL(clicked(bool)), this->ui->pokeTableContainer, SLOT(hide()));
    QObject::connect(this->ui->pokeballButton, SIGNAL(clicked(bool)), this->ui->readmeWidget, SLOT(show()));
    QObject::connect(this->ui->pokeballButton, SIGNAL(clicked(bool)), this->ui->rankWidget, SLOT(hide()));
    QObject::connect(this->ui->pokeballButton, SIGNAL(clicked(bool)), this->ui->myinfoWidget, SLOT(hide()));
    QObject::connect(this->ui->pokeballButton, SIGNAL(clicked(bool)), this->ui->listWidgetContainer, SLOT(hide()));
    QObject::connect(this->ui->pokeballButton, SIGNAL(clicked(bool)), this->ui->pokeTableContainer, SLOT(hide()));
    QObject::connect(this->ui->textBrowserCloseButton, SIGNAL(clicked(bool)), this->ui->readmeWidget, SLOT(hide()));
    QObject::connect(this->ui->textBrowser, SIGNAL(anchorClicked(QUrl)), this, SLOT(OpenInChrome(QUrl)));
    QObject::connect(this->ui->packageButton, SIGNAL(clicked(bool)), this, SLOT(onPackageClicked()));
    QObject::connect(this->ui->myPokemonCloseButton, SIGNAL(clicked(bool)), this, SLOT(onPackageClicked()));
    QObject::connect(this, SIGNAL(setPackegeScrollAreaSignal(QString, QString, QString, int)), this, SLOT(setPackageScrollArea(QString, QString, QString, int)));
    QObject::connect(this, SIGNAL(clearScrollAreaSignal()), this, SLOT(clearScrollArea()));
    QObject::connect(this->ui->storageButton, SIGNAL(clicked(bool)), this, SLOT(onStorageClicked()));
    QObject::connect(this->ui->myStorageCloseButton, SIGNAL(clicked(bool)), this, SLOT(onStorageClicked()));
}

MainPage::MainPage(SocketClient *sc, QWidget *parent) :
    MainPage(parent)
{
    socketClient = sc;
}

MainPage::~MainPage()
{
    delete socketClient;
    delete ui;
}

DWORD WINAPI RecvThreadFuncMainpage(SocketClient* socketClient, MainPage* mainpage);
DWORD WINAPI SendThreadFuncMainpage(LPVOID lParam, LPVOID sParam);

void MainPage::receiveSwitch()
{    
    this->ui->readmeWidget->hide();
    this->ui->listWidgetContainer->hide();
    this->ui->myinfoWidget->hide();
    this->ui->rankWidget->hide();
    this->ui->pokeTableContainer->hide();
    this->ui->myPokemonContainer->hide();
    this->ui->myStorageContainer->hide();
    this->ui->storageButton->setGeometry(622, 410, 48, 48);
    this->ui->myInfoButton->setGeometry(380, 410, 48, 48);
    this->ui->onlinePlayerBtn->setGeometry(270, 410, 48, 48);
    this->ui->rankButton->setGeometry(170, 410, 48, 48);
    this->ui->packageButton->setGeometry(490, 410, 48, 48);
    for (int i = 0; i < MAXSIZE_PLAYER; i++)
    {
        headLabel[i]->hide();
        playerPokeButton[i]->hide();
        thumbButton[i]->hide();
        rankPokeButton[i]->hide();
        rankThumbButton[i]->hide();
    }
    this->show();
    return;
}

void MainPage::setOnlinePlayerIcon(int i)
{
    headLabel[i]->setGeometry(2, 24*i, 24, 24);
    playerPokeButton[i]->setGeometry(2, 24*i, 24, 24);
    thumbButton[i]->setGeometry(10, 24*i, 24, 24);    
    thumbButton[i]->show();
    playerPokeButton[i]->show();
    headLabel[i]->show();
    return;
}

void MainPage::setRankIcons(int i)
{
    rankPokeButton[i]->setGeometry(2, 24*i, 24, 24);
    rankPokeButton[i]->show();
    rankThumbButton[i]->setGeometry(10, 24*i, 24, 24);
    rankThumbButton[i]->show();
    return;
}

void MainPage::setPackageScrollArea(QString kind, QString name, QString tip, int index)
{
    QString pixmap = ":/" + kind.toLower();
    pokePicLabel[index]->setPixmap(QPixmap(pixmap));
    pokePicLabel[index]->setAlignment(Qt::AlignCenter);
    pokeTextLabel[index]->setText(name);
    pokeTextLabel[index]->setToolTip(tip);
    pokeTextLabel[index]->setStyleSheet("font: 75 16pt Consolas;");
    exButton[index]->setStyleSheet("QPushButton{border-image : url(:/exchange);}");
    exButton[index]->setCursor(QCursor(Qt::PointingHandCursor));
    exButton[index]->setToolTip("Put in storage");
    pokePicLabel[index]->setMinimumSize(36, 36);
    pokeTextLabel[index]->setMinimumSize(36, 36);
    exButton[index]->setMinimumSize(48, 36);
    pokePicLabel[index]->setMaximumSize(36, 36);
    exButton[index]->setMaximumSize(36, 36);
    scrollHLayout[index]->addWidget(pokePicLabel[index]);
    scrollHLayout[index]->addSpacing(5);
    scrollHLayout[index]->addWidget(pokeTextLabel[index]);
    scrollHLayout[index]->addWidget(exButton[index]);
    scrollVLayout->addLayout(scrollHLayout[index]);
}

void MainPage::clearScrollArea()
{
    for (int i = 0; i < MAXSIZE_POKEMON; i++)
    {
        scrollHLayout[i]->removeWidget(pokePicLabel[i]);
        scrollHLayout[i]->removeWidget(pokeTextLabel[i]);
        scrollHLayout[i]->removeWidget(exButton[i]);
        scrollVLayout->removeItem(scrollHLayout[i]);
    }
}

void MainPage::LoadOnlinePlayer(json &recvJ)
{
    QListWidgetItem* item;
    int amount = recvJ["amount"];
    for (int i = 0; i < amount; i++)
    {
        std::stringstream stream;
        std::string indexStr;
        stream << (i + 1);
        stream >> indexStr;
        std::string nameKey = "name" + indexStr;
        std::string rankKey = "rank" + indexStr;
        std::string rateKey = "rate" + indexStr;
        std::string rank;
        std::string rate;
        stream.clear();
        stream << recvJ[rankKey];
        stream >> rank;
        stream.clear();
        stream << recvJ[rateKey];
        stream >> rate;
        QString str = QString::fromStdString(recvJ[nameKey]);
        playerNames[i] = recvJ[nameKey];
        int nameLen = str.length();
        int rankLen = rank.length();
        for (int j = 0; j < ((14 - nameLen - rankLen)/2); j++)
            str.append(" ");
        str.append(QString::fromStdString(rate + "%"));
        for (int j = 0; j < ((14 - nameLen - rankLen)/2); j++)
            str.append(" ");
        str.append(QString::fromStdString(rank));
        item = new QListWidgetItem(str, this->ui->OPListWidget);
        item->setFont(QFont("Consolas", 16, 2, false));
        item->setFlags(Qt::NoItemFlags);
        emit setOnlinePlayerIconSignal(i);
    }
    return;
}

bool MainPage::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->ui->huntPicContainer)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            this->hide();
            this->ui->readmeWidget->hide();
            this->ui->OPListWidget->clear();
            this->ui->rankWidget->hide();
            this->ui->rankButton->setGeometry(170, 410, 48, 48);
            this->ui->myinfoWidget->hide();
            this->ui->myInfoButton->setGeometry(380, 410, 48, 48);
            this->ui->myPokemonContainer->hide();
            emit clearScrollAreaSignal();
            this->ui->packageButton->setGeometry(490, 410, 48, 48);
            this->ui->listWidgetContainer->hide();
            this->ui->onlinePlayerBtn->setGeometry(270, 410, 48, 48);
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
            this->ui->pokeballButton->setGeometry(350, 150, 120, 120);
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
            this->ui->pokeballButton->setGeometry(350, 150, 120, 120);
            QPropertyAnimation* animation = new QPropertyAnimation(this->ui->pokeballButton, "size");
            animation->setStartValue(QSize(120, 120));
            animation->setEndValue(QSize(122, 122));
            animation->setDuration(100);
            animation->start(QPropertyAnimation::DeleteWhenStopped);
        }

        if (event->type() == QEvent::HoverLeave)
        {
            this->ui->pokeballButton->setGeometry(350, 150, 122, 122);
            QPropertyAnimation* animation = new QPropertyAnimation(this->ui->pokeballButton, "size");
            animation->setStartValue(QSize(122, 122));
            animation->setEndValue(QSize(120, 120));
            animation->setDuration(100);
            animation->start(QPropertyAnimation::DeleteWhenStopped);
        }
    }
    if (watched == this->ui->closeOPButton)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            this->ui->onlinePlayerBtn->setGeometry(270, 410, 48, 48);
            this->ui->battlePicContainer->setEnabled(true);
            this->ui->huntPicContainer->setEnabled(true);
        }
    }
    for (int i = 0; i < MAXSIZE_PLAYER; i++)
    {
        if (watched == this->playerPokeButton[i])
        {
            if (event->type() == QEvent::MouseButtonPress)
                emit playerPokeClicked(i);
        }
        if (watched == this->thumbButton[i])
        {
            if (event->type() == QEvent::MouseButtonPress)
                emit playerThumbClicked(i);
        }
        if (watched == this->rankPokeButton[i])
        {
            if (event->type() == QEvent::MouseButtonPress)
                emit rankPokeClicked(i);
        }
        if (watched == this->rankThumbButton[i])
        {
            if (event->type() == QEvent::MouseButtonPress)
                emit rankThumbClicked(i);
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
    if (symbol == "myinfo")
      {
        std::string name = recvJ["name"];
        int pokeNum = recvJ["pokemonNumber"];
        int rank = recvJ["rank"];
        int thumb = recvJ["thumb"];
        double rate = recvJ["rate"];
        std::string begintime = recvJ["begintime"];
        std::string beginYear = begintime.substr(0, 4);
        std::string beginMonth = begintime.substr(4, 2);
        std::string beginDay = begintime.substr(6, 2);
        std::string beginHour = begintime.substr(8, 2);
        std::string beginMin = begintime.substr(10,2);
        begintime.clear();
        begintime = beginYear + "/" + beginMonth + "/" + beginDay + " "
                + beginHour + ":" + beginMin;
        std::string gametime = recvJ["gametime"];
        std::string reverseGameTime(gametime.rbegin(),
                               gametime.rend());
        std::string gameMin = reverseGameTime.substr(0, 2);
        gameMin = std::string(gameMin.rbegin(), gameMin.rend());
        reverseGameTime.erase(0, 2);
        std::string gameHour = std::string(reverseGameTime.rbegin(),
                          reverseGameTime.rend());
        gametime = gameHour + "h " + gameMin + "m";
        std::stringstream stream;
        std::string pokeNumStr;
        std::string rankStr;
        std::string thumbStr;
        std::string rateStr;
        stream << pokeNum;
        stream >>pokeNumStr;
        stream.clear();
        stream << rank;
        stream >> rankStr;
        stream.clear();
        stream << thumb;
        stream >> thumbStr;
        stream.clear();
        stream << rate;
        stream >> rateStr;
        rateStr.append("%");
        std::string textString = "          " + name + "\n"
                + "Pokemon Number:" + pokeNumStr + "\n"
                + "Rank:" + rankStr + "  Rate:" + rateStr + "\n"
                + "Thumb Number:" + thumbStr + "\n"
                + "Game  Time:" + gametime + "\n"
                + "Begin From:\n" + "   " + begintime;
        this->ui->myinfoText->appendPlainText(QString::fromStdString(textString));
    }
    if (symbol == "hunt")
    {

    }
    if (symbol == "playerPoke")
    {
        int amount = recvJ["amount"];
        int row = 0;
        int col = 0;
        std::stringstream stream;
        std::string indexStr;
        std::string keyStr;
        for (int i = 0; i < amount && i < MAXSIZE_POKEMON; i++)
        {
            stream.clear();
            stream << i;
            stream >> indexStr;
            keyStr = "kind" + indexStr;
            QString kind = QString::fromStdString(recvJ[keyStr]);
            std::string tooltipStd = recvJ[keyStr];
            QString iconStr = ":/" + kind.toLower();
            QIcon icon(iconStr);
            QTableWidgetItem* tableItem = new QTableWidgetItem((QIcon)icon, "");
            keyStr = "name" + indexStr;
            std::string name = recvJ[keyStr];
            tooltipStd.append("\nName:" + name);
            keyStr = "level" + indexStr;
            std::string levelStr;
            stream.clear();
            stream << recvJ[keyStr];
            stream >> levelStr;
            tooltipStd.append("\nLevel:" + levelStr);
            QString tooltip = QString::fromStdString(tooltipStd);
            tableItem->setToolTip(tooltip);
            this->ui->pokeTable->setItem(row, col, tableItem);
            col++;
            if (col == TABLE_COL)
            {
                row++;
                col=0;
            }
        }
    }
    if (symbol == "thumb")
    {

    }
    if (symbol == "rank")
    {
        int amount = recvJ["amount"];
        QListWidgetItem* item;
        for (int i = 0; i < amount; i++)
        {
            std::stringstream stream;
            std::string indexStr;
            stream << i;
            stream >> indexStr;
            std::string nameKey = "name" + indexStr;
            std::string rankKey = "rank" + indexStr;
            std::string rateKey = "rate" + indexStr;
            std::string rank;
            std::string rate;
            stream.clear();
            stream << recvJ[rankKey];
            stream >> rank;
            stream.clear();
            stream << recvJ[rateKey];
            stream >> rate;
            QString str = "   ";
            str.append(QString::fromStdString(recvJ[nameKey]));
            rankPlayerNames[i] = recvJ[nameKey];
            std::string name = recvJ[nameKey];
            int nameLen = name.length();
            int rankLen = rank.length();
            for (int j = 0; j < ((13 - nameLen - rankLen)/2); j++)
                str.append(" ");
            str.append(QString::fromStdString(rate + "%"));
            for (int j = 0; j < ((13 - nameLen - rankLen)/2); j++)
                str.append(" ");
            str.append(QString::fromStdString(rank));
            item = new QListWidgetItem(str, this->ui->rankList);
            item->setFont(QFont("Consolas", 16, 2, false));
            emit setRankIconSignal(i);
        }
    }
    if (symbol == "package")
    {
        std::stringstream stream;
        std::string indexStr;
        std::string keyStr;
        std::string valueStr;
        std::string kind;
        std::string name;
        std::string tooltipStd = "";
        int amount = recvJ["amount"];
        for (int i = 0; i < amount && i < MAXSIZE_POKEMON; i++)
        {
            tooltipStd = "";
            stream.clear();
            stream << i;
            stream >> indexStr;
            keyStr =  "kind" + indexStr;
            valueStr = recvJ[keyStr];
            kind = recvJ[keyStr];
            tooltipStd.append("kind:" + valueStr);
            keyStr = "name" + indexStr;
            valueStr = recvJ[keyStr];
            name = recvJ[keyStr];
            tooltipStd.append("  name:" + valueStr);
            keyStr = "character" + indexStr;
            valueStr = recvJ[keyStr];
            tooltipStd.append("\ncharacter:" + valueStr);
            keyStr = "level" + indexStr;
            stream.clear();
            stream << recvJ[keyStr];
            stream >> valueStr;
            tooltipStd.append("     level:" + valueStr);
            keyStr = "exp" + indexStr;
            stream.clear();
            stream << recvJ[keyStr];
            stream >> valueStr;
            tooltipStd.append("\nexp:" + valueStr);
            keyStr = "att" + indexStr;
            stream.clear();
            stream << recvJ[keyStr];
            stream >> valueStr;
            tooltipStd.append("     attack:" + valueStr);
            keyStr = "def" + indexStr;
            stream.clear();
            stream << recvJ[keyStr];
            stream >> valueStr;
            tooltipStd.append("\ndefence" + valueStr);
            keyStr = "HP" + indexStr;
            stream.clear();
            stream << recvJ[keyStr];
            stream >> valueStr;
            tooltipStd.append("     HP:" + valueStr);
            keyStr = "speed" + indexStr;
            stream.clear();
            stream << recvJ[keyStr];
            stream >> valueStr;
            tooltipStd.append("\nspeed:" + valueStr);
            keyStr = "critical" + indexStr;
            stream.clear();
            stream << recvJ[keyStr];
            stream >> valueStr;
            tooltipStd.append("     critical:" + valueStr + "%");
            emit setPackegeScrollAreaSignal(QString::fromStdString(kind),
                                            QString::fromStdString(name),
                                            QString::fromStdString(tooltipStd),
                                            i);
        }
    }
    return true;
}

void MainPage::RecvAndSendOnlinePlayer(json j)
{
    std::string str = j.dump();
    std::thread mainpageSendThread = std::thread(SendThreadFuncMainpage, socketClient, &str);
    mainpageSendThread.join();

    std::thread mainpageRecvThread = std::thread(RecvThreadFuncMainpage, socketClient, this);
    mainpageRecvThread.join();
    return;
}

void MainPage::onOnlinePlayerClicked()
{
    this->ui->readmeWidget->hide();
    this->ui->OPListWidget->clear();
    this->ui->rankWidget->hide();
    this->ui->rankButton->setGeometry(170, 410, 48, 48);
    this->ui->myinfoWidget->hide();
    this->ui->myInfoButton->setGeometry(380, 410, 48, 48);
    this->ui->myPokemonContainer->hide();
    emit clearScrollAreaSignal();
    this->ui->packageButton->setGeometry(490, 410, 48, 48);
    this->ui->myStorageContainer->hide();
    this->ui->storageButton->setGeometry(622, 410, 48, 48);
    if (this->ui->listWidgetContainer->isHidden())
    {
        for (auto& s : playerNames)
            s = "";
        for (int i = 0; i < MAXSIZE_PLAYER; i++)
        {
            headLabel[i]->hide();
            playerPokeButton[i]->hide();
            thumbButton[i]->hide();
        }
        this->ui->battlePicContainer->setEnabled(false);
        this->ui->huntPicContainer->setEnabled(false);
        json j;
        j["symbol"] = "onlinePlayer";
        j["name"] = socketClient->getPlayerName();
        j["end"] = "end";
        RecvAndSendOnlinePlayer(j);
        this->ui->onlinePlayerBtn->setGeometry(258, 398, 72, 72);
        this->ui->listWidgetContainer->show();
    }
    else
    {
        for (int i = 0; i < MAXSIZE_PLAYER; i++)
        {
            headLabel[i]->hide();
            playerPokeButton[i]->hide();
            thumbButton[i]->hide();
        }
        this->ui->battlePicContainer->setEnabled(true);
        this->ui->huntPicContainer->setEnabled(true);
        this->ui->onlinePlayerBtn->setGeometry(270, 410, 48, 48);
        this->ui->listWidgetContainer->hide();
        this->ui->pokeTableContainer->hide();
        this->ui->OPListWidget->clear();
    }
    return;
}

void MainPage::onOnlinePlayerReloadClicked()
{
    this->ui->OPListWidget->clear();
    for (int i = 0; i < MAXSIZE_PLAYER; i++)
    {
        headLabel[i]->hide();
        playerPokeButton[i]->hide();
        thumbButton[i]->hide();
    }
    for (auto& s : playerNames)
        s = "";
    json j;
    j["symbol"] = "onlinePlayer";
    j["name"] = socketClient->getPlayerName();
    j["end"] = "end";
    RecvAndSendOnlinePlayer(j);
    return;
}

void MainPage::onPlayerPokeClicked(int i)
{
    if (this->ui->pokeTableContainer->isHidden())
    {
        json j;
        j["symbol"] = "playerPoke";
        j["name"] = playerNames[i];
        j["end"] = "end";
        this->ui->pokeTable->clear();
        RecvAndSendOnlinePlayer(j);
        this->ui->pokeTableContainer->setGeometry(480, 190, 320, 200);
        this->ui->pokeOwnerLabel->setText(QString::fromStdString(playerNames[i]));
        this->ui->pokeTableContainer->show();
    }
    else
    {
        this->ui->pokeTableContainer->hide();
    }
    return;
}

void MainPage::onPlayerThumbClicked(int i)
{
    json j;
    j["symbol"] = "thumb";
    j["name"] = playerNames[i];
    j["end"] = "end";
    RecvAndSendOnlinePlayer(j);
    return;
}

void MainPage::onMyInfoClicked()
{
    this->ui->readmeWidget->hide();
    this->ui->myinfoText->clear();
    this->ui->rankWidget->hide();
    this->ui->rankButton->setGeometry(170, 410, 48, 48);
    this->ui->listWidgetContainer->hide();
    this->ui->onlinePlayerBtn->setGeometry(270, 410, 48, 48);
    this->ui->myPokemonContainer->hide();
    emit clearScrollAreaSignal();
    this->ui->packageButton->setGeometry(490, 410, 48, 48);
    this->ui->myStorageContainer->hide();
    this->ui->storageButton->setGeometry(622, 410, 48, 48);
    if (this->ui->myinfoWidget->isHidden())
    {
        json j;
        j["symbol"] = "myinfo";
        j["name"] = socketClient->getPlayerName();
        j["end"] = "end";
        RecvAndSendOnlinePlayer(j);
        this->ui->myInfoButton->setGeometry(368, 398, 72, 72);
        this->ui->myinfoWidget->show();
    }
    else
    {
        this->ui->myInfoButton->setGeometry(380, 410, 48, 48);
        this->ui->myinfoWidget->hide();
    }
    return;
}

void MainPage::onRankClicked()
{
    this->ui->readmeWidget->hide();
    this->ui->myinfoWidget->hide();
    this->ui->myInfoButton->setGeometry(380, 410, 48, 48);
    this->ui->listWidgetContainer->hide();
    this->ui->onlinePlayerBtn->setGeometry(270, 410, 48, 48);
    this->ui->myPokemonContainer->hide();
    emit clearScrollAreaSignal();
    this->ui->packageButton->setGeometry(490, 410, 48, 48);
    this->ui->myStorageContainer->hide();
    this->ui->storageButton->setGeometry(622, 410, 48, 48);

    for (int i = 0; i < MAXSIZE_PLAYER; i++)
    {
        headLabel[i]->hide();
        playerPokeButton[i]->hide();
        thumbButton[i]->hide();
    }

    if (this->ui->rankWidget->isHidden())
    {
        this->ui->rankList->clear();
        json j;
        j["symbol"] = "rank";
        j["name"] = socketClient->getPlayerName();
        j["end"] = "end";
        RecvAndSendOnlinePlayer(j);
        this->ui->rankWidget->show();
        this->ui->rankButton->setGeometry(158, 398, 72, 72);
    }
    else
    {
        this->ui->rankList->clear();
        this->ui->rankWidget->hide();
        this->ui->pokeTableContainer->hide();
        this->ui->rankButton->setGeometry(170, 410, 48, 48);
    }
    return;
}

void MainPage::onRankPokeClicked(int i)
{
    if (this->ui->pokeTableContainer->isHidden())
    {
        json j;
        j["symbol"] = "playerPoke";
        j["name"] = rankPlayerNames[i];
        j["end"] = "end";
        this->ui->pokeTable->clear();
        RecvAndSendOnlinePlayer(j);
        this->ui->pokeOwnerLabel->setText(QString::fromStdString(rankPlayerNames[i]));
        this->ui->pokeTableContainer->setGeometry(370, 190, 320, 200);
        this->ui->pokeTableContainer->show();
    }
    else
    {
        this->ui->pokeTableContainer->hide();
    }
    return;
}

void MainPage::onRankThumbClicked(int i)
{
    json j;
    j["symbol"] = "thumb";
    j["name"] = rankPlayerNames[i];
    j["end"] = "end";
    RecvAndSendOnlinePlayer(j);
    return;
}

void MainPage::onPackageClicked()
{
    this->ui->readmeWidget->hide();
    this->ui->myinfoWidget->hide();
    this->ui->myInfoButton->setGeometry(380, 410, 48, 48);
    this->ui->listWidgetContainer->hide();
    this->ui->onlinePlayerBtn->setGeometry(270, 410, 48, 48);
    this->ui->rankWidget->hide();
    this->ui->rankButton->setGeometry(170, 410, 48, 48);
    this->ui->myStorageContainer->hide();
    this->ui->storageButton->setGeometry(622, 410, 48, 48);
    if (this->ui->myPokemonContainer->isHidden())
    {
        json j;
        j["symbol"] = "package";
        j["name"] = socketClient->getPlayerName();
        j["end"] = "end";
        RecvAndSendOnlinePlayer(j);
        this->ui->myPokemonContainer->show();
        this->ui->packageButton->setGeometry(478, 398, 72, 72);
    }
    else
    {
        this->ui->myPokemonContainer->hide();
        this->ui->packageButton->setGeometry(490, 410, 48, 48);
        emit clearScrollAreaSignal();
    }
    return;
}

void MainPage::onStorageClicked()
{
    this->ui->readmeWidget->hide();
    this->ui->myinfoWidget->hide();
    this->ui->myInfoButton->setGeometry(380, 410, 48, 48);
    this->ui->listWidgetContainer->hide();
    this->ui->onlinePlayerBtn->setGeometry(270, 410, 48, 48);
    this->ui->myPokemonContainer->hide();
    emit clearScrollAreaSignal();
    this->ui->packageButton->setGeometry(490, 410, 48, 48);
    this->ui->rankWidget->hide();
    this->ui->rankButton->setGeometry(170, 410, 48, 48);

    if (this->ui->myStorageContainer->isHidden())
    {
        this->ui->myStorageContainer->show();
        this->ui->storageButton->setGeometry(598, 386, 72, 72);
    }
    else
    {
        this->ui->myStorageContainer->hide();
        this->ui->storageButton->setGeometry(622, 410, 48, 48);
    }
}

void MainPage::OpenInChrome(const QUrl &url)
{
    ShellExecuteA(NULL, "open", url.toString().toStdString().c_str(), "", "", SW_SHOW);
}

DWORD WINAPI RecvThreadFuncMainpage(SocketClient* socketClient, MainPage* mainpage)
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
