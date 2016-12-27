#include "mainpage.h"
#include "ui_mainpage.h"
#include <sstream>

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    this->InitUI();
    this->SetEventFilter();
    this->InitConnect();
}

void MainPage::InitUI()
{
    setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("pokemon");
    QIcon LOGO (":/logo");
    this->setWindowIcon(LOGO);
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/resource/tyranitar.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);

    this->SetCursors();
    this->SetToolTips();
    this->SetStyles();
    this->InitPlayerArea();
    this->InitPokemonArea();
}

void MainPage::SetEventFilter()
{
    this->ui->pokeballButton->installEventFilter(this);
    this->ui->huntPicContainer->installEventFilter(this);
    this->ui->battlePicContainer->installEventFilter(this);
    this->ui->closeOPButton->installEventFilter(this);
    this->ui->fireGold->installEventFilter(this);
    this->ui->fireBronze->installEventFilter(this);
    this->ui->fireSilver->installEventFilter(this);
    this->ui->waterGold->installEventFilter(this);
    this->ui->waterSilver->installEventFilter(this);
    this->ui->waterBronze->installEventFilter(this);
    this->ui->bushGold->installEventFilter(this);
    this->ui->bushSilver->installEventFilter(this);
    this->ui->bushBronze->installEventFilter(this);
    this->ui->elecGold->installEventFilter(this);
    this->ui->elecSilver->installEventFilter(this);
    this->ui->elecBronze->installEventFilter(this);
}

void MainPage::SetToolTips()
{
    this->ui->pokeballButton->setToolTip("click to know more");
    this->ui->huntPicContainer->setToolTip("click to hunt");
    this->ui->rankButton->setToolTip("rank");
    this->ui->onlinePlayerBtn->setToolTip("online players");
    this->ui->myInfoButton->setToolTip("about me");
    this->ui->packageButton->setToolTip("package");
    this->ui->storageButton->setToolTip("storage");
    this->ui->otherLabel->setToolTip("To be online");
    this->ui->anotherLabel->setToolTip("To be online");
    this->ui->fireGold->setToolTip("VS fire master");
    this->ui->fireSilver->setToolTip("VS fire gym leader");
    this->ui->fireBronze->setToolTip("VS fire trainer");
    this->ui->waterGold->setToolTip("VS water master");
    this->ui->waterSilver->setToolTip("VS water gym leader");
    this->ui->waterBronze->setToolTip("VS water trainer");
    this->ui->bushGold->setToolTip("VS bush master");
    this->ui->bushSilver->setToolTip("VS bush gymleader");
    this->ui->bushBronze->setToolTip("VS bush trainer");
    this->ui->elecGold->setToolTip("VS electricity master");
    this->ui->elecSilver->setToolTip("VS electricity gym leader");
    this->ui->elecBronze->setToolTip("VS electricity trainer");
    this->ui->closeMainWidgetButton->setToolTip("close");
    this->ui->updownButton->setToolTip("Click to open");
}

void MainPage::SetCursors()
{
    this->ui->updownButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->myInfoButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->onlinePlayerBtn->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->packageButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->pokeballButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->rankButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->storageButton->setCursor(QCursor(Qt::PointingHandCursor));
    this->ui->huntPicContainer->setCursor(QCursor(Qt::PointingHandCursor));

}

void MainPage::SetStyles()
{
    this->ui->textBrowser->setOpenLinks(false);
    this->ui->pokeballButton->setStyleSheet("#pokeballButton{border-image: url(:/pokeball);}");//QToolTip{background-color:red;}");
    this->ui->rankButton->setStyleSheet("#rankButton{border-image: url(:/rank);}");
    this->ui->onlinePlayerBtn->setStyleSheet("#onlinePlayerBtn{border-image: url(:/onlinePlayer);}");
    this->ui->myInfoButton->setStyleSheet("#myInfoButton{border-image: url(:/player);}");
    this->ui->packageButton->setStyleSheet("#packageButton{border-image: url(:/package);}");
    this->ui->storageButton->setStyleSheet("#storageButton{border-image: url(:/storage);}");
    this->ui->updownButton->setStyleSheet("#updownButton{border-image: url(:/down);}");
     this->ui->battleChoice->resize(274, 0);
}

