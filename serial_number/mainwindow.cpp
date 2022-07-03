#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QMessageBox>
#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    res_index = 0;
    res_show = 0;

    //设置tabview首行增加网格线
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    company_init();
    company_part_init();
    file_type_init();
    project_init();
    project_type_init();
    my_file_config.file_read(CONFIG_PATH);
    updata_help_info();

    my_sql_op = new qsqlite_option;

    ui->pushButton_update->hide();

    //设置 comboBox 固定长度,超出部分按滚动条显示
    ui->comboBox_Company->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboBox_Company->setStyleSheet("QComboBox{combobox-popup:0;}");
    ui->comboBox_Company->setMaxVisibleItems(10);

    ui->comboBox_type->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboBox_type->setStyleSheet("QComboBox{combobox-popup:0;}");
    ui->comboBox_type->setMaxVisibleItems(10);

    ui->comboBox_part->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboBox_part->setStyleSheet("QComboBox{combobox-popup:0;}");
    ui->comboBox_part->setMaxVisibleItems(10);


    connect(my_sql_op, SIGNAL(sig_db_change(bool ,QString)),this, SLOT(db_change_slot(bool,QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::company_init() // 初始化公司信息
{
    company_map.clear();
    ui->comboBox_Company->clear();
    company_map = my_file_config.config_company_map;
    //qDebug() << "company_init" << company_map;
    QList<QString> values = company_map.keys();
    ui->comboBox_Company->addItems(values);
}

void MainWindow::company_part_init()    // 初始化体系部门信息
{
    part_map.clear();
    ui->comboBox_part->clear();
    part_map = my_file_config.config_part_map;
    QList<QString> values = part_map.keys();
    ui->comboBox_part->addItems(values);
}

void MainWindow::file_type_init()   // 初始化体系文件类别
{
    type_map.clear();
    ui->comboBox_type->clear();
    type_map = my_file_config.config_part_type_map;
//    QList<QString> values = type_map.keys();
//    ui->comboBox_type->addItems(values);
}

void MainWindow::project_init() // 初始化项目
{
    project_map.clear();
    project_map = my_file_config.config_project_map;
    QList<QString> values = project_map.keys();
    ui->comboBox_part->addItems(values);
}
void MainWindow::project_type_init()    // 初始化项目类别
{
    project_type_map.clear();
    ui->comboBox_type->clear();
    project_type_map = my_file_config.config_project_type_map;
}

//部门/项目信息变化之后,根据变化内容显示对应文件类别
void MainWindow::on_comboBox_part_currentTextChanged(const QString &arg1)
{
    QString str = "";
    qDebug() << project_map;
    qDebug() << project_map.contains(arg1);
    if(project_map.contains(arg1))
    {
        ui->comboBox_type->clear();
        ui->comboBox_type->addItems(project_type_map.keys());
    }
    else if(part_map.contains(arg1))
    {
        ui->comboBox_type->clear();
        ui->comboBox_type->addItems(type_map.keys());
    }
}

void MainWindow::updata_help_info()
{
    QString str = "";
    QMap<QString,QString>::Iterator iter;
    QStandardItemModel *model_part = new QStandardItemModel();
    QStandardItemModel *model_type = new QStandardItemModel();

    QStandardItemModel *model_project = new QStandardItemModel();
    QStandardItemModel *model_project_type = new QStandardItemModel();

    //显示部门信息
    model_part->setColumnCount(2);

    model_part->setHeaderData(0,Qt::Horizontal,QString::fromUtf8("部门"));
    model_part->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("标识"));
    ui->tableView_part->setModel(model_part);
    //设置表格列宽自适应
    ui->tableView_part->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    int i = 0;
    for(iter = my_file_config.config_part_map.begin(); iter != my_file_config.config_part_map.end(); ++iter)
    {
        //str += iter.key() + "---" + iter.value() + "\n";
        if(iter.key().isEmpty() || iter.value().isEmpty())
        {

        }
        else
        {
            model_part->setItem(i,0,new QStandardItem(iter.key()));
            model_part->setItem(i,1,new QStandardItem(iter.value()));
            i++;
        }

    }
    //显示类型信息
    model_type->setColumnCount(2);

    model_type->setHeaderData(0,Qt::Horizontal,QString::fromUtf8("文件类型"));
    model_type->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("标识"));

    ui->tableView_type->setModel(model_type);
    //设置表格列宽自适应
    ui->tableView_type->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    int j = 0;
//    添加内容
    for(iter = my_file_config.config_part_type_map.begin(); iter != my_file_config.config_part_type_map.end(); ++iter)
    {
        //str += iter.key() + "---" + iter.value() + "\n";
        if(iter.key().isEmpty() || iter.value().isEmpty())
        {
        }
        else
        {
            model_type->setItem(j,0,new QStandardItem(iter.key()));
            model_type->setItem(j,1,new QStandardItem(iter.value()));
            j++;
        }
    }

    // 显示项目信息
    model_project->setColumnCount(2);

    model_project->setHeaderData(0,Qt::Horizontal,QString::fromUtf8("项目文件类别"));
    model_project->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("标识"));

    ui->tableView_project_type->setModel(model_project);
    //设置表格列宽自适应
    ui->tableView_project_type->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    int p = 0;
//    添加内容
    qDebug() << "1111111111" << my_file_config.config_project_type_map.begin().key();
    for(iter = my_file_config.config_project_type_map.begin(); iter != my_file_config.config_project_type_map.end(); ++iter)
    {
        qDebug() << "6666666666 " << iter.key() << iter.value();
        //str += iter.key() + "---" + iter.value() + "\n";
        if(iter.key().isEmpty() || iter.value().isEmpty())
        {
        }
        else
        {
            model_project->setItem(p,0,new QStandardItem(iter.key()));
            model_project->setItem(p,1,new QStandardItem(iter.value()));
            p++;
        }
    }

    // 显示项目类别
    model_project_type->setColumnCount(2);

    model_project_type->setHeaderData(0,Qt::Horizontal,QString::fromUtf8("项目"));
    model_project_type->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("标识"));

    ui->tableView_project->setModel(model_project_type);
    //设置表格列宽自适应
    ui->tableView_project->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    int q = 0;
