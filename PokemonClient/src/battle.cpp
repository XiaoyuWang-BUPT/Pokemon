#include "battle.h"
#include "ui_battle.h"
#include <chrono>

Battle::Battle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Battle)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->ui->confirmButton->installEventFilter(this);
    this->ui->container->setStyleSheet("#container{border-image : url(:/forest3);}");

    QObject::connect(this, SIGNAL(battleNotQualified()), this, SLOT(onBattleNotQualified()));
    QObject::connect(this, SIGNAL(WinOrLoseSignal(bool)), this, SLOT(winOrLose(bool)));
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
    this->ui->confirmButton->hide();
    this->ui->winorloseLabel->hide();
    this->ui->myPokeHPBarWidget->resize(170, 20);
    this->ui->enemyPokeHPBarWidget->resize(170, 20);
    myPokemonName.clear();
    myPokemonKind.clear();
    myPokemonTHP.clear();
    enemyPokemonName.clear();
    enemyPokemonKind.clear();
    enemyPokemonTHP.clear();
    myPokeIndex = 0;
    enemyPokeIndex = 0;
    natureStd = "";
    standardStd = "";
    this->show();
    natureStd = nature.toStdString();
    standardStd = standard.toStdString();
    std::thread* sendAndRecvThread = new std::thread(SendAndRecvThreadFuncBattle, this);
    sendAndRecvThread->detach();
}

void Battle::getRecvStr(QString recvStr)
{
    std::string string = recvStr.toStdString();
    json recvJ = json::parse(string);
    std::string symbol = recvJ["symbol"];
    if (symbol == "battle")
    {
        int pokeNum = recvJ["amount"];
        int round = recvJ["round"];
        int roundCnt = 0;
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
        int myFightingIndex = 0;
        int enemyFightingIndex = 0;
        emit MyPokeChangeSignal(myFightingIndex);
        emit EnemyPokeChangeSignal(enemyFightingIndex);
        std::this_thread::sleep_for(std::chrono::microseconds(1250000));
        for (; roundCnt < round; roundCnt++)
        {
            stream.clear();
            stream << roundCnt;
            stream >> roundStr;
            keyStr = "round" + roundStr + "attack";
            attackPokemon = recvJ[keyStr];
            if (attackPokemon == "mypokemon")
            {
                keyStr = "round" + roundStr + "myhurt";
                bool myhurt = recvJ[keyStr];
                if (myhurt)
                {
                    keyStr = "round" + roundStr + "myhurthp";
                    int myhurthp = recvJ[keyStr];
                    keyStr = "round" + roundStr + "hurtnature";
                    std::string hurtNature = recvJ[keyStr];
                    emit MyPokeHurtSignal(myhurthp, QString::fromStdString(hurtNature));
                    std::this_thread::sleep_for(std::chrono::microseconds(1250000));
                    emit ClearHurtSignal();
                    //my fighting pokemon is hurt to dead change another pokemon
                    if (myhurthp == 0)
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
                            emit WinOrLoseSignal(false);
                        }
                    }
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
                                emit WinOrLoseSignal(true);
                            }
                        }
                    }
                }
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
                            emit WinOrLoseSignal(true);
                        }
                    }
                }
            }
            if (attackPokemon == "enemypokemon")
            {
                keyStr = "round" + roundStr + "enemyhurt";
                bool enemyhurt = recvJ[keyStr];
                if (enemyhurt)
                {
                    keyStr = "round" + roundStr + "enemyhurthp";
                    int enemyhurthp = recvJ[keyStr];
                    keyStr = "round" + roundStr + "hurtnature";
                    std::string hurtNature = recvJ[keyStr];
                    emit EnemyPokeHurtSignal(enemyhurthp, QString::fromStdString(hurtNature));
                    std::this_thread::sleep_for(std::chrono::microseconds(1250000));
                    emit ClearHurtSignal();
                    if (enemyhurthp == 0)
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
                            emit WinOrLoseSignal(true);
                        }
                    }
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
                                emit WinOrLoseSignal(false);
                            }
                        }
                    }
                }
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
                            emit WinOrLoseSignal(false);
                        }
                    }
                }
            }
        }
    }
    if (symbol == "nobattle")
    {
        emit battleNotQualified();
    }
}

bool Battle::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->ui->confirmButton)
        if (event->type() == QEvent::MouseButtonPress)
        {
            this->hide();
            emit switchToMainPage();
        }
    return QWidget::eventFilter(watched, event);
}