void MainPage::InitPlayerArea()
{
    //initialize online player list widget
    for (int i = 0; i < MAXSIZE_PLAYER; i++)
    {
        //set head label
        headLabel[i] = new QLabel(this->ui->headLabelWidget);
        headLabel[i]->setStyleSheet("border-image: url(:/online); background-color : rgba(255, 255, 255, 100);");
        headLabel[i]->hide();

        //set 'look through player's pokemon' button
        playerPokeButton[i] = new QPushButton(this->ui->playerPokeButtonWidget);
        playerPokeButton[i]->hide();
        playerPokeButton[i]->setStyleSheet("border-image: url(:/pokehauk);");
        playerPokeButton[i]->setFlat(true);
        playerPokeButton[i]->setCursor(QCursor(Qt::PointingHandCursor));
        playerPokeButton[i]->installEventFilter(this);

        //set 'give player a thumb' button
        thumbButton[i] = new QPushButton(this->ui->thumbButtonWidget);
        thumbButton[i]->hide();
        thumbButton[i]->setStyleSheet("border-image: url(:/thumb);");
        thumbButton[i]->setFlat(true);
        thumbButton[i]->setCursor(QCursor(Qt::PointingHandCursor));
        thumbButton[i]->installEventFilter(this);

        //set 'look through player's pokemon' button
        rankPokeButton[i] = new QPushButton(this->ui->rankPokeWidget);
        rankPokeButton[i]->hide();
        rankPokeButton[i]->setStyleSheet("border-image: url(:/pokehauk);");
        rankPokeButton[i]->setFlat(true);
        rankPokeButton[i]->setCursor(QCursor(Qt::PointingHandCursor));
        rankPokeButton[i]->installEventFilter(this);

        //set 'give player thumb' button
        rankThumbButton[i] = new QPushButton(this->ui->rankThumbWidget);
        rankThumbButton[i]->hide();
        rankThumbButton[i]->setStyleSheet("border-image: url(:/thumb);");
        rankThumbButton[i]->setFlat(true);
        rankThumbButton[i]->setCursor(QCursor(Qt::PointingHandCursor));
        rankThumbButton[i]->installEventFilter(this);
    }
}

void MainPage::InitPokemonArea()
{
    //initialize player's pokemon table widget
    scrollVLayout = new QVBoxLayout();
    for (int i = 0; i < MAXSIZE_POKEMON; i++)
    {
        scrollHLayout[i] = new QHBoxLayout();
        pokePicLabel[i] = new QLabel();
        pokeTextLabel[i] = new QLabel();
        exButton[i] = new QPushButton();
        exButton[i]->setFlat(true);
        exButton[i]->installEventFilter(this);
    }
}

