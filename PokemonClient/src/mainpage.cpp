#include "mainpage.h"
#include "ui_mainpage.h"

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    this->setWindowTitle("MainPage");
}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::receiveSwitch()
{
    this->setGeometry(100, 100, 640, 480);
    this->show();
}