//    添加内容
    for(iter = my_file_config.config_project_map.begin(); iter != my_file_config.config_project_map.end(); ++iter)
    {
        //str += iter.key() + "---" + iter.value() + "\n";
        if(iter.key().isEmpty() || iter.value().isEmpty())
        {
        }
        else
        {
            model_project_type->setItem(q,0,new QStandardItem(iter.key()));
            model_project_type->setItem(q,1,new QStandardItem(iter.value()));
            q++;
        }
    }

    //设置表格内容不可修改
    ui->tableView_part->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_type->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::updata_search_info(QStringList strlist,QString table)
{
    QStandardItemModel *model_search = new QStandardItemModel();
    QMap<QString,QString>::Iterator iter;
    QString str = "";
    QStringList search_list;

    connect(ui->tableView_search->horizontalHeader(),SIGNAL(sectionClicked(int)),this, SLOT(slotHeaderClicked(int)));

    //序列号查询
    if(table == "serial_number")
    {
        //显示类型信息
        model_search->setColumnCount(8);
        model_search->setHeaderData(0,Qt::Horizontal,QString::fromUtf8("日期"));
        model_search->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("时间"));
        model_search->setHeaderData(2,Qt::Horizontal,QString::fromUtf8("公司名称"));
        model_search->setHeaderData(3,Qt::Horizontal,QString::fromUtf8("部门/项目"));
        model_search->setHeaderData(4,Qt::Horizontal,QString::fromUtf8("文件类别"));
//        model_search->setHeaderData(5,Qt::Horizontal,QString::fromUtf8("附加"));
        model_search->setHeaderData(5,Qt::Horizontal,QString::fromUtf8("计数"));
        model_search->setHeaderData(6,Qt::Horizontal,QString::fromUtf8("文件名"));
        model_search->setHeaderData(7,Qt::Horizontal,QString::fromUtf8("序列号"));

        ui->tableView_search->setModel(model_search);
        //设置表格列宽自适应
        ui->tableView_search->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        //按降序排列
        ui->tableView_search->sortByColumn(7,Qt::DescendingOrder);
        ui->tableView_search->setSortingEnabled(true);
        ui->tableView_search->setSortingEnabled(true);

        //将数据填入表中
        for(int i=0; i< strlist.count(); i++)
        {
            str = "";
            str = strlist.at(i);
            search_list = str.split(QRegularExpression("\\s+"));
            for(int j=0; j< search_list.count();j++)
            {
                QString s = search_list.at(j);
                if(!s.isEmpty())
                    model_search->setItem(i,j,new QStandardItem(s));
            }
        }
    }
    else if (table == "serial_No")
    {

        //显示类型信息
        model_search->setColumnCount(7);
        model_search->setHeaderData(0,Qt::Horizontal,QString::fromUtf8("日期"));
        model_search->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("时间"));
