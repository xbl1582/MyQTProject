#include "appsiteminfo.h"
#include "ui_appsiteminfo.h"
#include <QDesktopWidget>
AppsItemInfo::AppsItemInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppsItemInfo)
{
    ui->setupUi(this);
    QDesktopWidget *desktop = QApplication::desktop();
    this->move((desktop->width() - this->width())/ 2, (desktop->height() - this->height()) /2);

    this->setWindowTitle(tr("占用空间"));
    appcurrentpath=QCoreApplication::applicationDirPath();
    basepath=appcurrentpath.split("/KSTMAIN")[0];

    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
    this->screenwidth=screenRect.width();
    this->screenhight=screenRect.height();
    //    this->setMinimumWidth(this->screenwidth);
    //    this->setMinimumHeight(this->screenhight);
    systemsettings = new QSettings (appcurrentpath+"/config/systemsetting.ini", QSettings::IniFormat);//构建函数
    readconfigforsystemsetting();
    creatdatabase();
    sql_query=new QSqlQuery(database);
    connect(ui->pushButton_7,&QPushButton::clicked,this,[=]{
        this->close();
    });


    ui->pushButton_2->setIcon(QIcon(appcurrentpath+"/image/delete.png"));
    ui->pushButton_2->setIconSize(QSize(30,30));
    connect(ui->pushButton_2,&QPushButton::clicked,this,[=](){
        on_pushButton_delTif_clicked();
    });

    ui->pushButton_3->setIcon(QIcon(appcurrentpath+"/image/delete.png"));
    ui->pushButton_3->setIconSize(QSize(30,30));
    connect(ui->pushButton_3,&QPushButton::clicked,this,[=](){
        on_pushButton_deljpg_clicked();
    });
    ui->pushButton_4->setIcon(QIcon(appcurrentpath+"/image/delete.png"));
    ui->pushButton_4->setIconSize(QSize(30,30));
    connect(ui->pushButton_4,&QPushButton::clicked,this,[=](){
        on_pushButton_delBMP_clicked();
    });
    ui->pushButton_5->setIcon(QIcon(appcurrentpath+"/image/delete.png"));
    ui->pushButton_5->setIconSize(QSize(30,30));
    connect(ui->pushButton_5,&QPushButton::clicked,this,[=](){
        on_pushButton_delZIP_clicked();
    });
    ui->pushButton_6->setIcon(QIcon(appcurrentpath+"/image/delete.png"));
    ui->pushButton_6->setIconSize(QSize(30,30));
    connect(ui->pushButton_6,&QPushButton::clicked,this,[=](){
        on_pushButton_delother_clicked();
    });



    ui->pushButton_8->setIcon(QIcon(appcurrentpath+"/image/deleteall.png"));
    ui->pushButton_8->setIconSize(QSize(30,30));

    connect(ui->pushButton_8,&QPushButton::clicked,this,[=](){
        on_pushButton_Del_clicked();
    });

    ui->pushButton_9->setIcon(QIcon(appcurrentpath+"/image/TIF.png"));
    ui->pushButton_9->setIconSize(QSize(30,30));
    ui->pushButton_10->setIcon(QIcon(appcurrentpath+"/image/JPG.png"));
    ui->pushButton_10->setIconSize(QSize(30,30));
    ui->pushButton_11->setIcon(QIcon(appcurrentpath+"/image/BMP.png"));
    ui->pushButton_11->setIconSize(QSize(30,30));
    ui->pushButton_12->setIcon(QIcon(appcurrentpath+"/image/ZIP.png"));
    ui->pushButton_12->setIconSize(QSize(30,30));


}

AppsItemInfo::~AppsItemInfo()
{
    delete ui;
}