void MainPage::InitConnect()
{
    QObject::connect(this->ui->closeMainWidgetButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    QObject::connect(this->ui->onlinePlayerBtn, SIGNAL(clicked(bool)), this, SLOT(onOnlinePlayerClicked()));
    QObject::connect(this->ui->OPReloadButton, SIGNAL(clicked(bool)), this, SLOT(onOnlinePlayerReloadClicked()));
    QObject::connect(this->ui->closeOPButton, SIGNAL(clicked(bool)), this->ui->listWidgetContainer, SLOT(hide()));
    QObject::connect(this->ui->closeOPButton, SIGNAL(clicked(bool)), this->ui->OPListWidget, SLOT(clear()));
    QObject::connect(this, SIGNAL(PlayerPokeClicked(int)), this, SLOT(onPlayerPokeClicked(int)));
    QObject::connect(this, SIGNAL(PlayerThumbClicked(int)), this, SLOT(onPlayerThumbClicked(int)));
    QObject::connect(this, SIGNAL(SetOnlinePlayerIconSignal(int)), this, SLOT(SetOnlinePlayerIcon(int)));
    QObject::connect(this->ui->myInfoButton, SIGNAL(clicked(bool)), this, SLOT(onMyInfoClicked()));
    QObject::connect(this->ui->myinfoCloseBtn, SIGNAL(clicked(bool)), this, SLOT(onMyInfoClicked()));
    QObject::connect(this, SIGNAL(SetMyInfoSignal(int, int, double, QString)), this, SLOT(SetMyInfo(int, int, double, QString)));
    QObject::connect(this->ui->rankButton, SIGNAL(clicked(bool)), this, SLOT(onRankClicked()));
    QObject::connect(this->ui->closeRankButton, SIGNAL(clicked(bool)), this, SLOT(onRankClicked()));
    QObject::connect(this->ui->closeRankButton, SIGNAL(clicked(bool)), this->ui->pokeTableContainer, SLOT(hide()));
    QObject::connect(this, SIGNAL(SetRankIconSignal(int)), this, SLOT(SetRankIcons(int)));
    QObject::connect(this, SIGNAL(RankPokeClicked(int)), this, SLOT(onRankPokeClicked(int)));
    QObject::connect(this, SIGNAL(RankThumbClicked(int)), this, SLOT(onRankThumbClicked(int)));
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
    QObject::connect(this, SIGNAL(SetPackegeScrollAreaSignal(QString, QString, QString, QString, int)), this, SLOT(SetPackageScrollArea(QString, QString, QString, QString, int)));
    QObject::connect(this, SIGNAL(ClearScrollAreaSignal(QString)), this, SLOT(ClearScrollArea(QString)));
    QObject::connect(this->ui->storageButton, SIGNAL(clicked(bool)), this, SLOT(onStorageClicked()));
    QObject::connect(this->ui->myStorageCloseButton, SIGNAL(clicked(bool)), this, SLOT(onStorageClicked()));
    QObject::connect(this, SIGNAL(ExButtonClicked(QObject*,int)), this, SLOT(onExButtonClicked(QObject*,int)));
    QObject::connect(this, SIGNAL(ExchangePokeSignal(QString, bool)), this, SLOT(ExchangePokemon(QString, bool)));
    QObject::connect(this->ui->updownButton, SIGNAL(clicked(bool)), this, SLOT(onUpDownClicked()));
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

void MainPage::SetOnlinePlayerIcon(int i)
{
    //set online player ui items in correct position
    headLabel[i]->setGeometry(2, 24*i, 24, 24);
    playerPokeButton[i]->setGeometry(2, 24*i, 24, 24);
    thumbButton[i]->setGeometry(10, 24*i, 24, 24);    
    thumbButton[i]->show();
    playerPokeButton[i]->show();
    headLabel[i]->show();
    return;
}

void MainPage::SetRankIcons(int i)
{
    //set rank ui items in right position
    rankPokeButton[i]->setGeometry(2, 24*i, 24, 24);
    rankPokeButton[i]->show();
    rankThumbButton[i]->setGeometry(10, 24*i, 24, 24);
    rankThumbButton[i]->show();
    return;
}

void MainPage::SetPackageScrollArea(QString symbol, QString kind, QString name, QString tip, int index)
{
    //get pokemon's gif resource according to its kind
    QString pixmap = ":/" + kind.toLower();
    pokePicLabel[index]->setScaledContents(true);
    pokePicLabel[index]->setPixmap(QPixmap(pixmap));
    pokePicLabel[index]->setAlignment(Qt::AlignCenter);

    //display pokemon's detailed information
    pokeTextLabel[index]->setText(name);
    pokeTextLabel[index]->setToolTip(tip);
    pokeTextLabel[index]->setStyleSheet("font: 75 16pt Consolas;");

    //set exchange button stylesheet
    exButton[index]->setStyleSheet("QPushButton{border-image : url(:/exchange);}");
    exButton[index]->setCursor(QCursor(Qt::PointingHandCursor));
    exButton[index]->setToolTip("Put in storage");
    pokePicLabel[index]->setMinimumSize(36, 36);
    pokeTextLabel[index]->setMinimumSize(36, 36);
    exButton[index]->setMinimumSize(36, 36);
    pokePicLabel[index]->setMaximumSize(36, 36);
    exButton[index]->setMaximumSize(36, 36);
    pokePicLabel[index]->show();
    pokeTextLabel[index]->show();
    exButton[index]->show();

    //add pokemon's items in layout
    scrollHLayout[index]->addWidget(pokePicLabel[index]);
    scrollHLayout[index]->addWidget(pokeTextLabel[index]);
    scrollHLayout[index]->addWidget(exButton[index]);

    //add pokemon's layout to super layout
    scrollVLayout->addLayout(scrollHLayout[index]);

    //set package scroll widget if player chose package to look through
    if (symbol == "package")
    {
        this->ui->scrollWidget->setLayout(scrollVLayout);
    }

    //set storage scroll widget if player chose storage to look through
    if (symbol == "storage")
        this->ui->storageScrollWidget->setLayout(scrollVLayout);
}

void MainPage::SetMyInfo(int pokeNum, int rank, double rate, QString info)
{
    //display player's detailed information
    this->ui->myinfoText->appendPlainText(info);
    QString trainerToolTip = "";
    QString rankToolTip = "";
    QString rateToolTip = "";
    std::stringstream stream;
    std::string numStr;

    //displayer player's pokemon number badge
    if (pokeNum < PokeNumLevel[0])
    {
        trainerToolTip.append("Noviciate\nGet ");
        stream.clear();
        stream << PokeNumLevel[0];
        stream >> numStr;
        trainerToolTip.append(QString::fromStdString(numStr));
        trainerToolTip.append(" pokemon to get promoted");
    }
    if (pokeNum >= PokeNumLevel[0] && pokeNum < PokeNumLevel[1])
    {
        trainerToolTip.append("Sophomore\nGet ");
        stream.clear();
        stream << PokeNumLevel[1];
        stream >> numStr;
        trainerToolTip.append(QString::fromStdString(numStr));
        trainerToolTip.append(" pokemon to get promoted");
        this->ui->trainerLevelLabel->setStyleSheet("#trainerLevelLabel{image: url(:/trainerBronze);}");
    }
    if (pokeNum >= PokeNumLevel[1] && pokeNum < PokeNumLevel[2])
    {
        trainerToolTip.append("Junior\nGet ");
        stream.clear();
        stream << PokeNumLevel[2];
        stream >> numStr;
        trainerToolTip.append(QString::fromStdString(numStr));
        trainerToolTip.append(" pokemon to get promoted");
        this->ui->trainerLevelLabel->setStyleSheet("#trainerLevelLabel{image: url(:/trainerSilver);}");
    }
    if (pokeNum >= PokeNumLevel[2])
    {
        trainerToolTip.append("Senior\nGinius pokemon trainer");
        this->ui->trainerLevelLabel->setStyleSheet("#trainerLevelLabel{image: url(:/trainerGold);}");
    }
    this->ui->trainerLevelLabel->setToolTip(trainerToolTip);

    //display player's rank badge
    if (rank < RankLevel[0])
    {
        rankToolTip.append("Noviciate\nGet ");
        stream.clear();
        stream << RankLevel[0];
        stream >> numStr;
        rankToolTip.append(QString::fromStdString(numStr));
        rankToolTip.append(" rank score to get promoted");
    }
    if (rank >= RankLevel[0] && rank < RankLevel[1])
    {
        rankToolTip.append("Sophomore\nGet ");
        stream.clear();
        stream << RankLevel[1];
        stream >> numStr;
        rankToolTip.append(QString::fromStdString(numStr));
        rankToolTip.append(" rank score to get promoted");
        this->ui->rankLevelLabel->setStyleSheet("#rankLevelLabel{image: url(:/rankBronze);}");
    }
    if (rank >= RankLevel[1] && rank < RankLevel[2])
    {
        rankToolTip.append("Junior\nGet ");
        stream.clear();
        stream << RankLevel[2];
        stream >> numStr;
        rankToolTip.append(QString::fromStdString(numStr));
        rankToolTip.append(" rank score to get promoted");
        this->ui->rankLevelLabel->setStyleSheet("#rankLevelLabel{image: url(:/rankSilver);}");
    }
    if (rank >= RankLevel[2])
    {
        rankToolTip.append("Senior\nGinius ranker");
        this->ui->rankLevelLabel->setStyleSheet("#rankLevelLabel{image: url(:/rankGold);}");
    }
    this->ui->rankLevelLabel->setToolTip(rankToolTip);

    //displayer player's rate badge
    int rateInteger = (int)rate;
    if (rateInteger < RateLevel[0])
    {
        rateToolTip.append("Noviciate\nGet ");
        stream.clear();
        stream << RateLevel[0];
        stream >> numStr;
        rateToolTip.append(QString::fromStdString(numStr));
        rateToolTip.append(" % rate to get promoted");
    }
    if ((rateInteger >= RateLevel[0]) && (rateInteger < RateLevel[1]))
    {
        rateToolTip.append("Sophomore\nGet ");
        stream.clear();
        stream << RateLevel[1];
        stream >> numStr;
        rateToolTip.append(QString::fromStdString(numStr));
        rateToolTip.append(" % rate to get promoted");
        this->ui->rateLevelLabel->setStyleSheet("#rateLevelLabel{image: url(:/rateBronze);}");
    }
    if ((rateInteger >= RateLevel[1]) && (rateInteger < RateLevel[2]))
    {
        rateToolTip.append("Junior\nGet ");
        stream.clear();
        stream << RateLevel[2];
        stream >> numStr;
        rateToolTip.append(QString::fromStdString(numStr));
        rateToolTip.append(" % rate to get promoted");
        this->ui->rateLevelLabel->setStyleSheet("#rateLevelLabel{image: url(:/rateSilver);}");
    }
    if (rateInteger >= RateLevel[2])
    {
        rateToolTip.append("Senior\nGinius combater");
        this->ui->rateLevelLabel->setStyleSheet("#rateLevelLabel{image: url(:/ratetGold);}");
    }
    this->ui->rateLevelLabel->setToolTip(rateToolTip);
}

void MainPage::ClearScrollArea(QString packorsto)
{
    //hide all ui items when leave this page
    for (int i = 0; i < MAXSIZE_POKEMON; i++)
    {
        scrollHLayout[i]->removeWidget(pokePicLabel[i]);
        scrollHLayout[i]->removeWidget(pokeTextLabel[i]);
        scrollHLayout[i]->removeWidget(exButton[i]);
        scrollVLayout->removeItem(scrollHLayout[i]);
        pokePicLabel[i]->hide();
        pokeTextLabel[i]->hide();
        exButton[i]->hide();
        pokePicLabel[i]->clear();
        pokeTextLabel[i]->clear();
    }

    //clear variable when leave this page
    if (packorsto == "package")
        packPokemon.clear();
    if (packorsto == "storage")
        stoPokemon.clear();
    if (packorsto == "both")
    {
        packPokemon.clear();
        stoPokemon.clear();
    }
}

void MainPage::ExchangePokemon(QString symbol, bool success)
{
    //player wanna take pokemon from package to storage
    if (symbol == "packout")
    {
        exClicked = true;
        emit this->ui->packageButton->clicked();
    }

    //player wanna take pokemon from storage to package
    if (symbol == "stoout")
    {
        //package is not full
        if (success)
        {
            exClicked = true;
            emit this->ui->storageButton->clicked();
        }

        //pakcage is full
        else
        {
            QMessageBox::information(this, "info", "Package Full");
        }
        delHLayIndex = -1;
    }
}

void MainPage::LoadOnlinePlayer(json &recvJ)
{
    QListWidgetItem* item;
    int amount = recvJ["amount"];

    //add player's items to layout according to online player's number
    for (int i = 0; i < amount; i++)
    {
        std::stringstream stream;
        std::string indexStr;
        stream << i;
        stream >> indexStr;
        std::string nameKey = "name" + indexStr;
        std::string rankKey = "rank" + indexStr;
        std::string rateKey = "rate" + indexStr;
        std::string name = recvJ[nameKey];
        std::string rank;
        std::string rate;
        stream.clear();
        stream << recvJ[rankKey];
        stream >> rank;
        stream.clear();
        double r = recvJ[rateKey];
        r *= 100;
        stream << std::setprecision(3) << r;
        stream >> rate;
        playerNames[i] = recvJ[nameKey];
        std::string infoStr;
        std::string tmpStr;
        stream.clear();
        stream << name;
        stream >> tmpStr;
        infoStr.append(tmpStr);
        stream.clear();
        stream << std::setw(7) << std::setfill('*') << (rate + "%");
        stream >> tmpStr;
        infoStr.append(tmpStr);
        stream.clear();
        stream << std::setw(7) << std::setfill('*') << rank;
        stream >> tmpStr;
        infoStr.append(tmpStr);
        QString str = QString::fromStdString(infoStr);
        item = new QListWidgetItem(str, this->ui->OPListWidget);
        item->setFont(QFont("Consolas", 16, 2, false));
        item->setFlags(Qt::NoItemFlags);
        emit SetOnlinePlayerIconSignal(i);
    }
    return;
}

void MainPage::SwitchClear()
{
    //re-initialize when leave this page
    this->hide();
    this->ui->readmeWidget->hide();
    this->ui->OPListWidget->clear();
    this->ui->rankWidget->hide();
    this->ui->rankButton->setGeometry(170, 410, 48, 48);
    this->ui->myinfoWidget->hide();
    this->ui->myInfoButton->setGeometry(380, 410, 48, 48);
    this->ui->myPokemonContainer->hide();
    emit ClearScrollAreaSignal("both");
    this->ui->packageButton->setGeometry(490, 410, 48, 48);
    this->ui->listWidgetContainer->hide();
    this->ui->onlinePlayerBtn->setGeometry(270, 410, 48, 48);
    this->ui->updownButton->setToolTip("Click to close");
    this->ui->updownButton->setStyleSheet("#updownButton{border-image: url(:/down)}");
    this->ui->battleChoice->resize(274, 0);
    return;
}

bool MainPage::eventFilter(QObject *watched, QEvent *event)
{
    //watch hunt contaienr
    if (watched == this->ui->huntPicContainer)
    {
        //switch to hunt page when clicked
        if (event->type() == QEvent::MouseButtonPress)
        {
            SwitchClear();
            emit SwitchToHunt();
        }
    }

    //watch battle choices buttons and switch to battle page
    if (watched == this->ui->fireGold)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            SwitchClear();
            emit SwitchToBattle("fire", "gold");
        }
    }
    if (watched == this->ui->fireSilver)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            SwitchClear();
            emit SwitchToBattle("fire", "silver");
        }
    }
    if (watched == this->ui->fireBronze)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            SwitchClear();
            emit SwitchToBattle("fire", "bronze");
        }
    }
    if (watched == this->ui->waterGold)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            SwitchClear();
            emit SwitchToBattle("water", "gold");
        }
    }
    if (watched == this->ui->waterSilver)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            SwitchClear();;
            emit SwitchToBattle("water", "silver");
        }
    }
    if (watched == this->ui->waterBronze)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            SwitchClear();
            emit SwitchToBattle("water", "bronze");
        }
    }
    if (watched == this->ui->bushGold)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            SwitchClear();
            emit SwitchToBattle("bush", "gold");
        }
    }
    if (watched == this->ui->bushSilver)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            SwitchClear();
            emit SwitchToBattle("bush", "silver");
        }
    }
    if (watched == this->ui->bushBronze)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            SwitchClear();
            emit SwitchToBattle("bush", "bronze");
        }
    }
    if (watched == this->ui->elecGold)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            SwitchClear();
            emit SwitchToBattle("electricity", "gold");
        }
    }
    if (watched == this->ui->elecSilver)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            SwitchClear();
            emit SwitchToBattle("electricity", "silver");
        }
    }
    if (watched == this->ui->elecBronze)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            SwitchClear();
            emit SwitchToBattle("electricity", "bronze");
        }
    }

    //watch pokeball
    if (watched == this->ui->pokeballButton)
    {
        //start animation when pokeball is clicked
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

        //start animation when pokeball is hovered
        if (event->type() == QEvent::HoverEnter)
        {
            this->ui->pokeballButton->setGeometry(350, 150, 120, 120);
            QPropertyAnimation* animation = new QPropertyAnimation(this->ui->pokeballButton, "size");
            animation->setStartValue(QSize(120, 120));
            animation->setEndValue(QSize(122, 122));
            animation->setDuration(100);
            animation->start(QPropertyAnimation::DeleteWhenStopped);
        }

        //start animation when hover leaves
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

    //watch online player close page button
    if (watched == this->ui->closeOPButton)
    {
        //enable other button
        if (event->type() == QEvent::MouseButtonPress)
        {
            this->ui->onlinePlayerBtn->setGeometry(270, 410, 48, 48);
            this->ui->battlePicContainer->setEnabled(true);
            this->ui->huntPicContainer->setEnabled(true);
        }
    }

    //watch online players' and rank players' pokemon buttons and thumb buttons
    for (int i = 0; i < MAXSIZE_PLAYER; i++)
    {
        if (watched == this->playerPokeButton[i])
        {
            if (event->type() == QEvent::MouseButtonPress)
                emit PlayerPokeClicked(i);
        }
        if (watched == this->thumbButton[i])
        {
            if (event->type() == QEvent::MouseButtonPress)
                emit PlayerThumbClicked(i);
        }
        if (watched == this->rankPokeButton[i])
        {
            if (event->type() == QEvent::MouseButtonPress)
                emit RankPokeClicked(i);
        }
        if (watched == this->rankThumbButton[i])
        {
            if (event->type() == QEvent::MouseButtonPress)
                emit RankThumbClicked(i);
        }
    }

    //watch exchange pokemon buttons
    for (int i = 0; i < MAXSIZE_POKEMON; i++)
    {
        if (watched == this->exButton[i])
        {
            if (event->type() == QEvent::MouseButtonPress)
            {
               emit ExButtonClicked(watched->parent(), i);
            }
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

    //get player's information form server and display
    if (symbol == "myinfo")
    {
        std::string name = recvJ["name"];
        int pokeNum = recvJ["pokemonNumber"];
        int rank = recvJ["rank"];
        int thumb = recvJ["thumb"];
        double rate = recvJ["rate"];
        rate *= 100;
        std::setprecision(3);
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
        stream << std::setprecision(3) << rate;
        stream >> rateStr;
        rateStr.append("%");
        std::string textString = "          " + name + "\n"
                + "Pokemon Number:" + pokeNumStr + "\n"
                + "Rank:" + rankStr + "  Rate:" + rateStr + "\n"
                + "Thumb Number:" + thumbStr + "\n"
                + "Game  Time:" + gametime + "\n"
                + "Begin From:\n" + "   " + begintime;
        emit SetMyInfoSignal(pokeNum, rank, rate, QString::fromStdString(textString));
    }

    //get player's pokemon information and display
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

    //get rank information and display
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
            std::string name = recvJ[nameKey];
            rankPlayerNames[i] = name;
            std::string rank;
            std::string rate;
            stream.clear();
            stream << recvJ[rankKey];
            stream >> rank;
            stream.clear();
            double r = recvJ[rateKey];
            r *= 100;
            stream << std::setprecision(3) << r;
            stream >> rate;
            std::string infoStr;
            std::string tmpStr;
            stream.clear();
            stream << std::setw(6) << std::setfill('*') << name;
            stream >> tmpStr;
            infoStr.append(tmpStr);
            stream.clear();
            stream << std::setw(7) << std::setfill('*') << (rate + "%");
            stream >> tmpStr;
            infoStr.append(tmpStr);
            stream.clear();
            stream << std::setw(7) << std::setfill('*') << rank;
            stream >> tmpStr;
            infoStr.append(tmpStr);
            QString str = QString::fromStdString(infoStr);
            item = new QListWidgetItem(str, this->ui->rankList);
            item->setFont(QFont("Consolas", 16, 2, false));
            emit SetRankIconSignal(i);
        }
    }

    //get package or storage information and display
    if (symbol == "package" || symbol == "storage")
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
            if (symbol == "package")
                packPokemon.push_back(name);
            if (symbol == "storage")
                stoPokemon.push_back(name);
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
            emit SetPackegeScrollAreaSignal(QString::fromStdString(symbol),
                                            QString::fromStdString(kind),
                                            QString::fromStdString(name),
                                            QString::fromStdString(tooltipStd),
                                            i);
        }
    }

    //get take out pokemon form package information and display
    if (symbol == "packout")
    {
        emit ExchangePokeSignal("packout", true);
    }

    //get take out pokemon from storage information and display
    if (symbol == "stoout")
    {
        bool success = recvJ["success"];
        emit ExchangePokeSignal("stoout", success);
    }
    return true;
}