//        model_search->setHeaderData(2,Qt::Horizontal,QString::fromUtf8("公司名称"));
        model_search->setHeaderData(2,Qt::Horizontal,QString::fromUtf8("部门/项目"));
        model_search->setHeaderData(3,Qt::Horizontal,QString::fromUtf8("文件类别"));
//        model_search->setHeaderData(5,Qt::Horizontal,QString::fromUtf8("附加"));
        model_search->setHeaderData(4,Qt::Horizontal,QString::fromUtf8("计数"));
        model_search->setHeaderData(5,Qt::Horizontal,QString::fromUtf8("文件名"));
        model_search->setHeaderData(6,Qt::Horizontal,QString::fromUtf8("序列号"));

        ui->tableView_search->setModel(model_search);
        //设置表格列宽自适应
        ui->tableView_search->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        //按降序排列
        ui->tableView_search->sortByColumn(7,Qt::DescendingOrder);
        ui->tableView_search->setSortingEnabled(true);
        ui->tableView_search->setSortingEnabled(true);

        //将数据填入表中
        for(int i=0; i< strlist.count(); i++)
        {
            str = "";
            str = strlist.at(i);
            search_list = str.split(QRegularExpression("\\s+"));
            for(int j=0; j< search_list.count();j++)
            {
                QString s = search_list.at(j);
                if(!s.isEmpty())
                    model_search->setItem(i,j,new QStandardItem(s));
            }
        }
    }
    //设置表格内容不可修改
    //ui->tableView_search->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::on_pushButton_create_clicked()
{
    QString res = "";
    QString res_num = "";
    QStringList search_res;
    QStringList serial_list;
    int i=0,index;

    file_name = ui->lineEdit_file_name->text();    
    if(ui->lineEdit_file_name->text().isEmpty())
    {
        QMessageBox::critical(this,tr("提示"), tr("请输入文件名"),QMessageBox::Close);
        return;
    }


    //生成序号
    if(!ui->comboBox_Company->currentText().isEmpty())
    {
        res += "\n==============================\n公司 : ";
        res += ui->comboBox_Company->currentText();
        res_num += company_map.value(ui->comboBox_Company->currentText());
    }
    else
    {
        qDebug() << ui->comboBox_Company->currentText();
        QMessageBox::critical(this,tr("提示"), tr("请输入公司名"),QMessageBox::Close);
        return;
    }

    //获取部门信息
    if(!ui->comboBox_part->currentText().isEmpty())
    {
        res += "\n部门 : ";
        res += ui->comboBox_part->currentText();
        res_num += "-";
        res_num += part_map.value(ui->comboBox_part->currentText());
    }
    else
    {
        QMessageBox::critical(this,tr("提示"), tr("请输入部门/项目信息"),QMessageBox::Close);
        return;
    }

    //获取文件类型
    if(!ui->comboBox_type->currentText().isEmpty())
    {
        res += "\n文件类型 : ";
        res += ui->comboBox_type->currentText();
        res_num += "-";
        res_num += type_map.value(ui->comboBox_type->currentText());
    }
    else
    {
        QMessageBox::critical(this,tr("提示"), tr("请输入文件类别信息"),QMessageBox::Close);
        return;
    }

//    //获取产品型号
//    if(!ui->lineEdit_device->text().isEmpty())
//    {
//        res += "\n产品型号 : ";
//        res += ui->lineEdit_device->text();
//        res += "\n";
//        res_num += "-";
//        res_num += ui->lineEdit_device->text();
//    }

    //查找结果
    QString search_serial = my_sql_op->sql_search_num(res_num,file_name, "serial_number");
    if(search_serial == "exist")
    {
        QString str = "";
        str = "序列号: " + res_num + "\n文件名: " + file_name;
        ui->textEdit_res->append(str);
        ui->textEdit_res->append("\n该序列号对应文件名存在重复,请修改文件名！！！\n");

        return;
    }
    qDebug() << "查找结果序列号:" << search_serial;

    //从查找结果中提取最后一个序列号
    serial_list = search_serial.split("-");
    for(i=0; i < serial_list.count();i++)
    {
        qDebug() << serial_list.at(i);
    }
    index = serial_list.at(--i).toInt() + 1;
    qDebug() << index;
    res_num += "-";
    //res_num += QString::number(index);
    res_num += QString("%1").arg(index,2,10,QLatin1Char('0'));
    ui->textEdit_res->append(res);
    QString str = "文件名 : " + ui->lineEdit_file_name->text();
    ui->textEdit_res->append(str);
    str = "序列号 : "+ res_num;
    ui->textEdit_res->append(str);
    ui->textEdit_res->append("==============================\n");

    //获取当前时间
    QString current_date = getcurrent_date();
    QString current_time = getcurrent_time();


    QString time;
    time = current_date + " " + current_time;
    //qDebug() << time;

    my_sql_op->serial_info.create_time = time;
    //my_sql_op->serial_info.pro_name = ui->lineEdit_project->text();      //公司名称
    my_sql_op->serial_info.pro_name = ui->comboBox_Company->currentText();
    my_sql_op->serial_info.part_code = ui->comboBox_part->currentText(); //部门/项目
    my_sql_op->serial_info.file_type = ui->comboBox_type->currentText(); //文件类别
//    my_sql_op->serial_info.device_id = ui->lineEdit_device->text();      //其他
    my_sql_op->serial_info.count = QString::number(index);           //序号
    my_sql_op->serial_info.description = ui->lineEdit_file_name->text(); //文件名
    my_sql_op->serial_info.serial_number = res_num;


    my_sql_op->sql_add_info(my_sql_op->serial_info,"serial_number");

}

