#ifndef SIGNON_H
#define SIGNON_H

#include <QWidget>
#include <QMouseEvent>
#include <QMessageBox>
#include <iostream>

namespace Ui {
class SignOn;
}

class SignOn : public QWidget
{
    Q_OBJECT

public:
    explicit SignOn(QWidget *parent = 0);
    ~SignOn();
private:
    Ui::SignOn *ui;
    bool isUNNotCLK = true; //username lineedit not clicked, first time change cursor
    bool isPWNotCLK = true; //password lineedit not clicked, first time change cursor
    bool isEPWNotCLK = true; //ensure password lineedit not clicked, first time change cursor
    bool userExist(std::string username);
private slots:    
    void receiveSwitch();
    void onTopSignInClicked();
    void userLineEditClicked(int, int);
    void pwLineEditClicked(int, int);
    void pwEnsureLineEditClicked(int, int);
    void signOnButtonClicked();
signals:
    void switchToSignIn();
    void switchToMainPage();
};

#endif // SIGNON_H
