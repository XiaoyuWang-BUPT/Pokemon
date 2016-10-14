#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QObject::connect(this->ui->dialogButton, SIGNAL(clicked(bool)), this, SLOT(onDialogClicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onDialogClicked() {
    this->hide();
    dialog.setGeometry(100, 100, 640, 480);
    int res = dialog.exec();
    if (res == QDialog::Accepted)
    {
        QMessageBox::information(this, "INFORMATION", "You clicked OK button!");
    }
    if (res == QDialog::Rejected)
    {
        QMessageBox::information(this, "INFORMATION", "You clicked CANCEL button!");
    }
    this->show();
}