void MainPage::RecvAndSendOnlinePlayer(json j)
{
    //transfer information from json to std::string
    std::string str = j.dump();

    //send to server
    std::thread mainpageSendThread = std::thread(SendThreadFuncMainpage, socketClient, &str);
    mainpageSendThread.join();

    //recv from server
    std::thread mainpageRecvThread = std::thread(RecvThreadFuncMainpage, socketClient, this);
    mainpageRecvThread.join();
    return;
}

void MainPage::onOnlinePlayerClicked()
{
    //close all other items when player looking through online player
    this->ui->readmeWidget->hide();
    this->ui->OPListWidget->clear();
    this->ui->rankWidget->hide();
    this->ui->rankButton->setGeometry(170, 410, 48, 48);
    this->ui->myinfoWidget->hide();
    this->ui->myInfoButton->setGeometry(380, 410, 48, 48);
    this->ui->myPokemonContainer->hide();
    emit ClearScrollAreaSignal("both");
    this->ui->packageButton->setGeometry(490, 410, 48, 48);
    this->ui->myStorageContainer->hide();
    this->ui->storageButton->setGeometry(622, 410, 48, 48);

    //open online player widget and notice server when it is closed
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

    //hide online player widget when it is already open
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
    //clear online player widget, notice server and reload the whole widget
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
    //show rank player's pokemon when the corresponding widget is closed
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

    //hide widget if it is already open
    else
    {
        this->ui->pokeTableContainer->hide();
    }
    return;
}

