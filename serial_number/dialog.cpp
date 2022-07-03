#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_pushButton_ok_clicked()
{
    new_path = ui->lineEdit->text();
    emit path_change_sig(new_path);
    this->close();
}


void Dialog::on_pushButton_cancel_clicked()
{
    this->close();
}

