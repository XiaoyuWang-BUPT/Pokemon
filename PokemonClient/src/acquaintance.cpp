#include "acquaintance.h"
#include "ui_acquaintance.h"

Acquaintance::Acquaintance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Acquaintance)
{
    ui->setupUi(this);
    this->InitUI();
    this->SetEventFilter();
    this->InitConnect();
}

void Acquaintance::InitUI()
{
    this->setWindowTitle("Acquaintance");
}

void Acquaintance::SetEventFilter()
{
    this->ui->charmanderButton->installEventFilter(this);
    this->ui->squirtleButton->installEventFilter(this);
    this->ui->bulbasaurButton->installEventFilter(this);
}

void Acquaintance::InitConnect()
{
    QObject::connect(this, SIGNAL(choseSignal(QObject*)), this, SLOT(onChose(QObject*)));
}

Acquaintance::Acquaintance(SocketClient *sc, QWidget *parent) :
    Acquaintance(parent)
{
    socketClient = sc;
}

Acquaintance::~Acquaintance()
{
    delete ui;
}

bool Acquaintance::eventFilter(QObject *watched, QEvent *event)
{
    //if player has chosen pokemon, choseSignal is emitted immediately
    if (watched == this->ui->charmanderButton
            || watched == this->ui->squirtleButton
            || watched == this->ui->bulbasaurButton)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            emit choseSignal(watched);
        }
    }
    return QWidget::eventFilter(watched, event);
}

void Acquaintance::receiveSwitch()
{
    this->show();
}

DWORD WINAPI SendThreadFuncAcq(LPVOID lParam, LPVOID sParam);

void Acquaintance::onChose(QObject *obj)
{
    //json is intialized according to pokemon has been chosed
    json j;
    j["symbol"] = "acquaintance";
    j["end"] = "end";
    if (obj == this->ui->charmanderButton)
    {
        j["pokemon"] = "Charmander";
    }
    if (obj == this->ui->squirtleButton)
    {
        j["pokemon"] = "Squirtle";
    }
    if (obj == this->ui->bulbasaurButton)
    {
        j["pokemon"] = "Bulbasaur";
    }
    std::string owner = socketClient->getPlayerName();
    j["owner"] = owner;
    std::string sendStr = j.dump();

    //send information about gosanke
    std::thread sendThread = std::thread(SendThreadFuncAcq, socketClient, &sendStr);
    sendThread.join();

    //receive feedback from server
    socketClient->Receive();
    socketClient->ClearRecvBuf();

    //after chosen gosanke, player would get into main page
    this->hide();
    emit SwitchToMainPage();
}

/**
    Function : SendThreadFuncAcq
    Description : send message to server in a new thread
    Input : lParam is socketClient, sParam is sendStr
    Return : 0
    Others : None
**/
DWORD WINAPI SendThreadFuncAcq(LPVOID lParam, LPVOID sParam)
{
    //message of std::string
    std::string *sendStr = (std::string*)sParam;

    //client socket linked to server
    SocketClient *socketClient = (SocketClient*)lParam;
    int iResult;

    //connect socket of client socket which is connected to server
    SOCKET ConnectSocket = socketClient->getConnectSocket();
    size_t len = (*sendStr).length();
    char *sendbuf = new char[len];
    strcpy(sendbuf, (*sendStr).data());
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);

    //cleaup if send failed
    if (iResult == SOCKET_ERROR)
    {
        std::cout << "Send failed with error " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        system("pause");
    }
    return 0;
}