void MainWindow::on_pushButton_find_clicked()
{
    SERIAL_INFO search;
    QStringList res;
    search.description = ui->lineEdit_search_name->text();
    search.pro_name = ui->comboBox_Company->currentText();
//    search.pro_name = ui->lineEdit_project->text();
    if(ui->checkBox_No->isChecked())
    {
        res = my_sql_op->sql_search_info(search, "serial_No");
        updata_search_info(res,"serial_No");
    }
    else
    {
        res = my_sql_op->sql_search_info(search, "serial_number");
        updata_search_info(res,"serial_number");
    }

//    for(int i=0; i < res.size(); i++)
//        ui->textEdit_res->append(res.at(i));


}



void MainWindow::on_pushButton_clear_clicked()
{
    ui->textEdit_res->clear();
}

QString MainWindow::getcurrent_time()
{
    QTime   current_time = QTime::currentTime();
    return current_time.toString();
}

QString MainWindow::getcurrent_date()
{
    QDate   current_date = QDate::currentDate();
    return current_date.toString("yyyy-MM-dd");
}

void MainWindow::on_pushButton_checkout_clicked()
{
    QFileDialog dlg;
    QString filename;

    // tabview没有数据时,点击无效
    if(ui->tableView_search->columnViewportPosition(0) == -1)
    {
        qDebug() << "on_pushButton_checkout_clicked" << -1;
        return ;
    }

    dlg.setAcceptMode(QFileDialog::AcceptSave);
    //  Qt 5
    dlg.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    dlg.setNameFilter("*.xls");
    filename=QDate::currentDate().toString("yyyy-MM-dd")+"body.xls";
    dlg.selectFile(filename);
    if(dlg.exec()!= QDialog::Accepted)
        return;
    QString filePath=dlg.selectedFiles()[0];//得到用户选择的文件名

    if(MainWindow::saveFromTable(filePath,ui->tableView_search,"")) {
        QMessageBox::information(this,tr("提示"),tr("保存成功"));
    }
    else{
        QMessageBox::information(this,tr("提示"),tr("保存失败"));
    }

    //my_sql_op.sql_export_info();
}

