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
    QMessageBox::information(this, "Info", "User Exists Please Sign In");
}

void SignIn::onSignInClicked() {
    std::cout << this->ui->userLineEdit->text().toStdString() << std::endl;
    std::cout << this->ui->pwLineEdit->text().toStdString() << std::endl;

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
                this->hide();
                emit switchToSignOn();
            }
        }
    }
}

bool SignIn::userAvailabel(std::__cxx11::string name)
{
    //session
    return true;
}