void AppsItemInfo::initAppsItemInfo(AppsVersion *appsversion,QStringList fPath,QStringList f)
{
    this->filePath=fPath;
    this->file=f;
    checkmemory();
    loadFile();
    if(lange=="en"){
        QFile qfile(appcurrentpath+"/image/logo/"+appsversion->getAppico().split(".png")[0]+"en.png");
        if(!qfile.exists()){
            ui->pushButton->setIcon( QIcon(appcurrentpath+"/image/logo/Default.png"));
        }else{
            ui->pushButton->setIcon( QIcon(appcurrentpath+"/image/logo/"+appsversion->getAppico().split(".png")[0]+"en.png"));
        }

        ui->pushButton->setIconSize(QSize(100,100));



    }else{
        QFile qfile(appcurrentpath+"/image/logo/"+appsversion->getAppico().split(".png")[0]+".png");
        if(!qfile.exists()){
            ui->pushButton->setIcon( QIcon(appcurrentpath+"/image/logo/Default.png"));
        }else{
            ui->pushButton->setIcon( QIcon(appcurrentpath+"/image/logo/"+appsversion->getAppico().split(".png")[0]+".png"));
        }
        ui->pushButton->setIconSize(QSize(100,100));


    }
    dialogtitle=changeappname(appsversion->getAppname());
    ui->label->setText(changeappname(appsversion->getAppname()));
    QString appsize=size_human(basepath+"/"+appsversion->getApppath());
    ui->label_2->setText(tr("占用：")+appsize);

    //    QString installsql=QString("UPDATE appsversion SET appsize='"+appsize+"' where uuid='"+appsversion->getUuid()+"' and appsize!='"+appsize+"'");
    ////    qDebug()<<installsql;
    //    if(database.isOpen()){
    //         todosql(installsql);
    //    }else{
    //        database.open();
    //        todosql(installsql);
    //    }

    //   closedatabase();






}
void AppsItemInfo::readconfigforsystemsetting()
{
    rote=systemsettings->value("systemconfig/rote").toInt();
    device= systemsettings->value("systemconfig/device").toInt();
    lange=systemsettings->value("systemconfig/lange").toString();
}
quint64 AppsItemInfo::getflodersize(const QString filePath)
{
    QDir tmpDir(filePath);
    quint64 size = 0;

    /*获取文件列表  统计文件大小*/
    foreach(QFileInfo fileInfo, tmpDir.entryInfoList(QDir::Files))
    {
        size += fileInfo.size();
    }

    /*获取文件夹  并且过滤掉.和..文件夹 统计各个文件夹的文件大小 */
    foreach(QString subDir, tmpDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        size += getflodersize(filePath + QDir::separator() + subDir); //递归进行  统计所有子目录
    }

    return size;


}

QString AppsItemInfo::size_human(const QString filePath)
{
    float num = getflodersize(filePath);
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(num >= 1024.0 && i.hasNext())
    {
        unit = i.next();
        num /= 1024.0;
    }
    /// orignal :: return QString().setNum(num,'f',2)+" "+unit;
    return QString::number(num,'f',2)+" "+unit; // update to this
}

void AppsItemInfo::creatdatabase()
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(appcurrentpath+"/db/KSTMAIN.db");
        database.setUserName("gdwt");
        database.setPassword("gdwt");
    }
    database.open();
}

//数据库操作
void AppsItemInfo::todosql(QString sql)
{

    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        // do something创建数据表
        QString create_sql = sql;
        sql_query->prepare(create_sql);
        if(!sql_query->exec())
        {
            qDebug() << "Error: Fail to do." << sql_query->lastError();
        }
        else
        {
            qDebug() << "success to do !";
        }

    }

}

//数据库操作完成后，最好关闭。
void AppsItemInfo::closedatabase()
{
    database.close();
}
//检测内存
void AppsItemInfo::checkmemory()
{


    //memory

    //设置内存大小数值
    QString shcode="sh GetDisk.sh";
    //qDebug()<<"执行脚本:sh GetDisk.sh";
    QFile f2(appcurrentpath+"/shellcode/GetDisk.sh");
    if(f2.exists()){
        QString commond="sh "+appcurrentpath+"/shellcode/GetDisk.sh";
        const char* commond2=commond.toLocal8Bit().constData();
        system(commond2);

        QFile f(QCoreApplication::applicationDirPath()+"/shellcode/GetDisk_1.txt");
        //qDebug()<<"读取磁盘返回内容"+QCoreApplication::applicationDirPath()+"/shellcode/GetDisk_1.txt";
        if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << ("打开文件失败");

        }
        QTextStream txtInput(&f);
        QString lineStr;
        while(!txtInput.atEnd())
        {
            lineStr = txtInput.readLine();
        }
        f.close();
        memory=lineStr;

        QRegExp rx("\\d+");
        rx.indexIn(memory,0);
        QString numsize=rx.cap(0);


        QString numsizedangwei=memory.at(memory.size()-1);
        //        qDebug()<<"设置硬盘容量:"+memory;
        //       ui->pushButton_2->setText(memory);
        ui->label_13->setText("剩余容量:"+memory);
    }


}

