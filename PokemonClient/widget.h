#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMessageBox>

#include "dialog.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    Dialog dialog;
private slots:
    void onDialogClicked();
};

#endif // WIDGET_H
