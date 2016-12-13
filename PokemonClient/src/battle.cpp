#include "battle.h"
#include "ui_battle.h"

Battle::Battle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Battle)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->ui->backButton->installEventFilter(this);

    QObject::connect(this, SIGNAL(MyPokeAttackSignal(QString,int)), this, SLOT(onMyPokeAttack(QString,int)));
    QObject::connect(this, SIGNAL(EnemyPokeAttackSignal(QString,int)), this, SLOT(onEnemyPokeAttack(QString,int)));
    QObject::connect(this, SIGNAL(MyPokeHurtSignal(int)), this, SLOT(onMyPokeHurt(int)));
    QObject::connect(this, SIGNAL(EnemyPokeHurtSignal(int)), this, SLOT(onEnemyPokeHurt(int)));
    QObject::connect(this, SIGNAL(MyPokeChangeSignal(int)), this, SLOT(onMyPokeChange(int)));
    QObject::connect(this, SIGNAL(EnemyPokeChangeSignal(int)), this, SLOT(onEnemyPokeChange(int)));
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

DWORD WINAPI SendThreadFuncBattle(LPVOID lParam, LPVOID sParam);
DWORD WINAPI RecvThreadFuncBattle(SocketClient* socketClient, Battle* battle);

void Battle::SendAndRecvFunc(json j)
{
    std::string str = j.dump();
    std::thread battleSendThread = std::thread(SendThreadFuncBattle, socketClient, &str);
    battleSendThread.join();
    std::thread battleRecvThread = std::thread(RecvThreadFuncBattle, socketClient, this);
    battleRecvThread.join();
    return;
}

void Battle::receiveSwitch(QString nature, QString standard)
{
    this->show();
    json j;
    j["symbol"] = "battle";
    j["nature"] = nature.toStdString();
    j["standard"] = standard.toStdString();
    j["name"] = socketClient->getPlayerName();
    j["end"] = "end";
    SendAndRecvFunc(j);
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
        ::Sleep(500);
        for (; roundCnt < round; roundCnt++)
        {
            stream.clear();
            stream << roundCnt;
            stream >> roundStr;
            keyStr = "round" + roundStr + "attack";
            attackPokemon = recvJ[keyStr];
            if (attackPokemon == "mypokemon")
            {
                std::cout << "my turn" << std::endl;
                keyStr = "round" + roundStr + "myhurt";
                bool myhurt = recvJ[keyStr];
                if (myhurt)
                {
                    keyStr = "round" + roundStr + "myhurthp";
                    int myhurthp = recvJ[keyStr];
                    emit MyPokeHurtSignal(myhurthp);
                    //my fighting pokemon is hurt to dead change another pokemon
                    if (myhurthp == 0)
                    {
                        myFightingIndex++;
                        if (myFightingIndex < pokeNum)
                        {
                            ::Sleep(1000);
                            emit MyPokeChangeSignal(myFightingIndex);
                        }
                        else
                        {
                            //TODO lose
                        }
                    }
                    else
                    {
                        keyStr = "round" + roundStr + "attway";
                        std::string attway = recvJ[keyStr];
                        keyStr = "round" + roundStr + "enemypokemonhp";
                        int enemypokehp = recvJ[keyStr];
                        emit MyPokeAttackSignal(QString::fromStdString(attway), enemypokehp);
                        if (enemypokehp == 0)
                        {
                            enemyFightingIndex++;
                            if (enemyFightingIndex < pokeNum)
                            {
                                ::Sleep(1000);
                                emit EnemyPokeChangeSignal(enemyFightingIndex);
                            }
                            else
                            {
                                //TODO win
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
                    emit MyPokeAttackSignal(QString::fromStdString(attway), enemypokehp);
                    if (enemypokehp == 0)
                    {
                        enemyFightingIndex++;
                        if (enemyFightingIndex < pokeNum)
                        {
                            ::Sleep(1000);
                            emit EnemyPokeChangeSignal(enemyFightingIndex);
                        }
                        else
                        {
                            //TODO win
                        }
                    }
                }
            }


            if (attackPokemon == "enemypokemon")
            {
                std::cout << "enemy turn" << std::endl;
                keyStr = "round" + roundStr + "enemyhurt";
                bool enemyhurt = recvJ[keyStr];
                if (enemyhurt)
                {
                    keyStr = "round" + roundStr + "enemyhurthp";
                    int enemyhurthp = recvJ[keyStr];
                    if (enemyhurthp == 0)
                    {
                        enemyFightingIndex++;
                        if (enemyFightingIndex < pokeNum)
                        {
                            ::Sleep(1000);
                            emit EnemyPokeHurtSignal(enemyFightingIndex);
                        }
                        else
                        {
                            //TODO win
                        }
                    }
                    else
                    {
                        keyStr = "round" + roundStr + "attway";
                        std::string attway = recvJ[keyStr];
                        keyStr = "round" + roundStr + "mypokemonhp";
                        int mypokehp = recvJ[keyStr];
                        emit EnemyPokeAttackSignal(QString::fromStdString(attway), mypokehp);
                        if (mypokehp == 0)
                        {
                            std::cout << "my " << (myFightingIndex) << " pokemon attacked yo die" << std::endl;
                            myFightingIndex++;
                            if (myFightingIndex < pokeNum)
                            {
                                ::Sleep(1000);
                                emit MyPokeChangeSignal(myFightingIndex);
                            }
                            else
                            {
                                //TODO lose
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
                    emit EnemyPokeAttackSignal(QString::fromStdString(attway), mypokehp);
                    if (mypokehp == 0)
                    {
                        std::cout << "my " << (myFightingIndex) << " pokemon attacked yo die" << std::endl;
                        myFightingIndex++;
                        if (myFightingIndex < pokeNum)
                        {
                            ::Sleep(1000);
                            emit MyPokeChangeSignal(myFightingIndex);
                        }
                        else
                        {
                            //TODO lose
                        }
                    }
                }
            }
        }
    }
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

void Battle::onMyPokeAttack(QString attway, int enemypokehp)
{

}

void Battle::onEnemyPokeAttack(QString attway, int mypokehp)
{

}

void Battle::onMyPokeHurt(int myhurthp)
{

}

void Battle::onEnemyPokeHurt(int enemyhurthp)
{

}

void Battle::onMyPokeChange(int mypokeindex)
{
    std::string kind = myPokemonKind[mypokeindex];
    std::string name = myPokemonName[mypokeindex];
    //std::string graphStr = ":/" + kind;
    std::cout << myPokemonKind.size() << " " << mypokeindex << std::endl;
    std::string graphStr = ":/squirtle";
    QPixmap pixmap(QString::fromStdString(graphStr));
    this->ui->myPokeLabel->setPixmap(pixmap);
    this->ui->myPokeInfoLabel->setText(QString::fromStdString(name));
    this->ui->myHPBar->setValue(100);
}

void Battle::onEnemyPokeChange(int enemypokeindex)
{

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
DWORD WINAPI SendThreadFuncBattle(LPVOID lParam, LPVOID sParam)
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