//点击表头排序
void MainWindow::slotHeaderClicked(int index)
{
    if(index != 6)
    {
        ui->tableView_search->horizontalHeader()->setSortIndicatorShown(false);
        return;
    }

    static bool bsort = true;
    Qt::SortOrder order = bsort ? (Qt::AscendingOrder) :(Qt::DescendingOrder);
    ui->tableView_search->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableView_search->horizontalHeader()->setSortIndicator(index, order);
    bsort = !bsort;
}

/**********************  保存数据到excel  *********************/

bool MainWindow::saveFromTable(QString filePath, QTableView *tableView, QString comment)
{
    //如果tabview 中没有数据,返回
    if(tableView->columnViewportPosition(0) == -1)
    {
        return false;
    }
    QAbstractItemModel *model=tableView->model();
    const int column=model->columnCount();
    const int row=model->rowCount();

    //header
    qDebug() << "save from table column:" << column << "row:" << row;
    QStringList headers;
    for(int i=0;i<column;i++)
    {
        //隐藏列
        if(tableView->isColumnHidden(i))
            continue;
        headers<<model->headerData(i,Qt::Horizontal).toString();
    }

    //data
    QStringList list;
    QList<QStringList> data;
    for(int i=0;i<row;i++)
    {
        if(model->index(i,0).data().isNull())
            continue;
        list.clear();
        for(int j=0;j<column;j++){
            //隐藏列
            if(tableView->isColumnHidden(j))
                continue;
            list<<model->index(i,j).data().toString();
        }
        data<<list;
    }
    return MainWindow::save(filePath,headers,data,comment);
}

