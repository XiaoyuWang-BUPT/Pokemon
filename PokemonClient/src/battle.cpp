#include "battle.h"
#include "ui_battle.h"
#include <chrono>

Battle::Battle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Battle)
{
    ui->setupUi(this);
    this->InitUI();
    this->InitConnect();
}

void Battle::InitUI()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->ui->container->setStyleSheet("#container{border-image : url(:/forest3);}");
}

void Battle::InitConnect()
{
    QObject::connect(this, SIGNAL(battleNotQualified()), this, SLOT(onBattleNotQualified()));
    QObject::connect(this, SIGNAL(WinOrLoseSignal(bool, int)), this, SLOT(WinOrLose(bool, int)));
    QObject::connect(this, SIGNAL(MyPokeAttackSignal(QString, QString)), this, SLOT(onMyPokeAttack(QString, QString)));
    QObject::connect(this, SIGNAL(EnemyPokeAttackSignal(QString, QString)), this, SLOT(onEnemyPokeAttack(QString, QString)));
    QObject::connect(this, SIGNAL(MyPokeBeAttacked(int)), this, SLOT(onMyPokeBeAttacked(int)));
    QObject::connect(this, SIGNAL(EnemyPokeBeAttacked(int)), this, SLOT(onEnemyPokeBeAttacked(int)));
    QObject::connect(this, SIGNAL(MyPokeHurtSignal(int, QString)), this, SLOT(onMyPokeHurt(int, QString)));
    QObject::connect(this, SIGNAL(EnemyPokeHurtSignal(int, QString)), this, SLOT(onEnemyPokeHurt(int, QString)));
    QObject::connect(this, SIGNAL(MyPokeChangeSignal(int)), this, SLOT(onMyPokeChange(int)));
    QObject::connect(this, SIGNAL(EnemyPokeChangeSignal(int)), this, SLOT(onEnemyPokeChange(int)));
    QObject::connect(this, SIGNAL(ClearHurtSignal()), this, SLOT(ClearHurt()));
    QObject::connect(this, SIGNAL(ClearSpecialAttSignal()), this, SLOT(ClearSpecialAtt()));
    QObject::connect(this->ui->confirmButton, SIGNAL(clicked(bool)), this, SLOT(ClearBattle()));
    QObject::connect(this, SIGNAL(ClearHPDecSignal()), this, SLOT(ClearHPDec()));
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

DWORD WINAPI SendAndRecvThreadFuncBattle(Battle* battle);
DWORD WINAPI SendThreadFuncBattle(SocketClient* socketClient, LPVOID sParam);
DWORD WINAPI RecvThreadFuncBattle(SocketClient* socketClient, Battle* battle);

void Battle::receiveSwitch(QString nature, QString standard)
{
    //initialize result widget when receive switch signal
    for (int i = 0; i < CAPACITY; i++)
    {
        endPokeLabel[i] = new QLabel();
        endInfoLabel[i] = new QLabel();
        endVLayout[i] = new QVBoxLayout();
    }

    //hide all ui items taking over animation when this page is loaded
    this->ui->myHPDecLabel->hide();
    this->ui->enemyHPDecLabel->hide();
    this->ui->confirmButton->hide();
    this->ui->winorloseLabel->hide();
    this->ui->myPokeHPBarWidget->resize(170, 20);
    this->ui->enemyPokeHPBarWidget->resize(170, 20);

    //initialize all variables when this page is loaded
    myPokemonName.clear();
    myPokemonKind.clear();
    myEvoKind.clear();
    myPokemonTHP.clear();
    enemyPokemonName.clear();
    enemyPokemonKind.clear();
    enemyPokemonTHP.clear();
    myPokeIndex = 0;
    enemyPokeIndex = 0;
    natureStd = "";
    standardStd = "";

    //display after ui items and variables are intialized
    this->show();

    //get boot's nature player chosen
    natureStd = nature.toStdString();

    //get boot's standard player chosen
    standardStd = standard.toStdString();

    //send battle request in a new thread
    std::thread* sendAndRecvThread = new std::thread(SendAndRecvThreadFuncBattle, this);
    sendAndRecvThread->detach();
}

void Battle::ClearBattle()
{
    //the number of ui items in result widget is at most 3
    //result widget items are cleared
    for (int i = 0; i < CAPACITY; i++)
    {
        endVLayout[i]->removeWidget(endPokeLabel[i]);
        endVLayout[i]->removeWidget(endInfoLabel[i]);
        endHLayout->removeItem(endVLayout[i]);
        delete endPokeLabel[i];
        delete endInfoLabel[i];
        delete endVLayout[i];
    }
    //hide result widget
    this->ui->endLayoutContainer->hide();
    this->hide();
    emit switchToMainPage();
}

void Battle::getRecvStr(QString recvStr)
{
    //transfer from QString to std::string
    std::string string = recvStr.toStdString();

    //treansfer fomr std::string to json
    json recvJ = json::parse(string);
    std::string symbol = recvJ["symbol"];

    //player's package is not empty, qualified to battle
    if (symbol == "battle")
    {
        int pokeNum = recvJ["amount"];
        int round = recvJ["round"];
        int roundCnt = 0;
        int expGot = recvJ["expGot"];

        //get player's pokemon crew and boot crew, including their name, kind and HP
        std::stringstream stream;
        std::string iStr;
        std::string pKey;
        for (int i = 0; i < pokeNum; i++)
        {
            stream.clear();
            stream << i;
            stream >> iStr;
            pKey = "mypokename" + iStr;
            myPokemonName.push_back(recvJ[pKey]);
            pKey = "mypokekind" + iStr;
            myPokemonKind.push_back(recvJ[pKey]);
            pKey = "mypokehp" + iStr;
            myPokemonTHP.push_back(recvJ[pKey]);
            pKey = "myevokind" + iStr;
            myEvoKind.push_back(recvJ[pKey]);
            pKey = "enemypokename" + iStr;
            enemyPokemonName.push_back(recvJ[pKey]);
            pKey = "enemypokekind" + iStr;
            enemyPokemonKind.push_back(recvJ[pKey]);
            pKey = "enemypokehp" + iStr;
            enemyPokemonTHP.push_back(recvJ[pKey]);
        }
        std::string roundStr;
        std::string keyStr;
        std::string attackPokemon;

        //first pokemon on both side show on
        int myFightingIndex = 0;
        int enemyFightingIndex = 0;
        emit MyPokeChangeSignal(myFightingIndex);
        emit EnemyPokeChangeSignal(enemyFightingIndex);

        //this thread wait for some time for animation completion
        std::this_thread::sleep_for(std::chrono::microseconds(1250000));

        //get information of each round and emit corresponding signal to animate
        for (; roundCnt < round; roundCnt++)
        {
            stream.clear();
            stream << roundCnt;
            stream >> roundStr;
            keyStr = "round" + roundStr + "attack";
            attackPokemon = recvJ[keyStr];

            //current round is player's turn
            if (attackPokemon == "mypokemon")
            {
                //judge whether player's pokemon is unhealthy and so hurt
                keyStr = "round" + roundStr + "myhurt";
                bool myhurt = recvJ[keyStr];

                //if player's pokemon is hurt, then judge whether it's dead or not
                if (myhurt)
                {
                    //get HP after get hurt and get boot's nature for variant pics
                    keyStr = "round" + roundStr + "myhurthp";
                    int myhurthp = recvJ[keyStr];
                    keyStr = "round" + roundStr + "hurtnature";
                    std::string hurtNature = recvJ[keyStr];

                    //this thread wait some time for unhealthy state animation completion
                    emit MyPokeHurtSignal(myhurthp, QString::fromStdString(hurtNature));
                    std::this_thread::sleep_for(std::chrono::microseconds(1250000));

                    //after animation completion, clear all pics
                    emit ClearHurtSignal();
                    emit ClearHPDecSignal();
                    //my fighting pokemon is hurt to dead, change another pokemon
                    if (myhurthp == 0)
                    {              
                        myFightingIndex++;
                        //change player's pokemon if they are not all dead
                        if (myFightingIndex < pokeNum)
                        {
                            //load change pokemon animation and this thread wait some time for animation completion
                            emit MyPokeChangeSignal(myFightingIndex);
                            std::this_thread::sleep_for(std::chrono::microseconds(1250000));
                        }

                        //turn to result, which is defeat
                        else
                        {
                            emit WinOrLoseSignal(false, expGot);
                        }
                    }

                    //player's pokemon attack if it's not dead
                    else
                    {
                        //get attack way-normal or special, boot pokemon's HP after attacked and nature of player's pokemon
                        keyStr = "round" + roundStr + "attway";
                        std::string attway = recvJ[keyStr];
                        keyStr = "round" + roundStr + "enemypokemonhp";
                        int enemypokehp = recvJ[keyStr];
                        keyStr = "round" + roundStr + "attnature";
                        std::string attNature = recvJ[keyStr];

                        //load player's pokemon attack animation and this thread wait for animation completion
                        emit MyPokeAttackSignal(QString::fromStdString(attway), QString::fromStdString(attNature));
                        std::this_thread::sleep_for(std::chrono::microseconds(1250000));

                        //load boot pokemon be attacked animation and this thread wait for animation completion
                        emit EnemyPokeBeAttacked(enemypokehp);
                        std::this_thread::sleep_for(std::chrono::microseconds(1250000));

                        //after animation completion, clear items
                        emit ClearHPDecSignal();
                        if (attway == "specialatt")
                            emit ClearSpecialAttSignal();

                        //judge whether boot pokemon is attacked to dead
                        if (enemypokehp == 0)
                        {
                            enemyFightingIndex++;

                            //change boot pokemon if they are not all dead
                            if (enemyFightingIndex < pokeNum)
                            {
                                //load change boot pokemon animation and this thread wait some time for animation completion
                                emit EnemyPokeChangeSignal(enemyFightingIndex);
                                std::this_thread::sleep_for(std::chrono::microseconds(1250000));
                            }

                            //turn to result-win, if boot pokemon all dead
                            else
                            {
                                emit WinOrLoseSignal(true, expGot);
                            }
                        }
                    }
                }

                //player's pokemon is healthy
                else
                {
                    keyStr = "round" + roundStr + "attway";
                    std::string attway = recvJ[keyStr];
                    keyStr = "round" + roundStr + "enemypokemonhp";
                    int enemypokehp = recvJ[keyStr];
                    keyStr = "round" + roundStr + "attnature";
                    std::string attNature = recvJ[keyStr];
                    emit MyPokeAttackSignal(QString::fromStdString(attway), QString::fromStdString(attNature));
                    std::this_thread::sleep_for(std::chrono::microseconds(1250000));
                    emit EnemyPokeBeAttacked(enemypokehp);
                    std::this_thread::sleep_for(std::chrono::microseconds(1250000));
                    emit ClearHPDecSignal();
                    if (attway == "specialatt")
                        emit ClearSpecialAttSignal();
                    if (enemypokehp == 0)
                    {
                        enemyFightingIndex++;
                        if (enemyFightingIndex < pokeNum)
                        {
                            emit EnemyPokeChangeSignal(enemyFightingIndex);
                            std::this_thread::sleep_for(std::chrono::microseconds(1250000));
                        }
                        else
                        {
                            //TODO win
                            emit WinOrLoseSignal(true, expGot);
                        }
                    }
                }
            }

            //current round is boot's turn
            if (attackPokemon == "enemypokemon")
            {
                //judge whether boot pokemon is unhealthy
                keyStr = "round" + roundStr + "enemyhurt";
                bool enemyhurt = recvJ[keyStr];

                //boot pokemon is unhealthy
                if (enemyhurt)
                {
                    //get boot pokemon HP after hurt and nature of player's pokemon
                    keyStr = "round" + roundStr + "enemyhurthp";
                    int enemyhurthp = recvJ[keyStr];
                    keyStr = "round" + roundStr + "hurtnature";
                    std::string hurtNature = recvJ[keyStr];

                    //load hurt animation and this thread wait some time for animation completion
                    emit EnemyPokeHurtSignal(enemyhurthp, QString::fromStdString(hurtNature));
                    std::this_thread::sleep_for(std::chrono::microseconds(1250000));

                    //clear animation items after animation completion
                    emit ClearHurtSignal();
                    emit ClearHPDecSignal();

                    //boot pokemon is hurt to dead
                    if (enemyhurthp == 0)
                    {
                        enemyFightingIndex++;
                        if (enemyFightingIndex < pokeNum)
                        {
                            //load change boot pokemon animation and this thread wait some time for animation completion
                            emit EnemyPokeChangeSignal(enemyFightingIndex);
                            std::this_thread::sleep_for(std::chrono::microseconds(1250000));
                        }

                        //boot pokemon are all dead
                        else
                        {
                            //turn to result-win
                            emit WinOrLoseSignal(true, expGot);
                        }
                    }

                    //boot pokemon is not hurt to dead
                    else
                    {
                        //boot pokemon attack, get attack way, player's pokemon HP after be attacked and nature of boot pokemon
                        keyStr = "round" + roundStr + "attway";
                        std::string attway = recvJ[keyStr];
                        keyStr = "round" + roundStr + "mypokemonhp";
                        int mypokehp = recvJ[keyStr];
                        keyStr = "round" + roundStr + "attnature";
                        std::string attNature = recvJ[keyStr];

                        //load boot pokemon attack animation and this thread wait some time for animation completion
                        emit EnemyPokeAttackSignal(QString::fromStdString(attway), QString::fromStdString(attNature));
                        std::this_thread::sleep_for(std::chrono::microseconds(1250000));

                        //load player's pokemon being attacked animation and this thread wait some time for animation completion
                        emit MyPokeBeAttacked(mypokehp);
                        std::this_thread::sleep_for(std::chrono::microseconds(1250000));

                        //clear animtion items after animation completion
                        emit ClearHPDecSignal();
                        if (attway == "specialatt")
                            emit ClearSpecialAttSignal();

                        //player's pokemon is attacked to dead
                        if (mypokehp == 0)
                        {
                            myFightingIndex++;

                            //player's pokemon are not all dead
                            if (myFightingIndex < pokeNum)
                            {
                                //load change player's pokemon animation and this thread wait some time for animation completion
                                emit MyPokeChangeSignal(myFightingIndex);
                                std::this_thread::sleep_for(std::chrono::microseconds(1250000));
                            }
                            //player's pokemon are all dead
                            else
                            {
                                //turn to result-lose
                                emit WinOrLoseSignal(false, expGot);
                            }
                        }
                    }
                }

                //boot pokemon is not hurt
                else
                {
                    keyStr = "round" + roundStr + "attway";
                    std::string attway = recvJ[keyStr];
                    keyStr = "round" + roundStr + "mypokemonhp";
                    int mypokehp = recvJ[keyStr];
                    keyStr = "round" + roundStr + "attnature";
                    std::string attNature = recvJ[keyStr];
                    emit EnemyPokeAttackSignal(QString::fromStdString(attway), QString::fromStdString(attNature));
                    std::this_thread::sleep_for(std::chrono::microseconds(1250000));
                    emit MyPokeBeAttacked(mypokehp);
                    std::this_thread::sleep_for(std::chrono::microseconds(1250000));
                    emit ClearHPDecSignal();
                    if (attway == "specialatt")
                        emit ClearSpecialAttSignal();
                    if (mypokehp == 0)
                    {
                        myFightingIndex++;
                        if (myFightingIndex < pokeNum)
                        {
                            emit MyPokeChangeSignal(myFightingIndex);
                            std::this_thread::sleep_for(std::chrono::microseconds(1250000));
                        }
                        else
                        {
                            //TODO lose
                            emit WinOrLoseSignal(false, expGot);
                        }
                    }
                }
            }
        }
    }

    //player's package is empty, not qualified to battle
    if (symbol == "nobattle")
    {
        emit battleNotQualified();
    }
}

void Battle::onBattleNotQualified()
{
    //battle not qualified, inform player and release resource
    QMessageBox::information(this, "info", "package empty, put pokemon into package first");
    delete[] endPokeLabel;
    delete[] endVLayout;
    this->hide();
    emit switchToMainPage();
}

void Battle::WinOrLose(bool win, int expGot)
{
    //get experience pokemon got and result-win or lose
    std::stringstream stream;
    std::string expGotStr;
    stream << expGot;
    stream >> expGotStr;
    QString expGotQStr = QString::fromStdString(expGotStr);

    //clear items' content before display
    this->ui->myPokeInfoLabel->clear();
    this->ui->myPokeLabel->clear();
    this->ui->enemyPokeInfoLabel->clear();
    this->ui->enemyPokeLabel->clear();
    this->ui->confirmButton->show();
    this->ui->winorloseLabel->show();

    //there is a layout for each of player's pokemon
    int pokeNum = myPokemonName.size();
    for (int i = 0; i < pokeNum; i++)
    {
        //display experience pokemon gained
        QString tooltip = "Pokemon gained " + expGotQStr + " experience";

        //display whether player's pokemon evolved and evolved to what
        if (QString::fromStdString(myPokemonKind[i]).toLower()
                == QString::fromStdString(myEvoKind[i]).toLower())
        {
            tooltip.append("\nPokemon did not evolute");
        }
        else
        {
            tooltip.append("\nPokemon evoluted to\n" +
                           QString::fromStdString(myEvoKind[i]).toUpper());
        }
        QString pixmapStr = ":/" + QString::fromStdString(myEvoKind[i]).toLower();
        QPixmap pixmap = QPixmap(pixmapStr);

        //set layout
        endPokeLabel[i]->setScaledContents(true);
        endPokeLabel[i]->setMinimumSize(150, 150);
        endPokeLabel[i]->setMaximumSize(150, 150);
        endPokeLabel[i]->setPixmap(pixmap);
        endInfoLabel[i]->setMinimumSize(150, 100);
        endInfoLabel[i]->setMaximumSize(150, 100);
        endInfoLabel[i]->setText(tooltip);
        endInfoLabel[i]->setWordWrap(true);
        endPokeLabel[i]->setAlignment(Qt::AlignCenter);
        endInfoLabel[i]->setAlignment(Qt::AlignCenter);
        endPokeLabel[i]->setStyleSheet("border-top : 2px solid white; border-radius : 5px; background-color : rgba(255, 255, 255, 100)");
        endInfoLabel[i]->setStyleSheet("border : 2px solid white; border-radius : 5px; background-color : rgba(200, 200, 200, 150); font: 75 11pt Consolas; color : rgb(255, 255, 255)");
        endVLayout[i]->addWidget(endPokeLabel[i]);
        endVLayout[i]->addWidget(endInfoLabel[i]);
        endHLayout->addLayout(endVLayout[i]);
    }
    this->ui->endLayoutContainer->setLayout(endHLayout);
    if (win)
    {        
        this->ui->winorloseLabel->setText("VICTORY");
    }
    else
    {        
        this->ui->winorloseLabel->setText("DEFEAT");
    }
    this->ui->endLayoutContainer->show();
}

void Battle::onMyPokeAttack(QString attway, QString myNature)
{
    //display normal-attack animation
    if (attway == "att")
    {
        QPropertyAnimation* animation = new QPropertyAnimation(this->ui->myPoke, "geometry");
        animation->setStartValue(QRect(30, 160, 200, 200));
        animation->setKeyValueAt(0.5, QRect(560, 160, 200, 200));
        animation->setEndValue(QRect(30, 160, 200, 200));
        animation->setDuration(2000);
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    }

    //display special-attack animation
    if (attway == "specialatt")
    {
        if (myNature.toLower() == "fire")
            this->ui->mySpecialAttLabel->setPixmap(QPixmap(":/myfireatt"));
        if (myNature.toLower() == "water")
            this->ui->mySpecialAttLabel->setPixmap(QPixmap(":/mywateratt"));
        if (myNature.toLower() == "bush")
            this->ui->mySpecialAttLabel->setPixmap(QPixmap(":/mybushatt"));
        if (myNature.toLower() == "electricity")
            this->ui->mySpecialAttLabel->setPixmap(QPixmap(":/myelectricityatt"));
        this->ui->mySpecialAttLabel->show();
        QPropertyAnimation* animation = new QPropertyAnimation(this->ui->mySpecialAttLabel, "geometry");
        animation->setStartValue(QRect(190, 250, 60, 30));
        animation->setEndValue(QRect(590, 250, 60, 30));
        animation->setDuration(2000);
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    }
}

void Battle::onEnemyPokeAttack(QString attway, QString enemyNature)
{
    //display normal-attack animation
    if (attway == "att")
    {
        QPropertyAnimation* animation = new QPropertyAnimation(this->ui->enemyPoke, "geometry");
        animation->setStartValue(QRect(630, 160, 200, 200));
        animation->setKeyValueAt(0.5, QRect(30, 160, 200, 200));
        animation->setEndValue(QRect(630, 160, 200, 200));
        animation->setDuration(2000);
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    }

    //display special-attack animation
    if (attway == "specialatt")
    {
        if (enemyNature.toLower() == "fire")
            this->ui->enemySpecialAttLabel->setPixmap(QPixmap(":/enemyfireatt"));
        if (enemyNature.toLower() == "water")
            this->ui->enemySpecialAttLabel->setPixmap(QPixmap(":/enemywateratt"));
        if (enemyNature.toLower() == "bush")
            this->ui->enemySpecialAttLabel->setPixmap(QPixmap(":/enemybushatt"));
        if (enemyNature.toLower() == "electricity")
            this->ui->enemySpecialAttLabel->setPixmap(QPixmap(":/enemyelectricityatt"));
        this->ui->enemySpecialAttLabel->show();
        QPropertyAnimation* animation = new QPropertyAnimation(this->ui->enemySpecialAttLabel, "geometry");
        animation->setStartValue(QRect(540, 250, 60, 30));
        animation->setEndValue(QRect(100, 250, 60, 30));
        animation->setDuration(2000);
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    }
}

void Battle::onMyPokeBeAttacked(int mypokehp)
{
    //display HP-decreasing of player's pokemon
    this->ui->myHPDecLabel->show();
    int HPDec = myPokeCurrentHP - mypokehp;
    myPokeCurrentHP = mypokehp;
    std::stringstream stream;
    std::string hp;
    stream << HPDec;
    stream >> hp;
    this->ui->myHPDecLabel->setText("-" + QString::fromStdString(hp));
    QPropertyAnimation* hpAnimation = new QPropertyAnimation(this->ui->myHPDecLabel, "geometry");
    hpAnimation->setStartValue(QRect(110, 190, 80, 30));
    hpAnimation->setEndValue(QRect(100, 140, 100, 50));
    hpAnimation->setDuration(1000);
    hpAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    double width = this->ui->myPokeHPBarWidget->width();
    double startWidth = width * myPokeCurrentHP / myPokemonTHP[myPokeIndex];
    double endWidth = width * mypokehp / myPokemonTHP[myPokeIndex];
    myPokeCurrentHP = mypokehp;
    QPropertyAnimation* animation = new QPropertyAnimation(this->ui->myPokeHPBarWidget, "size");
    animation->setStartValue(QSizeF(startWidth, 20));
    animation->setEndValue(QSizeF(endWidth, 20));
    animation->setDuration(1000);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Battle::onEnemyPokeBeAttacked(int enemypokehp)
{
    //display HP-decreasing of boot pokemon
    this->ui->enemyHPDecLabel->show();
    int HPDec = enemyPokeCurrentHP - enemypokehp;
    enemyPokeCurrentHP = enemypokehp;
    std::stringstream stream;
    std::string hp;
    stream << HPDec;
    stream >> hp;
    this->ui->enemyHPDecLabel->setText("-" + QString::fromStdString(hp));
    QPropertyAnimation* hpAnimation = new QPropertyAnimation(this->ui->enemyHPDecLabel, "geometry");
    hpAnimation->setStartValue(QRect(620, 190, 80, 30));
    hpAnimation->setEndValue(QRect(610, 140, 100, 50));
    hpAnimation->setDuration(1000);
    hpAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    double width = this->ui->enemyPokeHPBarWidget->width();
    double startWidth = width * enemyPokeCurrentHP / enemyPokemonTHP[enemyPokeIndex];
    double endWidth = width * enemypokehp / enemyPokemonTHP[enemyPokeIndex];
    enemyPokeCurrentHP = enemypokehp;
    QPropertyAnimation *animation = new QPropertyAnimation(this->ui->enemyPokeHPBarWidget, "size");
    animation->setStartValue(QSizeF(startWidth, 20));
    animation->setEndValue(QSizeF(endWidth, 20));
    animation->setDuration(1000);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Battle::onMyPokeHurt(int myhurthp, QString enemyNature)
{
    //display HP-decreasing of player's pokemon
    this->ui->myHPDecLabel->show();
    int HPDec = myPokeCurrentHP - myhurthp;
    myPokeCurrentHP = myhurthp;
    std::stringstream stream;
    std::string hp;
    stream << HPDec;
    stream >> hp;
    this->ui->myHPDecLabel->setText("-" + QString::fromStdString(hp));
    QPropertyAnimation* hpAnimation = new QPropertyAnimation(this->ui->myHPDecLabel, "geometry");
    hpAnimation->setStartValue(QRect(110, 190, 80, 30));
    hpAnimation->setEndValue(QRect(100, 140, 100, 50));
    hpAnimation->setDuration(1000);
    hpAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    //display hurt pic
    this->ui->myHurtLabel->show();
    QPixmap pixmap;
    if (enemyNature == "Poisoned")
        pixmap = QPixmap(":/poisoned");
    if (enemyNature == "Frozen")
        pixmap = QPixmap(":/shock");
    if (enemyNature == "Burned")
        pixmap = QPixmap(":/burned");
    if (enemyNature == "Drowned")
        pixmap = QPixmap(":/drowned");
    this->ui->myHurtLabel->setPixmap(pixmap);

    //display HP-Bar animation
    double width = this->ui->myPokeHPBarWidget->width();
    double startWidth = width * myPokeCurrentHP / myPokemonTHP[myPokeIndex];
    double endWidth = width * myhurthp / myPokemonTHP[myPokeIndex];
    QPropertyAnimation* barAnimation = new QPropertyAnimation(this->ui->myPokeHPBarWidget, "size");
    barAnimation->setStartValue(QSizeF(startWidth, 20));
    barAnimation->setEndValue(QSizeF(endWidth, 20));
    barAnimation->setDuration(1000);
    barAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Battle::onEnemyPokeHurt(int enemyhurthp, QString myNature)
{
    //display HP-decreasing of boot pokemon
    this->ui->enemyHPDecLabel->show();
    int HPDec = enemyPokeCurrentHP - enemyhurthp;
    enemyPokeCurrentHP = enemyhurthp;
    std::stringstream stream;
    std::string hp;
    stream << HPDec;
    stream >> hp;
    this->ui->enemyHPDecLabel->setText("-" + QString::fromStdString(hp));
    QPropertyAnimation* hpAnimation = new QPropertyAnimation(this->ui->enemyHPDecLabel, "geometry");
    hpAnimation->setStartValue(QRect(620, 190, 80, 30));
    hpAnimation->setEndValue(QRect(610, 140, 100, 50));
    hpAnimation->setDuration(1000);
    hpAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    this->ui->enemyHurtLabel->show();

    //display hurt pic
    QPixmap pixmap;
    if (myNature == "Poisoned")
        pixmap = QPixmap(":/poisoned");
    if (myNature == "Frozen")
        pixmap = QPixmap(":/shock");
    if (myNature == "Burned")
        pixmap = QPixmap(":/burned");
    if (myNature == "Drowned")
        pixmap = QPixmap(":/drowned");
    this->ui->enemyHurtLabel->setPixmap(pixmap);

    //display HP-Bar animation
    double width = this->ui->enemyPokeHPBarWidget->width();
    double startWidth = width * enemyPokeCurrentHP / enemyPokemonTHP[enemyPokeIndex];
    double endWidth = width * enemyhurthp / enemyPokemonTHP[enemyPokeIndex];
    QPropertyAnimation* barAnimation = new QPropertyAnimation(this->ui->enemyPokeHPBarWidget, "size");
    barAnimation->setStartValue(QSizeF(startWidth, 20));
    barAnimation->setEndValue(QSizeF(endWidth, 20));
    barAnimation->setDuration(1000);
    barAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Battle::ClearHurt()
{
    this->ui->myHurtLabel->hide();
    this->ui->enemyHurtLabel->hide();
}

void Battle::ClearSpecialAtt()
{
    this->ui->mySpecialAttLabel->clear();
    this->ui->mySpecialAttLabel->hide();
    this->ui->enemySpecialAttLabel->clear();
    this->ui->enemySpecialAttLabel->hide();
}

void Battle::ClearHPDec()
{
    this->ui->myHPDecLabel->hide();
    this->ui->enemyHPDecLabel->hide();
    this->ui->myHPDecLabel->setGeometry(110, 190, 80, 30);
    this->ui->enemyHPDecLabel->setGeometry(620, 190, 80, 30);
}

void Battle::onMyPokeChange(int mypokeindex)
{
    //resize pokemon HP-Bar to full again
    this->ui->myPokeHPBarWidget->resize(170, 20);

    //reload pokemon pic
    myPokeIndex = mypokeindex;
    std::string kind = myPokemonKind[mypokeindex];
    std::string name = myPokemonName[mypokeindex];
    QString styleStr = "#myPokeLabel{image : url(:/" + QString::fromStdString(kind).toLower() + ")}";
    this->ui->myPokeLabel->setStyleSheet(styleStr);
    this->ui->myPokeInfoLabel->setText(QString::fromStdString(name));
    myPokeCurrentHP = myPokemonTHP[mypokeindex];

    //load disappear and appear animation
    QPropertyAnimation* animation = new QPropertyAnimation(this->ui->myPoke, "geometry");
    animation->setStartValue(QRect(30, 160, 200, 200));
    animation->setKeyValueAt(0.5, QRect(30, 160, 0, 200));
    animation->setEndValue(QRect(30, 160, 200, 200));
    animation->setDuration(1000);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Battle::onEnemyPokeChange(int enemypokeindex)
{
    //resize pokemon HP-Bar to full again
    this->ui->enemyPokeHPBarWidget->resize(170, 20);

    //reload pokemon pic
    enemyPokeIndex = enemypokeindex;
    std::string kind = enemyPokemonKind[enemypokeindex];
    std::string name = enemyPokemonName[enemypokeindex];
    QString styleStr = "#enemyPokeLabel{image : url(:/" + QString::fromStdString(kind).toLower() + ");}";
    this->ui->enemyPokeLabel->setStyleSheet(styleStr);
    this->ui->enemyPokeLabel->setAlignment(Qt::AlignCenter);
    this->ui->enemyPokeInfoLabel->setText(QString::fromStdString(name));
    enemyPokeCurrentHP = enemyPokemonTHP[enemypokeindex];

    //load disappear and appear animation
    QPropertyAnimation* animation = new QPropertyAnimation(this->ui->enemyPoke, "geometry");
    animation->setStartValue(QRect(630, 160, 200, 200));
    animation->setKeyValueAt(0.5, QRect(630, 160, 0, 200));
    animation->setEndValue(QRect(630, 160, 200, 200));
    animation->setDuration(1000);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

//recv form server in a new thraed
DWORD WINAPI RecvThreadFuncBattle(SocketClient* socketClient, Battle* battle)
{
    //connect socket of socket client
    SOCKET ConnectSocket = socketClient->getConnectSocket();

    //clear recvbuf to receive
    socketClient->ClearRecvBuf();
    socketClient->iResult = recv(ConnectSocket, socketClient->recvbuf, socketClient->recvbuflen, 0);

    //call getRecvStr function if recv succeed
    if (socketClient->iResult > 0)
    {
        QMetaObject::invokeMethod(battle, "getRecvStr", Qt::DirectConnection, Q_ARG(QString, QString::fromStdString(socketClient->GetRecvStr())));
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

//send to server in a new thraed
DWORD WINAPI SendThreadFuncBattle(SocketClient* socketClient, LPVOID sParam)
{
    std::string *sendStr = (std::string*)sParam;
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

DWORD WINAPI SendAndRecvThreadFuncBattle(Battle* battle)
{
    std::string nature = battle->getNatureStd();
    std::string standard = battle->getStandardStd();
    json j;
    j["symbol"] = "battle";
    j["nature"] = nature;
    j["standard"] = standard;
    j["name"] = battle->socketClient->getPlayerName();
    j["end"] = "end";
    std::string str = j.dump();
    std::thread battleSendThread = std::thread(SendThreadFuncBattle, battle->socketClient, &str);
    battleSendThread.join();
    std::thread battleRecvThread = std::thread(RecvThreadFuncBattle, battle->socketClient, battle);
    battleRecvThread.join();
    return 0;
}
