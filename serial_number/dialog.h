#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

signals:
    void path_change_sig(QString str);

private:
    Ui::Dialog *ui;
    QString new_path;
};

#endif // DIALOG_H
