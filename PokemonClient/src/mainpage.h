#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include "lib/json.hpp"
#include "helper.h"
#include "socketClient.h"

using json = nlohmann::json;

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = 0);
    ~MainPage();
private:
    Ui::MainPage *ui;
    std::thread calledThread;
private slots:
    void receiveSwitch();
};

#endif // !MAINPAGE_H