void AppsItemInfo:: loadFile(){
    int t=0;
    int j=0;
    int b=0;
    int z=0;
    int o=0;
    for (int i =0;i<this->file.count();++i) {
        if(!QFile::exists(this->file.at(i)))
            continue;
        if((QFileInfo(this->file.at(i)).suffix()).toUpper()=="TIF")
        {
            t++;
            lt.append(this->file.at(i));
        }
        else if((QFileInfo(this->file.at(i)).suffix()).toUpper()=="JPEG")
        {
            j++;
            lj.append(this->file.at(i));
        }
        else  if((QFileInfo(this->file.at(i)).suffix()).toUpper()=="BMP")
        {
            b++;
            lb.append(this->file.at(i));
        }
        else if((QFileInfo(this->file.at(i)).suffix()).toUpper()=="ZIP")
        {
            z++;
            lz.append(this->file.at(i));
        }
        else
        {
            o++;
            lo.append(this->file.at(i));
        }
    }

    for (int var = 0; var < this->filePath.count(); ++var) {
        if(this->filePath.at(var).isEmpty())
            continue;
        QDir dir(this->filePath.at(var));
        if(!dir.exists())
            continue;
        QFileInfoList file_info_list = GetFileList(this->filePath.at(var));
        if(file_info_list.count()>0)
        {

            QString filter;
            foreach(QFileInfo fileinfo, file_info_list)
            {
                //_filter = _fileinfo.completeSuffix();
                filter = fileinfo.suffix();    //后缀名
                if(filter.toUpper()=="TIF")
                {
                    t++;
                    lt.append(fileinfo.filePath());
                }
                else if(filter.toUpper()=="JPG"||filter.toUpper()=="JPEG")
                {
                    j++;
                    lj.append(fileinfo.filePath());
                }
                else  if(filter.toUpper()=="BMP")
                {
                    b++;
                    lb.append(fileinfo.filePath());
                }
                else if(filter.toUpper()=="ZIP")
                {
                    z++;
                    lz.append(fileinfo.filePath());
                }
                else
                {
                    o++;
                    lo.append(fileinfo.filePath());
                }
            }


        }
    }

    ui->label_8->setText(QString::number(lt.size()));
    ui->label_9->setText(QString::number(lj.size()));
    ui->label_10->setText(QString::number(lb.size()));
    ui->label_11->setText(QString::number(lz.size()));
    ui->label_12->setText(QString::number(lo.size()));
    this->repaint();

}
//递归遍历文件夹
QFileInfoList AppsItemInfo::GetFileList(QString path)
{
    QDir dir(path);
    //列出dir(path)目录文件下所有文件和目录信息，存储到file_list容器
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    //列出dir(path)目录下所有子文件夹
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    //进行子文件夹folder_list递归遍历，将内容存入file_list容器
    for(int i= 0; i != folder_list.size(); i++)
    {
        QString name = folder_list.at(i).absoluteFilePath();
        QFileInfoList child_file_list = GetFileList(name);
        file_list.append(child_file_list);
    }
    return file_list;
}





void AppsItemInfo::on_pushButton_delTif_clicked()
{
    if(lt.count()<1){
        ui->pushButton_2->setDisabled(true);
        return;
    }else{
        ui->pushButton_2->setDisabled(false);
    }
    QString str=+"\n";
    for (int var = 0; var < lt.count(); ++var) {
        QStringList temp=lt.at(var).split("/");
        str+=temp.at(temp.length()-1).toLocal8Bit()+"\n";

    }
    Dialog *d=new Dialog(nullptr,QString(tr("是否清除")+ui->label_8->text()+tr("个TIF文件？")+str));
    d->setWindowTitle(dialogtitle);
    if(d->exec()==QDialog::Rejected)
        return;
    for (int var = 0; var < lt.count(); ++var) {
        QFile f(lt.at(var));
        if(f.exists())
            f.remove();
    }
    loadFile();
}

void AppsItemInfo::on_pushButton_deljpg_clicked()
{
    if(lj.count()<1){
        ui->pushButton_3->setDisabled(true);
        return;
    }else{
        ui->pushButton_3->setDisabled(false);
    }
    QString str=+"\n";
    for (int var = 0; var < lj.count(); ++var) {
        QStringList temp=lj.at(var).split("/");
        str+=temp.at(temp.length()-1)+"\n";

    }
    Dialog *d=new Dialog(nullptr,QString(tr("是否清除")+ui->label_9->text()+tr("个JPG文件？")+str));
    d->setWindowTitle(dialogtitle);
    if(d->exec()==QDialog::Rejected)
        return;
    for (int var = 0; var < lj.count(); ++var) {
        QFile f(lj.at(var));
        if(f.exists())
            f.remove();
    }
    loadFile();
}

