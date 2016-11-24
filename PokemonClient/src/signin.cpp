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
            if (userAvailabel(this->ui->userLineEdit->text().toStdString()))
            {
                this->ui->userLineEdit->clear();
                this->ui->pwLineEdit->clear();
                this->hide();
                emit switchToMainPage();
            }
            else
            {
                QMessageBox::information(this, "Info", "User exists please sign on");
            }
        }
    }
}

bool SignIn::userAvailabel(std::__cxx11::string name)
{
    //session
    return false;
}
