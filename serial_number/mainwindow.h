#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QDebug>
#include <QTime>
#include <QDate>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QStandardPaths>
#include <QStringLiteral>

#include <QTableView>
#include "qsqlite_option.h"
#include "file_parse.h"
#include "dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void company_init();
    void company_part_init();
    void file_type_init();
    void project_init();
    void project_type_init();
    QString getcurrent_time();
    QString getcurrent_date();
    void updata_help_info();
    void updata_search_info(QStringList str,QString table);

    //保存数据到excel
    bool saveFromTable(QString filePath, QTableView *tableView, QString comment);
    bool save(QString filePath, QStringList headers, QList<QStringList> data,QString comment);
    bool insert(QSqlQuery &query, QString sheetName, QStringList slist);

private slots:
    void on_pushButton_create_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_checkout_clicked();

    void slotHeaderClicked(int index);

    void on_pushButton_back_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void updata_path(QString str);

    void db_change_slot(bool res,QString str);

    void on_pushButton_help_back_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_show_clicked();

    void on_pushButton_create_No_clicked();

    void on_comboBox_part_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QStringList part_list;
    QMap<QString,QString > company_map;
    QMap<QString,QString > part_map;
    QMap<QString,QString > type_map;
    QMap<QString,QString > project_map;
    QMap<QString,QString > project_type_map;
    int res_index;
    int res_show;
    QString pre_file_name;
    QString file_name;

    qsqlite_option *my_sql_op;
    file_parse my_file_config;

    Dialog *dialog;
};

#endif // MAINWINDOW_H