void AppsItemInfo::on_pushButton_delBMP_clicked()
{
    if(lb.count()<1){
        ui->pushButton_4->setDisabled(true);
        return;
    }else{
        ui->pushButton_4->setDisabled(false);
    }
    QString str=+"\n";
    for (int var = 0; var < lb.count(); ++var) {
        QStringList temp=lb.at(var).split("/");
        str+=temp.at(temp.length()-1)+"\n";

    }
    Dialog *d=new Dialog(nullptr,QString(tr("是否清除")+ui->label_10->text()+tr("个BMP文件？")+str));
    d->setWindowTitle(dialogtitle);
    if(d->exec()==QDialog::Rejected)
        return;
    for (int var = 0; var < lb.count(); ++var) {
        QFile f(lb.at(var));
        if(f.exists())
            f.remove();
    }
    loadFile();
}

void AppsItemInfo::on_pushButton_delZIP_clicked()
{
    if(lz.count()<1){
        ui->pushButton_5->setDisabled(true);
        return;
    }else{
        ui->pushButton_5->setDisabled(false);
    }
    QString str=+"\n";
    for (int var = 0; var < lz.count(); ++var) {
        QStringList temp=lz.at(var).split("/");
        str+=temp.at(temp.length()-1)+"\n";

    }

    Dialog *d=new Dialog(nullptr,QString(tr("是否清除")+ui->label_11->text()+tr("个ZIP文件？"))+str);
    d->setWindowTitle(dialogtitle);
    if(d->exec()==QDialog::Rejected)
        return;
    for (int var = 0; var < lz.count(); ++var) {
        QFile f(lz.at(var));
        if(f.exists())
            f.remove();
    }
    loadFile();
}

void AppsItemInfo::on_pushButton_delother_clicked()
{
    if(lo.count()<1){
        ui->pushButton_6->setDisabled(true);
        return;
    }else{
        ui->pushButton_6->setDisabled(false);
    }
    QString str=+"\n";
    for (int var = 0; var < lo.count(); ++var) {

        QStringList temp=lo.at(var).split("/");
        str+=temp.at(temp.length()-1)+"\n";

    }

    Dialog *d=new Dialog(nullptr,QString(tr("是否清除")+ui->label_12->text()+tr("个其它文件？"))+str);
    d->setWindowTitle(dialogtitle);
    if(d->exec()==QDialog::Rejected)
        return;
    for (int var = 0; var < lo.count(); ++var) {
        QFile f(lo.at(var));
        if(f.exists())
            f.remove();
    }
    loadFile();
}

void AppsItemInfo::on_pushButton_Del_clicked()
{
    Dialog *d=new Dialog(nullptr,QString(tr("是否清除所有文件？")));
    d->setWindowTitle(dialogtitle);
    if(d->exec()==QDialog::Rejected)
        return;
    for (int var = 0; var < lt.count(); ++var) {
        QFile f(lt.at(var));
        if(f.exists())
            f.remove();
    }
    for (int var = 0; var < lj.count(); ++var) {
        QFile f(lj.at(var));
        if(f.exists())
            f.remove();
    }
    for (int var = 0; var < lb.count(); ++var) {
        QFile f(lb.at(var));
        if(f.exists())
            f.remove();
    }
    for (int var = 0; var < lz.count(); ++var) {
        QFile f(lz.at(var));
        if(f.exists())
            f.remove();
    }
    for (int var = 0; var < lo.count(); ++var) {
        QFile f(lo.at(var));
        if(f.exists())
            f.remove();
    }
    loadFile();
}

void AppsItemInfo::changelabel(QString label)
{
    ui->label->setText(label);
}
QString AppsItemInfo::changeappname(QString appname)
{
    if(appname=="KSTPXX"||appname=="KSTPXQ"){
        if(appname=="KSTPXQ"){
            return appname+tr("-智能电子投票箱");
        }else{
            return appname+tr("-计票（903计票机）");
        }

    }else
        if(appname=="KSTSCAN"){
            return appname+tr("-扫描(903 扫描)");
        }else
            if(appname=="WTKPX"||appname=="WTKP"){
                if(appname=="WTKPX"){
                    return appname+tr("-测评(903 测评机)");
                }else{
                    return appname+tr("-测评系统后台");
                }
            }else if(appname=="KST"){
                return appname+tr("-选举系统后台");
            }else if(appname=="KSTLX"){
                return appname+tr("-另选检查");
            }
            else
                if(appname=="KSTMAIN"){
                    return appname+tr("-启动页（应用管理）");
                }
                else
                    if(appname=="YYEditor"){
                        return appname+tr("-智能模板编辑器");
                    }
                    else
                        if(appname=="KSTSP"){
                            return appname+tr("-扫描加工");
                        }


}
