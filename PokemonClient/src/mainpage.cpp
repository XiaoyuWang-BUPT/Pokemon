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

DWORD WINAPI CalledThreadFunc(MainPage* mainpage)
{
    ::Sleep(1000);
    QMetaObject::invokeMethod(mainpage, "setStyleSheet", Q_ARG(QString, "background-color:blue"));
    return 0;
}

void MainPage::receiveSwitch()
{
    this->setGeometry(100, 100, 640, 480);
    this->show();
    calledThread = std::thread(CalledThreadFunc, this);
    calledThread.detach();
}
