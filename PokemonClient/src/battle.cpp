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
    QObject::connect(this, SIGNAL(MyPokeAttackSignal(QString)), this, SLOT(onMyPokeAttack(QString)));
    QObject::connect(this, SIGNAL(EnemyPokeAttackSignal(QString)), this, SLOT(onEnemyPokeAttack(QString)));
    QObject::connect(this, SIGNAL(MyPokeBeAttacked(int)), this, SLOT(onMyPokeBeAttacked(int)));
    QObject::connect(this, SIGNAL(EnemyPokeBeAttacked(int)), this, SLOT(onEnemyPokeBeAttacked(int)));
    QObject::connect(this, SIGNAL(MyPokeHurtSignal(int, QString)), this, SLOT(onMyPokeHurt(int, QString)));
    QObject::connect(this, SIGNAL(EnemyPokeHurtSignal(int, QString)), this, SLOT(onEnemyPokeHurt(int, QString)));
    QObject::connect(this, SIGNAL(MyPokeChangeSignal(int)), this, SLOT(onMyPokeChange(int)));
    QObject::connect(this, SIGNAL(EnemyPokeChangeSignal(int)), this, SLOT(onEnemyPokeChange(int)));
    QObject::connect(this, SIGNAL(ClearHurtSignal()), this, SLOT(ClearHurt()));
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
        std::string myNature = recvJ["mynature"];
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
        std::this_thread::sleep_for(std::chrono::seconds(2));
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
                    emit MyPokeHurtSignal(myhurthp, QString::fromStdString(natureStd));
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    emit ClearHurtSignal();
                    //my fighting pokemon is hurt to dead change another pokemon
                    if (myhurthp == 0)
                    {
                        myFightingIndex++;
                        if (myFightingIndex < pokeNum)
                        {
                            emit MyPokeChangeSignal(myFightingIndex);
                            std::this_thread::sleep_for(std::chrono::seconds(1));
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
                        emit MyPokeAttackSignal(QString::fromStdString(attway));
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        emit EnemyPokeBeAttacked(enemypokehp);
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        if (enemypokehp == 0)
                        {
                            enemyFightingIndex++;
                            if (enemyFightingIndex < pokeNum)
                            {
                                emit EnemyPokeChangeSignal(enemyFightingIndex);
                                std::this_thread::sleep_for(std::chrono::seconds(1));
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
                    emit MyPokeAttackSignal(QString::fromStdString(attway));
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    emit EnemyPokeBeAttacked(enemypokehp);
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    if (enemypokehp == 0)
                    {
                        enemyFightingIndex++;
                        if (enemyFightingIndex < pokeNum)
                        {
                            emit EnemyPokeChangeSignal(enemyFightingIndex);
                            std::this_thread::sleep_for(std::chrono::seconds(1));
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
                    emit EnemyPokeHurtSignal(enemyhurthp, QString::fromStdString(myNature));
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    emit ClearHurtSignal();
                    if (enemyhurthp == 0)
                    {
                        enemyFightingIndex++;
                        if (enemyFightingIndex < pokeNum)
                        {
                            emit EnemyPokeChangeSignal(enemyFightingIndex);
                            std::this_thread::sleep_for(std::chrono::seconds(1));
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
                        emit EnemyPokeAttackSignal(QString::fromStdString(attway));
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        emit MyPokeBeAttacked(mypokehp);
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        if (mypokehp == 0)
                        {
                            myFightingIndex++;
                            if (myFightingIndex < pokeNum)
                            {
                                emit MyPokeChangeSignal(myFightingIndex);
                                std::this_thread::sleep_for(std::chrono::seconds(1));
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
                    emit EnemyPokeAttackSignal(QString::fromStdString(attway));
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    emit MyPokeBeAttacked(mypokehp);
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    if (mypokehp == 0)
                    {
                        myFightingIndex++;
                        if (myFightingIndex < pokeNum)
                        {
                            emit MyPokeChangeSignal(myFightingIndex);
                            std::this_thread::sleep_for(std::chrono::seconds(1));
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

void Battle::onMyPokeAttack(QString attway)
{
    QPropertyAnimation* animation = new QPropertyAnimation(this->ui->myPoke, "geometry");
    animation->setStartValue(QRect(30, 160, 200, 200));
    animation->setKeyValueAt(0.5, QRect(560, 160, 200, 200));
    animation->setEndValue(QRect(30, 160, 200, 200));
    animation->setDuration(2000);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void Battle::onEnemyPokeAttack(QString attway)
{
    QPropertyAnimation* animation = new QPropertyAnimation(this->ui->enemyPoke, "geometry");
    animation->setStartValue(QRect(560, 160, 200, 200));
    animation->setKeyValueAt(0.5, QRect(30, 160, 200, 200));
    animation->setEndValue(QRect(560, 160, 200, 200));
    animation->setDuration(2000);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void Battle::onMyPokeBeAttacked(int mypokehp)
{
    int startHP = this->ui->myPokeHPBar->value();
    QPropertyAnimation* animation = new QPropertyAnimation(this->ui->myPokeHPBar, "value");
    animation->setStartValue(startHP);
    animation->setEndValue(mypokehp);
    animation->setDuration(1000);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Battle::onEnemyPokeBeAttacked(int enemypokehp)
{
    int startHP = this->ui->enemyPokeHPBar->value();
    QPropertyAnimation* animation = new QPropertyAnimation(this->ui->enemyPokeHPBar, "value");
    animation->setStartValue(startHP);
    animation->setEndValue(enemypokehp);
    animation->setDuration(1000);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Battle::onMyPokeHurt(int myhurthp, QString enemyNature)
{
    std::cout << "in mypokehurt function" << std::endl;
    this->ui->myHurtLabel->show();
    QPixmap pixmap;
    if (enemyNature == "bush")
        pixmap = QPixmap(":/poisoned");
    if (enemyNature == "electricity")
        pixmap = QPixmap(":/shock");
    if (enemyNature == "fire")
        pixmap = QPixmap(":/burned");
    if (enemyNature == "water")
        pixmap = QPixmap(":/drowned");
    this->ui->myHurtLabel->setPixmap(pixmap);
    int startHP = this->ui->myPokeHPBar->value();
    QPropertyAnimation* barAnimation = new QPropertyAnimation(this->ui->myPokeHPBar, "value");
    barAnimation->setStartValue(startHP);
    barAnimation->setEndValue(myhurthp);
    barAnimation->setDuration(1000);
//    QPropertyAnimation* animation = new QPropertyAnimation(this->ui->myPoke, "geometry");
//    animation->setStartValue(QRect(30, 160, 200, 200));
//    animation->setKeyValueAt(0.25, QRect(80, 210, 100, 100));
//    animation->setKeyValueAt(0.5, QRect(30, 160, 200, 200));
//    animation->setKeyValueAt(0.75, QRect(80, 210, 0, 0));
//    animation->setEndValue(QRect(30, 160, 200, 200));
//    animation->setDuration(1000);
//    animation->start(QAbstractAnimation::DeleteWhenStopped);
    barAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Battle::onEnemyPokeHurt(int enemyhurthp, QString myNature)
{
    this->ui->enemyHurtLabel->show();
    QPixmap pixmap;
    if (myNature == "bush")
        pixmap = QPixmap(":/poisoned");
    if (myNature == "electricity")
        pixmap = QPixmap(":/shock");
    if (myNature == "fire")
        pixmap = QPixmap(":/burned");
    if (myNature == "water")
        pixmap = QPixmap(":/drowned");
    this->ui->enemyHurtLabel->setPixmap(pixmap);
    int startHP = this->ui->enemyPokeHPBar->value();
    QPropertyAnimation* barAnimation = new QPropertyAnimation(this->ui->enemyPokeHPBar, "value");
    barAnimation->setStartValue(startHP);
    barAnimation->setEndValue(enemyhurthp);
    barAnimation->setDuration(1000);
//    QPropertyAnimation* animation = new QPropertyAnimation(this->ui->enemyPoke, "geometry");
//    animation->setStartValue(QRect(560, 160, 200, 200));
//    animation->setKeyValueAt(0.25, QRect(610, 210, 100, 100));
//    animation->setKeyValueAt(0.5, QRect(560, 160, 200, 200));
//    animation->setKeyValueAt(0.75, QRect(610, 210, 100, 100));
//    animation->setEndValue(QRect(560, 160, 200, 200));
//    animation->setDuration(1000);
//    animation->start(QAbstractAnimation::DeleteWhenStopped);
    barAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Battle::ClearHurt()
{
    this->ui->myHurtLabel->hide();
    this->ui->enemyHurtLabel->hide();
}

void Battle::onMyPokeChange(int mypokeindex)
{
    QPropertyAnimation* animation = new QPropertyAnimation(this->ui->myPoke, "geometry");
    animation->setStartValue(QRect(30, 160, 200, 200));
    animation->setKeyValueAt(0.5, QRect(30, 160, 0, 200));
    animation->setEndValue(QRect(30, 160, 200, 200));
    animation->setDuration(1000);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    myPokeIndex = mypokeindex;
    std::string kind = myPokemonKind[mypokeindex];
    std::string name = myPokemonName[mypokeindex];
    //QString styleStr = "#myPokeLabel{border-image : url(:/" + QString::fromStdString(kind) + ")}";
    QString styleStr = "#myPokeLabel{image : url(:/squirtle)}";
    this->ui->myPokeLabel->setStyleSheet(styleStr);
    this->ui->myPokeInfoLabel->setText(QString::fromStdString(name));
    int thp = myPokemonTHP[mypokeindex];
    this->ui->myPokeHPBar->setRange(0, thp);
    this->ui->myPokeHPBar->setValue(thp);
}

void Battle::onEnemyPokeChange(int enemypokeindex)
{
    QPropertyAnimation* animation = new QPropertyAnimation(this->ui->enemyPoke, "geometry");
    animation->setStartValue(QRect(560, 160, 200, 200));
    animation->setKeyValueAt(0.5, QRect(560, 160, 0, 200));
    animation->setEndValue(QRect(560, 160, 200, 200));
    animation->setDuration(1000);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    enemyPokeIndex = enemypokeindex;
    std::string kind = enemyPokemonKind[enemypokeindex];
    std::string name = enemyPokemonName[enemypokeindex];
    //QString styleStr = "#enemyPokeLabel{border-image : url(:/" + QString::fromStdString(kind) + ");}";
    QString styleStr = "#enemyPokeLabel{image : url(:/charmander);}";
    this->ui->enemyPokeLabel->setStyleSheet(styleStr);
    this->ui->enemyPokeInfoLabel->setText(QString::fromStdString(name));
    int thp = enemyPokemonTHP[enemypokeindex];
    this->ui->enemyPokeHPBar->setRange(0, thp);
    this->ui->enemyPokeHPBar->setValue(thp);
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
