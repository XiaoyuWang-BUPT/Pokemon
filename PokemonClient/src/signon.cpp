#include "signon.h"
#include "ui_signon.h"
#include <QPalette>

SignOn::SignOn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignOn)
{
    ui->setupUi(this);
    this->setWindowTitle("Sign On");
    this->ui->topButtonSignIn->setFocus();
    QObject::connect(this->ui->topButtonSignIn, SIGNAL(clicked(bool)), this, SLOT(onTopSignInClicked()));
    QObject::connect(this->ui->signOnButton, SIGNAL(clicked(bool)), this, SLOT(signOnButtonClicked()));
    this->ui->userLineEdit->installEventFilter(this);
    this->ui->pwLineEdit1->installEventFilter(this);
    this->ui->pwLineEidt2->installEventFilter(this);
    this->ui->signOnButton->installEventFilter(this);
    this->ui->topButtonSignIn->installEventFilter(this);
}

SignOn::SignOn(SocketClient *sc, QWidget *parent) :
    SignOn(parent)
{
    socketClient = sc;
}

SignOn::~SignOn()
{
    delete socketClient;
    delete ui;
}

void SignOn::receiveSwitch()
{
    this->setGeometry(400, 200, 400, 300);
    this->show();
    this->ui->userLineEdit->setText(QString::fromStdString("input username"));
    this->ui->pwLineEdit1->setText(QString::fromStdString("input password"));
    this->ui->pwLineEidt2->setText(QString::fromStdString("ensure password"));
    this->ui->pwLineEdit1->setEchoMode(QLineEdit::Normal);
    this->ui->pwLineEidt2->setEchoMode(QLineEdit::Normal);
    isUNNotCLK = true;
    isPWNotCLK = true;
    isEPWNotCLK = true;
}

void SignOn::onTopSignInClicked()
{
    this->hide();
    emit switchToSignIn();
}

bool SignOn::eventFilter(QObject *watched, QEvent *event)
{
    QPalette focusinPalette = QPalette();
    QPalette focusoutPalette = QPalette();
    focusinPalette.setColor(QPalette::Base, QColor(221, 240, 237));
    focusoutPalette.setColor(QPalette::Base, Qt::white);
    if (watched == this->ui->userLineEdit)
    {
        if (event->type() == QEvent::FocusIn)
        {
            this->ui->userLineEdit->clear();
            this->ui->userLineEdit->setPalette(focusinPalette);
        }
        else if (event->type() == QEvent::FocusOut)
            this->ui->userLineEdit->setPalette(focusoutPalette);
    }
    if (watched == this->ui->pwLineEdit1)
    {
        if (event->type() == QEvent::FocusIn)
        {
            this->ui->pwLineEdit1->clear();
            this->ui->pwLineEdit1->setEchoMode(QLineEdit::Password);
            this->ui->pwLineEdit1->setPalette(focusinPalette);
        }
        else if (event->type() == QEvent::FocusOut)
            this->ui->pwLineEdit1->setPalette(focusoutPalette);
    }
    if (watched == this->ui->pwLineEidt2)
    {
        if (event->type() == QEvent::FocusIn)
        {
            this->ui->pwLineEidt2->clear();
            this->ui->pwLineEidt2->setEchoMode(QLineEdit::Password);
            this->ui->pwLineEidt2->setPalette(focusinPalette);
        }
        else if (event->type() == QEvent::FocusOut)
            this->ui->pwLineEidt2->setPalette(focusoutPalette);
    }
    if (watched == this->ui->signOnButton)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
            if (keyEvent->key() == Qt::Key_Enter
                    || keyEvent->key() == Qt::Key_Return)
                emit this->ui->signOnButton->clicked();
        }
    }
    if (watched == this->ui->topButtonSignIn)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
            if (keyEvent->key() == Qt::Key_Enter
                    || keyEvent->key() == Qt::Key_Return)
                emit this->ui->topButtonSignIn->clicked();
        }
    }
    return QWidget::eventFilter(watched, event);
}

DWORD WINAPI SendThreadFuncSignOn(LPVOID lParam, LPVOID sParam);
DWORD WINAPI RecvThreadFuncSignOn(LPVOID lParam, SignOn* signon);

void SignOn::signOnButtonClicked()
{
    if (this->ui->userLineEdit->text().isEmpty())
        QMessageBox::information(this, "Error", "Username can't be empty");
    else if (this->ui->userLineEdit->text().toStdString() == "input username")
    {
        QMessageBox::information(this, "Error", "Username can't be 'input username'");
        this->ui->userLineEdit->clear();
    }
    else if (this->ui->pwLineEdit1->text().isEmpty())
        QMessageBox::information(this, "Error", "Password can't be empty");
    else if (this->ui->pwLineEdit1->text().toStdString() == "input password")
    {
        QMessageBox::information(this, "Error", "Password can't be 'input password'");
        this->ui->pwLineEdit1->clear();
        this->ui->pwLineEidt2->clear();
    }
    else if (this->ui->pwLineEidt2->text().isEmpty())
        QMessageBox::information(this, "Error", "Ensure password");
    else if (this->ui->pwLineEdit1->text().toStdString() !=
             this->ui->pwLineEidt2->text().toStdString())
    {
        QMessageBox::information(this, "Error", "Password not consistent");
        this->ui->pwLineEdit1->clear();
        this->ui->pwLineEidt2->clear();
        this->ui->pwLineEdit1->setFocus();
    }
    else
    {
        json j;
        j["symbol"] = "signon";
        j["username"] = this->ui->userLineEdit->text().toStdString();
        j["password"] = this->ui->pwLineEdit1->text().toStdString();
        j["end"] = "end";
        std::string str = j.dump();

        std::thread signonSendThread = std::thread(SendThreadFuncSignOn, socketClient, &str);
        signonSendThread.join();

        std::thread signonRecvThread = std::thread(RecvThreadFuncSignOn, socketClient, this);
        signonRecvThread.join();

        json recvJ = json::parse(recvString);
        recvString = "";
        bool signonsuccess = recvJ["signonsuccess"];
        bool useravailable = recvJ["useravailable"];

        if (useravailable)
        {
            if (signonsuccess)
            {
                this->hide();
                emit switchToMainPage();
            }
            else
                QMessageBox::information(this, "Info", "Sign On failed, please Try Again");
        }
        else
            QMessageBox::information(this, "Info", "User already exists, please Sign In");
    }
}

bool SignOn::setRecvStrSignOn(QString s)
{
    this->recvString = s.toStdString();
    return true;
}

DWORD WINAPI SendThreadFuncSignOn(LPVOID lParam, LPVOID sParam)
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

DWORD WINAPI RecvThreadFuncSignOn(LPVOID lParam, SignOn* signon)
{
    SocketClient* socketClient = (SocketClient*)lParam;
    SOCKET ConnectSocket = socketClient->getConnectSocket();
    socketClient->ClearRecvBuf();
    socketClient->iResult = recv(ConnectSocket, socketClient->recvbuf, socketClient->recvbuflen, 0);
    if (socketClient->iResult > 0)
    {
        QMetaObject::invokeMethod(signon, "setRecvStrSignOn", Qt::DirectConnection, Q_ARG(QString, QString::fromStdString(socketClient->GetRecvStr())));
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