bool MainWindow::save(QString filePath, QStringList headers, QList<QStringList> data,QString comment)
{
    QString sheetName = "Sheet1";

    // 创建一个数据库实例， 设置连接字符串
    QSqlDatabase dbexcel = QSqlDatabase::addDatabase("QODBC","excelexport");
    if(!dbexcel.isValid())
    {
        qDebug()<<"数据库驱动异常";
        return false;   //! type error
    }

    QString dsn = QString("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%1\";DBQ=%2").
                  arg(filePath).arg(filePath);
    qDebug()<<dsn;
    dbexcel.setDatabaseName(dsn);

    // open connection
    if(!dbexcel.open())
    {
         qDebug()<<"无法打开数据库";
        return false;  //! db error
    }

    QSqlQuery query(dbexcel);
    QString sql;

    // drop the table if it's already exists
    sql = QString("DROP TABLE [%1]").arg(sheetName);
    query.exec( sql);
    //create the table (sheet in Excel file)
    sql = QString("CREATE TABLE [%1] (").arg(sheetName);
    foreach (QString name, headers) {
        sql +=QString("[%1] varchar(200)").arg(name);
        if(name!=headers.last())
            sql +=",";
    }
    sql += ")";
    query.prepare(sql);
    if( !query.exec()) {
        //UIDemo01::printError( query.lastError());
        dbexcel.close();
        return false;
    }
    foreach (QStringList slist, data) {
        insert(query,sheetName,slist);
    }
    if(!comment.isEmpty())
    {
        QStringList slist;
        slist<<comment;
        for(int i=0,n=headers.size()-1;i<n;i++)
        {
            slist<<"";
        }
        insert(query,sheetName,slist);
    }

    dbexcel.close();
    return true;
}
bool MainWindow::insert(QSqlQuery &query, QString sheetName, QStringList slist)
{
    QString sSql = QString("INSERT INTO [%1] VALUES(").arg(sheetName);
    for(int i=0,n=slist.size();i<n;i++)
    {
        sSql+=QString(":%1").arg(i);
        if(i!=n-1)
            sSql+=",";
        else
            sSql+=")";
    }
    query.prepare(sSql);
    for(int i=0,n=slist.size();i<n;i++)
    {
        query.bindValue(QString(":%1").arg(i),slist.at(i));
    }
    if( !query.exec()) {

        return false;
    }
    return true;
}

void MainWindow::on_pushButton_back_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}


// 弹窗更新路径
void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index == 2)
    {
        res_index++;
        qDebug() << "i = " << res_index;
    }else
    {
        res_show = 0;
        res_index = 0;
    }
    if(res_index == 5)
    {
        ui->pushButton_update->show();
    }
    else
    {
        ui->pushButton_update->hide();
    }
}

void MainWindow::updata_path(QString str)
{

    qDebug() << "数据库路径已被修改" << str;

    my_sql_op->db.close();
    my_sql_op->sql_open_db(str);
}

// 数据库打开弹窗
void MainWindow::db_change_slot(bool res,QString str)
{
    qDebug() << "数据库提示信息:" << str;
    if(res)
    {
        if(!str.isEmpty())
        {
            ui->textEdit_res->append("OK\n添加数据成功\n");
            QMessageBox::information(this,QObject::tr("数据库操作提示"),str);
        }
    }
    else
    {
        if(!str.isEmpty())
        {
            ui->textEdit_res->append("ERROR\n添加数据失败\n");

            QMessageBox::warning(this,QObject::tr("数据库操作提示"),str);
        }
    }

}

