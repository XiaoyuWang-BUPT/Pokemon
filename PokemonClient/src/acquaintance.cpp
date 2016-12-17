#include "acquaintance.h"
#include "ui_acquaintance.h"

Acquaintance::Acquaintance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Acquaintance)
{
    ui->setupUi(this);
    this->setWindowTitle("Acquaintance");
    this->ui->charmanderButton->installEventFilter(this);
    this->ui->squirtleButton->installEventFilter(this);
    this->ui->bulbasaurButton->installEventFilter(this);
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
    std::thread sendThread = std::thread(SendThreadFuncAcq, socketClient, &sendStr);
    sendThread.join();
    socketClient->Receive();
    socketClient->ClearRecvBuf();
    this->hide();
    emit SwitchToMainPage();
}

DWORD WINAPI SendThreadFuncAcq(LPVOID lParam, LPVOID sParam)
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
