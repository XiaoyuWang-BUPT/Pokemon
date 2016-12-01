#include "signin.h"
#include "ui_signin.h"
#include <QMessageBox>
#include <iostream>

SignIn::SignIn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignIn)
{
    ui->setupUi(this);
    this->setGeometry(400, 200, 400, 300);
    this->setWindowTitle("Sign In");
    QObject::connect(this->ui->signInButton, SIGNAL(clicked(bool)), this, SLOT(onSignInClicked()));
    QObject::connect(this->ui->topButtonSignOn, SIGNAL(clicked(bool)), this, SLOT(onTopSignOnClicked()));
}

SignIn::~SignIn()
{
    delete ui;
}

void SignIn::receiveSwitch()
{
    this->ui->userLineEdit->clear();
    this->ui->pwLineEdit->clear();
    this->setGeometry(400, 200, 400, 300);
    this->show();
}

void SignIn::onTopSignOnClicked()
{
    this->hide();
    emit switchToSignOn();
}

void SignIn::onSignInClicked()
{
    if (this->ui->userLineEdit->text().isEmpty())
        QMessageBox::information(this, "Error", QString::fromStdString("Username can't be empty"));
    else
    {
        if (this->ui->pwLineEdit->text().isEmpty())
            QMessageBox::information(this, "Error", QString::fromStdString("Password can't be empty"));
        else
        {
            //Session
            json j;
            j["symbol"] = "singin";
            j["username"] = this->ui->userLineEdit->text().toStdString();
            j["password"] = this->ui->pwLineEdit->text().toStdString();
            j["end"] = "end";
            std::string str = j.dump();

            extern std::string sendStrHelper;
            sendStrHelper = str;

            extern std::string recvStrHelper;
            while (recvStrHelper == "");
            json recvJ = json::parse(recvStrHelper);
            recvStrHelper == "";
            std::string useravailable = recvJ["useravailable"];
            std::string passwordcorrect = recvJ["passwordcorrect"];

            if (useravailable == "false")
            {
                QMessageBox::information(this, "Info", "User exists please sign on");
            }
            else if (passwordcorrect == "true")
            {
                this->ui->userLineEdit->clear();
                this->ui->pwLineEdit->clear();
                this->hide();
                emit switchToMainPage();
            }
            else
            {
                QMessageBox::information(this, "Info", "Wrong password");
            }
        }
    }
}

bool SignIn::userAvailabel()
{
    return false;
}
