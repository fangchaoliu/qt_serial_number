#include "qsqlite_option.h"

qsqlite_option::qsqlite_option(QObject *parent)
    : QObject{parent}
{
    sql_open_db(SQL_URL);
}


void qsqlite_option::sql_open_db(QString path)
{
    if(path.isEmpty())
    {
        emit sig_db_change(false,"路径错误");
        return;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");    // 设置数据库类型
    db.setHostName("localhost:");      // 数据库主机名
    db.setDatabaseName(path);       // 数据库名
    db.setUserName("test");           // 数据库用户名
    db.setPassword("123456");         // 数据库密码
    db.open();                        // 打开数据库
    if(!db.isOpen())
    {
        emit sig_db_change(false,"数据库打开失败1");
        return;
    }
    bool test_sql_name = sql_tableisexist("serial_number");
    if(!test_sql_name)  // 创建表
    {
        QString create_table = "create table serial_number(create_time varchar(20),pro_num varchar(5),part_code varchar(5),file_type varchar(5), count varchar(2), description varchar(50),serial_number varchar(30) primary key)"; //创建数据表

        QSqlQuery sql_query;
        sql_query.prepare(create_table); //创建表

        if(!sql_query.exec()) //查看创建表是否成功
        {
            emit sig_db_change(false,"数据表已存在");
            //qDebug()<<QObject::tr("数据表创建失败") << sql_query.lastError().text();
        }
        else
        {
            emit sig_db_change(true,"创建数据表成功");
            qDebug()<< "创建数据表成功" ;
        }
    }
    test_sql_name = sql_tableisexist("serial_No");
    if(!test_sql_name)  // 创建表
    {
        QString create_table = "create table serial_No(create_time varchar(20),part_code varchar(5),file_type varchar(5), count varchar(2), description varchar(50),serial_No varchar(30) primary key)"; //创建数据表

        QSqlQuery sql_query;
        sql_query.prepare(create_table); //创建表

        if(!sql_query.exec()) //查看创建表是否成功
        {
            emit sig_db_change(false,"数据表已存在");
            //qDebug()<<QObject::tr("数据表创建失败") << sql_query.lastError().text();
        }
        else
        {
            emit sig_db_change(true,"创建数据表成功");
            qDebug()<< "创建数据表成功" ;
        }
    }
    db.close();
}


bool qsqlite_option::sql_add_info(SERIAL_INFO data, QString table)
{
    if(!db.open())    // 如果数据库关了再次打开,失败直接返回
    {
        emit sig_db_change(false,"数据库打开失败2");
        return false;
    }
    //qDebug() << "sql_add_info 3 :" << db.isOpen();

    QSqlQuery sql_query(db);    //指定连接到db数据库

    if(table == "serial_number")
    {
        qDebug() << "sql_add_info serial_number";
        QString str = QString("INSERT INTO serial_number(create_time,pro_num,part_code,file_type, count, description,serial_number) VALUES('%1','%2','%3','%4','%5','%6','%7')")\
                .arg(data.create_time).arg(data.pro_name).arg(data.part_code).arg(data.file_type).arg(data.count).arg(data.description).arg(data.serial_number);
        if(sql_query.exec(str))
        {
            emit sig_db_change(true,"添加数据成功");
        }
        else
        {
            emit sig_db_change(false,"添加数据失败");
        }
    }
    if(table == "serial_No")
    {
        qDebug() << "sql_add_info serial_No 1";
        QString str = QString("INSERT INTO serial_No(create_time,part_code,file_type, count, description,serial_No) VALUES('%1','%2','%3','%4','%5','%6')")\
                .arg(data.create_time).arg(data.part_code).arg(data.file_type).arg(data.count).arg(data.description).arg(data.serial_number);
        if(sql_query.exec(str))
        {
            emit sig_db_change(true,"添加数据成功");
        }
        else
        {
            emit sig_db_change(false,"添加数据失败");
        }
    }

    db.close();
    return true;
}

QStringList qsqlite_option::sql_search_info(SERIAL_INFO data, QString table)
{
    QString search_str = "";
    QString search_res = "";
    QStringList res;

    if(!db.open())    // 如果数据库关了再次打开,失败直接返回
    {
        emit sig_db_change(false,"数据库打开失败3");
        return res;
    }
    qDebug() << "sql_search_info 3 :" << db.isOpen();


    QSqlQuery sql_query(db);

    if(table == "serial_number")
    {
        search_str = data.description;
        //组装sql 搜索语句
        QString str = "select * from serial_number where description like '%";
        str += search_str;
        str += "%'";

        //qDebug() << str;
        sql_query.exec(str);

        while(sql_query.next())
        {
            search_res = "";
            for(int i=0; i<7; i++)
            {
                QString s = sql_query.value(i).toString();
                qDebug() << s;
                if(s.isEmpty())
                {
                    s = "-";
                }
                search_res += s;
                search_res += "\t";
            }
            res << search_res;
        }
        return res;
    }
    if(table == "serial_No")
    {
        search_str = data.description;
        //组装sql 搜索语句
        QString str = "select * from serial_No where description like '%";
        str += search_str;
        str += "%'";

        qDebug() << str;
        sql_query.exec(str);

        while(sql_query.next())
        {
            search_res = "";
            for(int i=0; i<6; i++)
            {
                QString s = sql_query.value(i).toString();
                if(s.isEmpty())
                {
                    s = "-";
                }
                search_res += s;
                search_res += "\t";
            }
            res << search_res;

        }
        qDebug() << "sql_search_info serial_No 555" << table;
    }
    return res;

}

// 按序列号查找,不带count
QString qsqlite_option::sql_search_num(QString search_str, QString file_name, QString table)
{
    QString str = "";
    QString search_res = "";

    if(!db.open())    // 如果数据库关了再次打开,失败直接返回
    {
        emit sig_db_change(false,"数据库打开失败4");
        qDebug() << "sql_search_num 2 :" << db.isOpen();
        return search_res;
    }
    qDebug() << "sql_search_num 3 :" << db.isOpen();

    QSqlQuery sql_query(db);

    if(table == "serial_number")
    {
        //组装sql 搜索语句
        str = "select * from serial_number where serial_number like '%";
        str += search_str;
        str += "%'";

        qDebug() << "sql_search_num 4 :" << str;
        sql_query.exec(str);

        while(sql_query.next())
        {
            search_res = "";
            QString str = sql_query.value(5).toString();
            qDebug() << "sql_search_num file_name:" << file_name  << "str:" << str;
            if(str == file_name)
            {
                qDebug() << "file_name is exist :" << str;
                emit sig_db_change(false,"文件名重复");
                search_res = "exist";
                break;
            }
            else
            {
                search_res = sql_query.value(6).toString();
            }

        }
        qDebug() << "sql_search_num 5 :" << str;

    }
    if(table == "serial_No")
    {
        //组装sql 搜索语句
        str = "select * from serial_No where serial_No like '%";
        str += search_str;
        str += "%'";

        qDebug() << "serial_No sql_search_num 5 :" << str;
        sql_query.exec(str);

        while(sql_query.next())
        {
            search_res = "";
            QString str = sql_query.value(4).toString();
            //qDebug() << sql_query.value(0).toString()<< sql_query.value(1).toString()<< sql_query.value(2).toString()<< sql_query.value(3).toString()<< sql_query.value(4).toString();
            if(str == file_name)
            {
                qDebug() << "file_name is exist :" << str;
                emit sig_db_change(false,"文件名重复");
                search_res = "exist";
                break;
                //return search_res;
            }
            else
            {
                search_res = sql_query.value(3).toString();
            }
                qDebug() << "search_res" << search_res;

        }
    }
    return search_res;

}

bool qsqlite_option::sql_tableisexist(QString name)
{
    QString str;
    if(name.isEmpty())
        return false;
    QSqlQuery sql_query(db);
    if(sql_query.exec("select count(*) from name"))
    {
        return true;
    }
    else
    {
        return false;
    }
}
