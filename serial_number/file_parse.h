#ifndef FILE_PARSE_H
#define FILE_PARSE_H

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QIODevice>
#include <QTextCursor>
#include <QTextStream>
//#include <QTextCodec>
#include <QSplitter>
//#include <QRegExp>
#include <QStringList>
#include <QRegularExpression>

#define CONFIG_PATH "//192.168.2.3/xjk/项目一部/共享资料/序列号生成/serial_number_config.ini"
//#define CONFIG_PATH "E:/Andy/QT_Project/serial_number_config.ini"
//#define CONFIG_PATH "//192.168.2.3/xjk/项目一部/共享资料/serial_number_config.ini"
//#define CONFIG_PATH "./serial_number_config.ini"

class file_parse
{
public:
    QMap<QString,QString > config_company_map;  //公司

    QMap<QString,QString > config_part_map; //体系--部门
    QMap<QString,QString > config_part_type_map; // 体系文件类别

    QMap<QString,QString > config_project_map; // 项目列表
    QMap<QString,QString > config_project_type_map;

    QFile *my_file;

public:
    file_parse();

    bool file_open();
    bool file_read(QString str);
};

#endif // FILE_PARSE_H