void MainPage::onPlayerThumbClicked(int i)
{
    //notice server which player is given a thumb
    json j;
    j["symbol"] = "thumb";
    j["name"] = playerNames[i];
    j["end"] = "end";
    RecvAndSendOnlinePlayer(j);
    return;
}

void MainPage::onMyInfoClicked()
{
    //hide all other items when player's information is chosen to look through
    emit ClearScrollAreaSignal("both");
    this->ui->readmeWidget->hide();
    this->ui->myinfoText->clear();
    this->ui->rankWidget->hide();
    this->ui->rankButton->setGeometry(170, 410, 48, 48);
    this->ui->listWidgetContainer->hide();
    this->ui->onlinePlayerBtn->setGeometry(270, 410, 48, 48);
    this->ui->myPokemonContainer->hide();
    this->ui->packageButton->setGeometry(490, 410, 48, 48);
    this->ui->myStorageContainer->hide();
    this->ui->storageButton->setGeometry(622, 410, 48, 48);

    //show player's information widget when it is closed
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

    //hide when widget is already open
    else
    {
        this->ui->myInfoButton->setGeometry(380, 410, 48, 48);
        this->ui->myinfoWidget->hide();
    }
    return;
}

void MainPage::onRankClicked()
{
    //hide all other items when player looking through rank information
    this->ui->readmeWidget->hide();
    this->ui->myinfoWidget->hide();
    this->ui->myInfoButton->setGeometry(380, 410, 48, 48);
    this->ui->listWidgetContainer->hide();
    this->ui->onlinePlayerBtn->setGeometry(270, 410, 48, 48);
    this->ui->myPokemonContainer->hide();
    emit ClearScrollAreaSignal("both");
    this->ui->packageButton->setGeometry(490, 410, 48, 48);
    this->ui->myStorageContainer->hide();
    this->ui->storageButton->setGeometry(622, 410, 48, 48);

    //initialize rank widget before displaying
    for (int i = 0; i < MAXSIZE_PLAYER; i++)
    {
        headLabel[i]->hide();
        playerPokeButton[i]->hide();
        thumbButton[i]->hide();
    }

    //show rank widget when it is closed
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

    //hide when widget is already open
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
    //show rank player pokemon widget when it is closed
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

    //hide when widget is already open
    else
    {
        this->ui->pokeTableContainer->hide();
    }
    return;
}

