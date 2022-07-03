#include "file_parse.h"


file_parse::file_parse()
{

//    if(!file_open())
//    {
//        qDebug() << "配置文件不存在";
//    }
//    else
//    {
//        qDebug() << "打开配置文件成功";
//        file_read(CONFIG_PATH);
//    }
    my_file = new QFile(CONFIG_PATH);

    file_read(CONFIG_PATH);

    //公司
    config_company_map.insert("心吉康","XJK");

    //体系部门
    config_part_map.insert("","");
    config_part_map.insert("研发中心","RD");
    config_part_map.insert("质管部","QM");
    config_part_map.insert("生产部","MF");
    config_part_map.insert("市场部","MD");
    config_part_map.insert("管理部","MG");
    config_part_map.insert("采购部","PD");

    //项目
    config_project_map.insert("多参数","P005");
    config_project_map.insert("多通道","P006");

    //体系文件类别

    config_part_type_map.insert("","");
    config_part_type_map.insert("总要求","4.1");
    config_part_type_map.insert("文件要求","4.2");
    config_part_type_map.insert("管理承诺","5.1");
    config_part_type_map.insert("以顾客为关注焦点","5.2");
    config_part_type_map.insert("质量方针","5.3");
    config_part_type_map.insert("策划","5.4");
    config_part_type_map.insert("职责、权限与沟通","5.5");
    config_part_type_map.insert("管理评审","5.6");
    config_part_type_map.insert("资源提供","6.1");
    config_part_type_map.insert("人力资源","6.2");
    config_part_type_map.insert("基础设施","6.3");
    config_part_type_map.insert("工作环境和污染控制","6.4");
    config_part_type_map.insert("产品实现的策划","7.1");
    config_part_type_map.insert("与顾客有关的过程","7.2");
    config_part_type_map.insert("设计和开发","7.3");
    config_part_type_map.insert("采购","7.4");
    config_part_type_map.insert("生产和服务提供","7.5");
    config_part_type_map.insert("监视和测量设备的控制","7.6");
    config_part_type_map.insert("监视和测量","8.2");
    config_part_type_map.insert("不合格品控制","8.3");
    config_part_type_map.insert("数据分析","8.4");
    config_part_type_map.insert("改进","8.5");

    //项目文件类别
    config_project_type_map.insert("","");
    config_project_type_map.insert("风险管理计划","FXJH");
    config_project_type_map.insert("设计开发计划","SJJH");
    config_project_type_map.insert("设计验证计划","YZJH");
    config_project_type_map.insert("设计确认计划","QRJH");
}

bool file_parse::file_open()
{
    QFile file_name(CONFIG_PATH);
    if(!file_name.exists())
        return false;
    else
        return true;
}
bool file_parse::file_read(QString path)
{
    QString str;

    if(my_file->isOpen())
    {
        my_file->close();
    }
    my_file = new QFile(path);

    int company_flag = 0;
    int part_flag = 0;
    int type_flag = 0;

    int project_flag = 0;
    int project_type_flag = 0;

    if(my_file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while(1)
        {
            QString line = QString::fromUtf8(my_file->readLine());
            if(line.contains("[Company_code]")) //公司标志
            {
                company_flag = 1;
                part_flag = 0;
                type_flag = 0;
                project_flag = 0;
                project_type_flag = 0;
                continue;
            }
            if(line.contains("[Department_code]"))  //体系部门标志
            {
                part_flag = 1;
                type_flag = 0;
                company_flag = 0;
                project_flag = 0;
                project_type_flag = 0;
                continue;
                //qDebug() << part_flag;
            }
            if(line.contains("[Project_code]"))    //项目标志
            {
                company_flag = 0;
                part_flag = 0;
                type_flag = 0;
                project_flag = 1;
                project_type_flag = 0;
                continue;
                //qDebug() << type_flag;
            }
            if(line.contains("[system_type]"))    //体系文件标志
            {
                company_flag = 0;
                part_flag = 0;
                type_flag = 1;
                project_flag = 0;
                project_type_flag = 0;
                continue;
                //qDebug() << type_flag;
            }
            if(line.contains("[project_type]"))    //项目文件标志
            {
                company_flag = 0;
                part_flag = 0;
                type_flag = 0;
                project_flag = 0;
                project_type_flag = 1;
                continue;
                //qDebug() << type_flag;
            }

            if(line.isEmpty())
            {
               break;
            }
            if(company_flag == 1)   //获取公司信息
            {
                QStringList strlist = line.split(QRegularExpression("\\s+"));

                //qDebug() << strlist;
                if(strlist.count()>1)
                    config_company_map.insert(strlist.at(0),strlist.at(1));
            }
            if(part_flag == 1)  //获取体系部门信息
            {
                //QStringList strlist = line.split(QLatin1Char('\t'), Qt::SkipEmptyParts);
                QStringList strlist = line.split(QRegularExpression("\\s+"));

                //qDebug() << strlist;
                if(strlist.count()>1)
                    config_part_map.insert(strlist.at(0),strlist.at(1));
            }
            if(type_flag == 1)  // 获取体系文件类别
            {
                QStringList strlist = line.split(QRegularExpression("\\s+"));
                //QStringList strlist = line.split(QLatin1Char('\t'), Qt::SkipEmptyParts);
                //qDebug() << "strlist" << strlist << strlist.count();
                if(strlist.count()>1)
                    config_part_type_map.insert(strlist.at(0),strlist.at(1));
            }
            if(project_flag == 1)  // 获取项目信息
            {
                QStringList strlist = line.split(QRegularExpression("\\s+"));
                //QStringList strlist = line.split(QLatin1Char('\t'), Qt::SkipEmptyParts);
                //qDebug() << "strlist" << strlist << strlist.count();
                if(strlist.count()>1)
                    config_project_map.insert(strlist.at(0),strlist.at(1));
            }
            if(project_type_flag == 1)  // 获取项目文件类别
            {
                QStringList strlist = line.split(QRegularExpression("\\s+"));
                //QStringList strlist = line.split(QLatin1Char('\t'), Qt::SkipEmptyParts);

                if(strlist.count()>1)
                {
//                    qDebug() << "strlist555 " << strlist << strlist.count();
                    config_project_type_map.insert(strlist.at(0),strlist.at(1));
                }

            }
        }

        my_file->close();
        return true;
    }
    else
    {
        qDebug() << "文件打开失败";
        return false;
    }
}