void Battle::onBattleNotQualified()
{
    QMessageBox::information(this, "info", "package empty, put pokemon into package first");
    this->hide();
    emit switchToMainPage();
}

void Battle::winOrLose(bool win)
{
    this->ui->myPokeInfoLabel->clear();
    this->ui->myPokeLabel->clear();
    this->ui->enemyPokeInfoLabel->clear();
    this->ui->enemyPokeLabel->clear();
    this->ui->confirmButton->show();
    this->ui->winorloseLabel->show();
    if (win)
    {        
        this->ui->winorloseLabel->setText("VICTORY");
        //QMessageBox::information(this, "info", "win");
    }
    else
    {        
        this->ui->winorloseLabel->setText("DEFEAT");
        //QMessageBox::information(this, "info", "lose");
    }
}

void Battle::onMyPokeAttack(QString attway, QString myNature)
{
    if (attway == "att")
    {
        QPropertyAnimation* animation = new QPropertyAnimation(this->ui->myPoke, "geometry");
        animation->setStartValue(QRect(30, 160, 200, 200));
        animation->setKeyValueAt(0.5, QRect(560, 160, 200, 200));
        animation->setEndValue(QRect(30, 160, 200, 200));
        animation->setDuration(2000);
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    }
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
    if (attway == "att")
    {
        QPropertyAnimation* animation = new QPropertyAnimation(this->ui->enemyPoke, "geometry");
        animation->setStartValue(QRect(560, 160, 200, 200));
        animation->setKeyValueAt(0.5, QRect(30, 160, 200, 200));
        animation->setEndValue(QRect(560, 160, 200, 200));
        animation->setDuration(2000);
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    }
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
    this->ui->enemyHurtLabel->show();
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

void Battle::onMyPokeChange(int mypokeindex)
{
    this->ui->myPokeHPBarWidget->resize(170, 20);
    myPokeIndex = mypokeindex;
    std::string kind = myPokemonKind[mypokeindex];
    std::string name = myPokemonName[mypokeindex];
    //QString styleStr = "#myPokeLabel{border-image : url(:/" + QString::fromStdString(kind) + ")}";
    QString styleStr = "#myPokeLabel{image : url(:/squirtle)}";
    this->ui->myPokeLabel->setStyleSheet(styleStr);
    this->ui->myPokeInfoLabel->setText(QString::fromStdString(name));
    myPokeCurrentHP = myPokemonTHP[mypokeindex];
    QPropertyAnimation* animation = new QPropertyAnimation(this->ui->myPoke, "geometry");
    animation->setStartValue(QRect(30, 160, 200, 200));
    animation->setKeyValueAt(0.5, QRect(30, 160, 0, 200));
    animation->setEndValue(QRect(30, 160, 200, 200));
    animation->setDuration(1000);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Battle::onEnemyPokeChange(int enemypokeindex)
{
    this->ui->enemyPokeHPBarWidget->resize(170, 20);
    enemyPokeIndex = enemypokeindex;
    std::string kind = enemyPokemonKind[enemypokeindex];
    std::string name = enemyPokemonName[enemypokeindex];
    //QString styleStr = "#enemyPokeLabel{border-image : url(:/" + QString::fromStdString(kind) + ");}";
    QString styleStr = "#enemyPokeLabel{image : url(:/charmander);}";
    this->ui->enemyPokeLabel->setStyleSheet(styleStr);
    this->ui->enemyPokeInfoLabel->setText(QString::fromStdString(name));
    enemyPokeCurrentHP = enemyPokemonTHP[enemypokeindex];
    QPropertyAnimation* animation = new QPropertyAnimation(this->ui->enemyPoke, "geometry");
    animation->setStartValue(QRect(560, 160, 200, 200));
    animation->setKeyValueAt(0.5, QRect(560, 160, 0, 200));
    animation->setEndValue(QRect(560, 160, 200, 200));
    animation->setDuration(1000);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

DWORD WINAPI RecvThreadFuncBattle(SocketClient* socketClient, Battle* battle)
{
    SOCKET ConnectSocket = socketClient->getConnectSocket();
    socketClient->ClearRecvBuf();
    socketClient->iResult = recv(ConnectSocket, socketClient->recvbuf, socketClient->recvbuflen, 0);
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