void MainPage::onRankThumbClicked(int i)
{
    //notice server which player is given a thumb
    json j;
    j["symbol"] = "thumb";
    j["name"] = rankPlayerNames[i];
    j["end"] = "end";
    RecvAndSendOnlinePlayer(j);
    return;
}

void MainPage::onPackageClicked()
{
    //clear package widget before displaying and initialzie variable
    emit ClearScrollAreaSignal("package");
    packPokemon.clear();

    //hide all other items when player looking through package
    this->ui->readmeWidget->hide();
    this->ui->myinfoWidget->hide();
    this->ui->myInfoButton->setGeometry(380, 410, 48, 48);
    this->ui->listWidgetContainer->hide();
    this->ui->onlinePlayerBtn->setGeometry(270, 410, 48, 48);
    this->ui->rankWidget->hide();
    this->ui->rankButton->setGeometry(170, 410, 48, 48);
    this->ui->myStorageContainer->hide();
    this->ui->storageButton->setGeometry(622, 410, 48, 48);

    //show package widget when it is closed or exchang button is clicked
    if (this->ui->myPokemonContainer->isHidden() || exClicked)
    {
        if (exClicked)
            exClicked = false;
        json j;
        j["symbol"] = "package";
        j["name"] = socketClient->getPlayerName();
        j["end"] = "end";
        RecvAndSendOnlinePlayer(j);
        this->ui->myPokemonContainer->show();
        this->ui->packageButton->setGeometry(478, 398, 72, 72);
    }

    //hide when widget is already open and exchange button is not clicked
    else
    {
        this->ui->myPokemonContainer->hide();
        this->ui->packageButton->setGeometry(490, 410, 48, 48);
    }
    return;
}

