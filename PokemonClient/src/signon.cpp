#include "signon.h"
#include "ui_signon.h"

SignOn::SignOn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignOn)
{
    ui->setupUi(this);
    this->setWindowTitle("Sign On");

    QObject::connect(this->ui->topButtonSignIn, SIGNAL(clicked(bool)), this, SLOT(onTopSignInClicked()));
    QObject::connect(this->ui->userLineEdit, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(userLineEditClicked(int,int)));
    QObject::connect(this->ui->pwLineEdit1, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(pwLineEditClicked(int,int)));
    QObject::connect(this->ui->pwLineEidt2, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(pwEnsureLineEditClicked(int,int)));
    QObject::connect(this->ui->signOnButton, SIGNAL(clicked(bool)), this, SLOT(signOnButtonClicked()));
}

SignOn::~SignOn()
{
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

void SignOn::userLineEditClicked(int, int)
{
    if (this->ui->userLineEdit->text().toStdString() == "input username" && isUNNotCLK)
    {
        isUNNotCLK = false;
        this->ui->userLineEdit->clear();
    }
}

void SignOn::pwLineEditClicked(int, int)
{
    if (this->ui->pwLineEdit1->text().toStdString() == "input password" && isPWNotCLK)
    {
        isPWNotCLK = false;
        this->ui->pwLineEdit1->clear();
        this->ui->pwLineEdit1->setEchoMode(QLineEdit::Password);
    }
}

void SignOn::pwEnsureLineEditClicked(int, int)
{
    if (this->ui->pwLineEidt2->text().toStdString() == "ensure password" && isEPWNotCLK)
    {
        isEPWNotCLK = false;
        this->ui->pwLineEidt2->clear();
        this->ui->pwLineEidt2->setEchoMode(QLineEdit::Password);
    }
}

void SignOn::signOnButtonClicked()
{
    if (this->ui->userLineEdit->text().isEmpty())
        QMessageBox::information(this, "Error", "Username can't be empty");
    else if (this->ui->userLineEdit->text().toStdString() == "input username")
    {
        QMessageBox::information(this, "Error", "Username can't be input password");
        this->ui->userLineEdit->clear();
    }
    else if (this->ui->pwLineEdit1->text().isEmpty())
        QMessageBox::information(this, "Error", "Password can't be empty");
    else if (this->ui->pwLineEdit1->text().toStdString() == "input password")
    {
        QMessageBox::information(this, "Error", "Password can't be input password");
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
    }
    else
    {
        //check from session
        if (userExist(this->ui->userLineEdit->text().toStdString()))
        {
            QMessageBox::information(this, "Info", "User exists please sign in");
        }
        else
        {
            //sign on
            this->hide();
            emit switchToMainPage();
        }
    }
}

bool SignOn::userExist(std::string username)
{
    //session
    return true;
}