void MainWindow::on_pushButton_help_back_clicked()
{
    res_index = 0;
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_update_clicked()
{
    QMessageBox::warning(this,QObject::tr("配置文件操作"),"修改配置文件");
    // 获取配置文件路径
    QString filename = QFileDialog::getOpenFileName(this,QStringLiteral("选择配置文件"));

    my_file_config.file_read(filename);
    //更新页面显示信息
    company_init();
    company_part_init();
    file_type_init();
    updata_help_info();
}


void MainWindow::on_pushButton_show_clicked()
{
    res_show++;
    if(res_show == 10)
    {
        res_show = 0;
        QMessageBox::warning(this,QObject::tr("数据库操作提示"),"修改数据库路径");
        // 获取配置文件路径
        QString filename = QFileDialog::getOpenFileName(this,QStringLiteral("选择配置文件"));

        if(filename.isEmpty())
            return;

        QFile config_file(filename);
        if(config_file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            //从文件中读取新的路径
            QString new_path = config_file.readLine();
            if(new_path.split('.').endsWith("db"))
            {
                qDebug() << "config_file1 " << new_path;
                updata_path(new_path);
            }
            else
            {
                qDebug() << "config_file2 " << new_path;
            }
        }
    }
}


void MainWindow::on_pushButton_create_No_clicked()
{

    QString res = "";
    QString res_num = "";
    QStringList search_res;
    QStringList serial_list;
    int i=0,index;

    //获取当前时间
    QDate date = QDate::currentDate();

    QString current_date = getcurrent_date();
    QString current_time = getcurrent_time();

    file_name = ui->lineEdit_file_name->text();
    if(ui->lineEdit_file_name->text().isEmpty())
    {
        QMessageBox::critical(this,tr("提示"), tr("请输入文件名"),QMessageBox::Close);
        return;
    }

    //生成No
    if(ui->comboBox_Company->currentText().isEmpty())
    {
//        QMessageBox::critical(this,tr("提示"), tr("请输入公司名"),QMessageBox::Close);
//        return;
    }
    else
    {
        res += "\n==============================\n公司 : ";
        res += ui->comboBox_Company->currentText();
    }

    //获取部门信息
    if(!ui->comboBox_part->currentText().isEmpty())
    {
        res += "\n部门 : ";
        res += ui->comboBox_part->currentText();
        res_num += part_map.value(ui->comboBox_part->currentText());
        res_num += ".";
    }
    else
    {
    }

    //获取文件类型
    if(!ui->comboBox_type->currentText().isEmpty())
    {
        res += "\n文件类型 : ";
        res += ui->comboBox_type->currentText();

    }
    else
    {
//        QMessageBox::critical(this,tr("提示"), tr("请输入文件类别信息"),QMessageBox::Close);
//        return;
    }

    //获取产品型号
//    if(!ui->lineEdit_device->text().isEmpty())
    {
//        res += "\n产品型号 : ";
//        res += ui->lineEdit_device->text();
//        res += "\n";
//        res_num += "-";
//        res_num += ui->lineEdit_device->text();
    }
    res_num += date.toString("yyyyMMdd");

    //查找No数据库结果
    qDebug() <<"file_name111 " << file_name << "res_num" << res_num;
    QString search_serial = my_sql_op->sql_search_num(res_num,file_name, "serial_No");
    qDebug() << "查找结果序列号:" << search_serial;
    if(search_serial == "exist")
    {
        QString str = "";
        str = "No号: " + res_num + "\n文件名: " + file_name;
        ui->textEdit_res->append(str);
        ui->textEdit_res->append("\n该序列号对应文件名存在重复,请修改文件名！！！\n");
        return;
    }


    //从查找结果中提取最后一个序列号
    serial_list = search_serial.split("-");
    for(i=0; i < serial_list.count();i++)
    {
        qDebug() << serial_list.at(i);
    }
    index = serial_list.at(--i).toInt() + 1;
    qDebug() << index;
    res_num += QString("%1").arg(index,3,10,QLatin1Char('0'));
    ui->textEdit_res->append(res);
    QString str = "文件名 : " + ui->lineEdit_file_name->text();
    ui->textEdit_res->append(str);
    str = "No号 : "+ res_num;
    ui->textEdit_res->append(str);
    ui->textEdit_res->append("==============================\n");




    QString time;
    time = current_date + " " + current_time;
    //qDebug() << time;

    my_sql_op->serial_info.create_time = time;
//    my_sql_op->serial_info.pro_name = ui->lineEdit_project->text();      //公司名称
    my_sql_op->serial_info.pro_name = ui->comboBox_Company->currentText();      //公司名称
    my_sql_op->serial_info.part_code = ui->comboBox_part->currentText(); //部门/项目
    my_sql_op->serial_info.file_type = ui->comboBox_type->currentText(); //文件类别
//    my_sql_op->serial_info.device_id = ui->lineEdit_device->text();      //其他
    my_sql_op->serial_info.count = QString::number(index);           //序号
    my_sql_op->serial_info.description = ui->lineEdit_file_name->text(); //文件名
    my_sql_op->serial_info.serial_number = res_num;


    qDebug() << res_num;
    my_sql_op->sql_add_info(my_sql_op->serial_info,"serial_No");
}




