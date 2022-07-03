#ifndef QSQLITE_OPTION_H
#define QSQLITE_OPTION_H

#include <QObject>
#include <QMessageBox>
#include <qmessagebox.h>
#include <QDebug>
#include <QStringList>
#include <QApplication>

#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QSqlQuery>
#include <QFile>
#include <QVariantList>
#include <QSqlDriver>

#define SQL_URL "//192.168.2.3/xjk/项目一部/共享资料/序列号生成/info.db"
//#define SQL_URL "E:/qt_pro/info.db"
//#define SQL_URL "./info.db"
typedef struct SERIAL_INFO{
    QString create_time;
    QString pro_name;
    QString part_code;
    QString file_type;
    QString device_id;
    QString count;
    QString description;
    QString serial_number;
}SERIAL_INFO;

class qsqlite_option : public QObject
{
    Q_OBJECT
public:
    explicit qsqlite_option(QObject *parent = nullptr);

    QSqlDatabase db;

    QSqlQuery query;

    SERIAL_INFO serial_info;

    void sql_open_db(QString path);

    bool sql_tableisexist(QString name);
    bool sql_add_info(SERIAL_INFO data, QString table);

    QStringList sql_search_info(SERIAL_INFO data, QString table);
    QString sql_search_num(QString str, QString file_name, QString table);

signals:
    void sig_db_change(bool, QString);
};

#endif // QSQLITE_OPTION_H