void MainPage::onUpDownClicked()
{
    //if battle choice widget is hiden, show it
    if (this->ui->battleChoice->height() == 0)
    {
        this->ui->updownButton->setStyleSheet("#updownButton{border-image: url(:/up)}");
        this->ui->updownButton->setToolTip("Click to close");
        QPropertyAnimation* animation = new QPropertyAnimation(this->ui->battleChoice, "size");
        animation->setStartValue(QSize(274, 0));
        animation->setEndValue(QSize(274, 204));
        animation->setDuration(250);
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    }

    //if battle choice widget is showed, hide it
    if (this->ui->battleChoice->height() == 204)
    {
        this->ui->updownButton->setStyleSheet("#updownButton{border-image: url(:/down)}");
        this->ui->updownButton->setToolTip("Click to open");
        QPropertyAnimation* animation = new QPropertyAnimation(this->ui->battleChoice, "size");
        animation->setStartValue(QSize(274, 204));
        animation->setEndValue(QSize(274, 0));
        animation->setDuration(250);
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    }
}

void MainPage::onStorageClicked()
{
    //clear storage widget and variable before display
    emit ClearScrollAreaSignal("storage");
    stoPokemon.clear();

    //hide all other items before displaying
    this->ui->readmeWidget->hide();
    this->ui->myinfoWidget->hide();
    this->ui->myInfoButton->setGeometry(380, 410, 48, 48);
    this->ui->listWidgetContainer->hide();
    this->ui->onlinePlayerBtn->setGeometry(270, 410, 48, 48);
    this->ui->myPokemonContainer->hide();
    this->ui->packageButton->setGeometry(490, 410, 48, 48);
    this->ui->rankWidget->hide();
    this->ui->rankButton->setGeometry(170, 410, 48, 48);

    //show if widget is hiden or exchange button is clicked
    if (this->ui->myStorageContainer->isHidden() || exClicked)
    {
        if (exClicked)
            exClicked = false;
        json j;
        j["symbol"] = "storage";
        j["name"] = socketClient->getPlayerName();
        j["end"] = "end";
        RecvAndSendOnlinePlayer(j);
        this->ui->myStorageContainer->show();
        this->ui->storageButton->setGeometry(598, 386, 72, 72);
    }

    //hide if widget is already open and exchange button is not clicked
    else
    {
        this->ui->myStorageContainer->hide();
        this->ui->storageButton->setGeometry(622, 410, 48, 48);
    }
}

void MainPage::onExButtonClicked(QObject *obj, int i)
{
    //get the index of pokemon in container
    delHLayIndex = i;
    json j;
    j["end"] = "end";
    j["owner"] = socketClient->getPlayerName();

    //ensure which pokemon is chosen
    if (obj == this->ui->scrollWidget)
    {
        j["symbol"] = "packout";
        j["name"] = packPokemon[i];
        RecvAndSendOnlinePlayer(j);
    }
    if (obj == this->ui->storageScrollWidget)
    {
        j["symbol"] = "stoout";
        j["name"] = stoPokemon[i];
        RecvAndSendOnlinePlayer(j);
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
