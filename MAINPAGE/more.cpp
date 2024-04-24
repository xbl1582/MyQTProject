#include "more.h"
#include "ui_more.h"
#include "QDebug"
#include "more.h"
#include "more.h"
More::More(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::More)
{
    ui->setupUi(this);

    this->initmore();


}

More::~More()
{
    delete ui;
}

void More::changewidget(int i)
{
    //
}



void More::initmore()
{
    this->setWindowIcon(QIcon(appcurrentpath+"/image/logo/KSTMAIN.ico"));
    //
    appcurrentpath=QCoreApplication::applicationDirPath();
    basepath=appcurrentpath.mid(0,appcurrentpath.lastIndexOf("KSTMAIN"));

    readconfigforIPService();
    readconfigforsystemsetting();

    ui->pushButton_2->setIcon(QIcon(appcurrentpath+"/image/return.png"));
    ui->pushButton_2->setIconSize(QSize(50,50));

    connect( ui->pushButton_2,&QPushButton::clicked,this,[=](){
        //关闭更多页面

        this->close();
    });
    ui->pushButton->setIcon(QIcon(appcurrentpath+"/image/ter.png"));
    ui->pushButton->setIconSize(QSize(50,50));
    connect( ui->pushButton,&QPushButton::clicked,this,[=](){
        ShowTreaim();
    });

    ui->pushButton_5->setIcon(QIcon(appcurrentpath+"/image/authorization.png"));
    ui->pushButton_5->setVisible(false);
    ui->pushButton_5->setIconSize(QSize(50,50));
    connect( ui->pushButton_5,&QPushButton::clicked,this,[=](){
        checkauthorization();
    });

    ui->pushButton_6->setIcon(QIcon(appcurrentpath+"/image/appcheck.png"));
    ui->pushButton_6->setIconSize(QSize(50,50));
    ui->pushButton_6->setVisible(false);
    connect( ui->pushButton_6,&QPushButton::clicked,this,[=](){
        retrievalapps();
    });

    appcurrentpath=QCoreApplication::applicationDirPath();
    ui->pushButton_3->setIcon(QIcon(appcurrentpath+"/image/setting.png"));
    ui->pushButton_3->setIconSize(QSize(50,50));
    connect(ui->pushButton_3,&QPushButton::clicked,this,[=](){
        //打开系统设置服务
        showsetting();
    });

    appcurrentpath=QCoreApplication::applicationDirPath();
    ui->pushButton_4->setIcon(QIcon(appcurrentpath+"/image/app.png"));
    ui->pushButton_4->setIconSize(QSize(50,50));
    connect(ui->pushButton_4,&QPushButton::clicked,this,[=](){
        //打开应用服务
        showinstallservice();
    });
    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
    //----------------------------------
    this->screenwidth=screenRect.width();
    this->screenhight=screenRect.height();
    //----------------------------------
    //    this->resize(this->screenwidth,this->screenhight);
    //----------------------------------
    ui->horizontalWidget->resize(this->size());

    //----------------------------------

    installService=new InstallService();
    installService->resize(this->size());
    connect(installService, SIGNAL(successtomore()), this, SLOT(recivesuccessforapps()));
    connect(installService, SIGNAL(sendhavenewtomore(int)), this, SLOT(receivehavenew(int)));
    ui->verticalLayout_9->addWidget(installService);
    emit sendhavenewtomainwindow(active);
    QDir dir(appcurrentpath+"/lib/SSN/");
    if(!dir. exists()){
        dir.mkdir(appcurrentpath+"/lib/SSN/");
        //download the YYSN softe
        //http://192.168.10.22:9988/driver/download?path=Driver/YYSN_rtm_arm_13.1.0.1001_20240327.zip

        //unzip this zip and chmod 7777 to it



    }
    creatdatabase();
    sql_query=new QSqlQuery(database);
      getapppaths();
    checkauthorization();
    retrievalapps();

}






void More::showsetting()
{
    if(installService!=nullptr){
        delete(installService);
        installService=nullptr;
        ui->verticalLayout_9->removeWidget(installService);
    }
    if(systemsetting==nullptr){
        systemsetting=new SystemSetting();
        systemsetting->changesetshow();
        systemsetting->onResize(supersize);
        connect(systemsetting, SIGNAL(sendlange(QString,int)), this, SLOT(receivelange(QString,int )));
        ui->verticalLayout_9->addWidget(systemsetting);
    }
}

void More::showinstallservice()
{

    if(systemsetting!=nullptr){
        delete(systemsetting);
        systemsetting=nullptr;

        ui->verticalLayout_9->removeWidget(systemsetting);
    }

    if(installService==nullptr){
        installService=new InstallService();
        installService->onResize(supersize);
        connect(installService, SIGNAL(successtomore()), this, SLOT(recivesuccessforapps()));
        connect(installService, SIGNAL(sendhavenewtomore(int)), this, SLOT(receivehavenew(int)));
        ui->verticalLayout_9->addWidget(installService);

    }

}
void More:: reloadinstallservice(){
    if(systemsetting!=nullptr){
        delete(systemsetting);
        systemsetting=nullptr;

        ui->verticalLayout_9->removeWidget(systemsetting);
    }

    if(installService==nullptr){
        installService=new InstallService();
        installService->onResize(supersize);
        connect(installService, SIGNAL(successtomore()), this, SLOT(recivesuccessforapps()));
        connect(installService, SIGNAL(sendhavenewtomore(int)), this, SLOT(receivehavenew(int)));
        ui->verticalLayout_9->addWidget(installService);

    }
    if(installService!=nullptr){
        delete(installService);
        installService=nullptr;
        ui->verticalLayout_9->removeWidget(installService);
        installService=new InstallService();
        installService->onResize(supersize);
        connect(installService, SIGNAL(successtomore()), this, SLOT(recivesuccessforapps()));
        connect(installService, SIGNAL(sendhavenewtomore(int)), this, SLOT(receivehavenew(int)));
        ui->verticalLayout_9->addWidget(installService);

    }
}


void More::receivelange(QString mylange,int type)
{
    lange=mylange;
    devicetype=type;
      ui->retranslateUi(this);
    emit sendlange(lange,devicetype);
}
void More::receivehavenew(int myactive){
    qDebug()<<"more:receivehavenew";
    active=myactive;
    emit sendhavenewtomainwindow(myactive);
}

void More::recivesuccessforapps()
{
    qDebug()<<"--------------------recivesuccessforapps-----------------------";
    emit successtomainwindow();
}

void More::ShowTreaim()
{
    system("weston-terminal &");
}

void  More::onResize(QSize size)
{
    this->resize(size);
    supersize=size;
    ui->horizontalWidget->resize(this->size());

    if(installService!=nullptr){
        installService->onResize(QSize(this->size().width(),this->size().height()));
    }
    if(systemsetting!=nullptr){
        systemsetting->resize(QSize(this->size().width(),this->size().height()));
    }

}

void More::showButton(bool returnbutton, bool terbutton)
{
    ui->pushButton_2->setVisible(returnbutton);
    ui->pushButton->setVisible(terbutton);
}
//检查ssn
void More::checkauthorization()
{

    QDir dir(appcurrentpath+"/lib/SSN/");
    QFile file(appcurrentpath+"/lib/SSN/libyy.so");
    //1.判断存储ssn文件夹是否存在
    qDebug()<<appcurrentpath+"/lib/SSN/:"+(dir.exists()?"true":"false");
    qDebug()<<appcurrentpath+"/lib/SSN/libyy.so:"+(file.exists()?"true":"false");
    if(dir.exists()&&file.exists()){
        ui->pushButton_5->setText(tr("已授权"));
        ui->pushButton_5->setDisabled(true);
        //判断数据库安装了那些软件
        QString selectapplistsql="select * from appsversion";
        QList<AppsVersion *>appversions=selectAppsversion(selectapplistsql);
        int result;
        for(int i=0;i<appversions.size();i++){

            qDebug()<<appversions.at(i)->getAppname();
            if(appversions.at(i)->getAppname()=="KST"){
                qDebug()<<basepath+""+appversions.at(i)->getAppname()+"/libyy.so";
                QFile tempfile(basepath+appversions.at(i)->getAppname()+"/libyy.so");
                if(!tempfile.exists()){
                    result=copyfile(appcurrentpath+"/lib/SSN/libyy.so",basepath+appversions.at(i)->getAppname()+"/");

                    if(!(System_Check(result))){
                        //showmessage("授权"+appversions.at(i)->getAppname()+"成功",nullptr);
                    }else{
                        showmessage(tr("授权")+appversions.at(i)->getAppname()+tr("失败"),nullptr);
                    }
                }
                qDebug()<<basepath+appversions.at(i)->getAppname()+"/UYEditor/libyy.so";
                QFile tempfile2(basepath+appversions.at(i)->getAppname()+"/UYEditor/libyy.so");
                if(!tempfile2.exists()){
                    int result2=copyfile(appcurrentpath+"/lib/SSN/libyy.so",basepath+appversions.at(i)->getAppname()+"/UYEditor/");
                    if(!(System_Check(result2))){
                        //showmessage("授权"+appversions.at(i)->getAppname()+"成功",nullptr);
                    }else{
                        showmessage(tr("授权")+appversions.at(i)->getAppname()+"->UYEditor"+tr("失败"),nullptr);
                    }
                }

            }else{

                QFile tempfile2(basepath+appversions.at(i)->getAppname()+"/libyy.so");
                if(!tempfile2.exists()){
                    result=copyfile(appcurrentpath+"/lib/SSN/libyy.so",basepath+appversions.at(i)->getAppname()+"/");
                    if(!(System_Check(result))){
                        //showmessage("授权"+appversions.at(i)->getAppname()+"成功",nullptr);
                    }else{
                        showmessage(tr("授权")+appversions.at(i)->getAppname()+tr("失败"),nullptr);
                    }
                }

            }
        }

        return;
    }else
        if(dir.exists()){
            //1.1 获取文件信息列表
            QFileInfoList list = dir.entryInfoList ();
            //1.2 遍历文件列表
            for(int i=0;i<list.size();i++){
                //1.3 获取文件基本信息
                QFileInfo qfileinfo=list.at(i);
                //1.4 判断是否存在以前缀未YYSN后缀未txt的文件
                if(qfileinfo.fileName().startsWith("YYSN_")&&qfileinfo.fileName().endsWith(".txt")){
                    //上传txt文件,请求是否有zip文件，有下载
                    //1.4.1 文件如果存在，先显示受理
                    ui->pushButton_5->setText("受理中...");
                    //1.4.2 如果sn文件存在，同时libyy.so文件也存在
                    if(file.exists()){
                        //1.4.2.1 显示已经授权
                        ui->pushButton_5->setText(tr("已授权"));
                        return;
                    }else{
                        //1.4.2.2 下载lic文件
                        //请求根据txt名称请求，并下载
                        QList<EmpowerPoJo *> qlist=getempower(qfileinfo.fileName());
                        if(qlist.size()==0){
                            //                        qDebug()<<"get the lic is empty ";
                            postFileTask(qfileinfo.filePath(),qfileinfo.fileName());
                            //                        qDebug()<<"如果不存在就显示";
                            ui->pushButton_5->setText(tr("受理中..."));
                            ui->pushButton_5->setFlat(true);
                            ui->pushButton_5->setStyleSheet("QPushButton{color:red;}");
                            return;
                        }else{
                            //下载sn对应的lic，
                            ui->pushButton_5->setText(tr("下载中..."));
                            ui->pushButton_5->setFlat(true);
                            ui->pushButton_5->setStyleSheet("QPushButton{color:red;}");
                            //存在就下载下来
                            for(int i=0;i<qlist.size();i++){
                                if(qlist.at(i)->getSn()==qfileinfo.fileName()&&
                                        (qlist.at(i)->getLic()!=nullptr&&qlist.at(i)->getLic()!="")){
                                    QString fullrequest= ipservicepojo->getUrltype()+"://"+ ipservicepojo->getIp()+":"+QString::number(ipservicepojo->getPort())
                                            +"/empower/download?path=empower/"+qlist.at(i)->getLic();

                                    bool successdownload=getdownfun(fullrequest,qlist.at(i)->getLic());
                                    //解压
                                    bool succesunzip=false;
                                    if(successdownload){
                                        ui->pushButton_5->setText(tr("解压中..."));
                                        ui->pushButton_5->setFlat(true);
                                        ui->pushButton_5->setStyleSheet("QPushButton{color:red;}");
                                        succesunzip=unzip(appcurrentpath+"/lib/SSN/zip/"+qlist.at(i)->getLic());
                                    }
                                    //存储位置 主程序目录，
                                    // KST：主程序目录+UYEditor
                                    if(succesunzip){

                                        QString libyysofilestr=appcurrentpath+"/lib/SSN/unzip/libyy.so";
                                        QString libyysofilestr2=appcurrentpath+"/lib/SSN/unzip/"+qlist.at(i)->getLic().replace(".zip","")+"/libyy.so";
                                        qDebug()<<libyysofilestr2;
                                        QFile libyysofile(libyysofilestr);
                                        QFile libyysofile2(libyysofilestr2);
                                        if(libyysofile.exists()||libyysofile2.exists()){
                                            QString libyysofilestrtemp="";
                                            if(libyysofile.exists()){
                                                libyysofilestrtemp=libyysofilestr;
                                                copyfile(libyysofilestr,appcurrentpath+"/lib/SSN/");
                                            }else{
                                                libyysofilestrtemp=libyysofilestr2;
                                                copyfile(libyysofilestr2,appcurrentpath+"/lib/SSN/");
                                            }
                                            //判断数据库安装了那些软件
                                            QString selectapplistsql="select * from appsversion";
                                            QList<AppsVersion *>appversions=selectAppsversion(selectapplistsql);
                                            int result;
                                            for(int i=0;i<appversions.size();i++){
                                                qDebug()<<appversions.at(i)->getAppname();
                                                if(appversions.at(i)->getAppname()=="KST"){
                                                    result=copyfile(libyysofilestrtemp,basepath+"/"+appversions.at(i)->getAppname()+"/");

                                                    if(!(System_Check(result))){
                                                        //showmessage("授权"+appversions.at(i)->getAppname()+"成功",nullptr);
                                                    }else{
                                                        showmessage(tr("授权")+appversions.at(i)->getAppname()+tr("失败"),nullptr);
                                                    }
                                                    int result2=copyfile(libyysofilestrtemp,basepath+"/"+appversions.at(i)->getAppname()+"/UYEditor/");
                                                    if(!(System_Check(result2))){
                                                        //showmessage("授权"+appversions.at(i)->getAppname()+"成功",nullptr);
                                                    }else{
                                                        showmessage(tr("授权")+appversions.at(i)->getAppname()+"->UYEditor"+tr("失败"),nullptr);
                                                    }
                                                }else{
                                                    result=copyfile(libyysofilestrtemp,basepath+"/"+appversions.at(i)->getAppname()+"/");
                                                    if(!(System_Check(result))){
                                                        //showmessage("授权"+appversions.at(i)->getAppname()+"成功",nullptr);
                                                    }else{
                                                        showmessage(tr("授权")+appversions.at(i)->getAppname()+tr("失败"),nullptr);
                                                    }
                                                }
                                            }
                                            if(!(System_Check(result))){
                                                if(libyysofile2.exists()){
                                                    QString commond="rm -rf "+libyysofilestr2.replace("/libyy.so","");
                                                    const char* code=commond.toUtf8().data();
                                                    system(code);
                                                }
                                                //                                            showmessage("授权成功",nullptr);
                                            }else{
                                                showmessage(tr("授权失败"),nullptr);
                                            }
                                        }

                                        break;
                                    }
                                }
                            }


                            return;
                        }
                    }

                }
                else{
                    if(i==list.size()-1){
                        ui->pushButton_5->setText(tr("未授权"));

                        QString commond="cd "+appcurrentpath+"/lib/SSN && ./YYSN ";
                        QFile qfile(appcurrentpath+"/lib/SSN/YYSN");
                        if(qfile.exists()){
                            const char* code=commond.toUtf8().data();
                            system(code);
                            QDir dir2(appcurrentpath+"/lib/SSN/");
                            QFileInfoList list2 = dir2.entryInfoList ();// 获取文件信息列表
                            for(int i=0;i<list2.size();i++){
                                QFileInfo qfileinfo=list2.at(i);
                                //                            qDebug()<<qfileinfo.fileName();
                                if(qfileinfo.fileName().startsWith("YYSN_")&&qfileinfo.fileName().endsWith(".txt")){
                                    postFileTask(qfileinfo.absolutePath()+"/"+qfileinfo.fileName(),qfileinfo.fileName());
                                    ui->pushButton_5->setText(tr("受理中..."));
                                    ui->pushButton_5->setFlat(true);
                                    ui->pushButton_5->setStyleSheet("QPushBUtton{color:red;}");
                                }
                            }


                        }else{
                            downloadYYSN();
                            //                        showmessage("出错了，YYSN不存在",nullptr);

                        }
                        //上传txt文件，提示后台受理
                    }

                }
            }

        }
        else{
            dir.mkdir(appcurrentpath+"/lib/SSN/");
            //下载YYSN程序
            downloadYYSN();
        }
}



//检索文件夹，获取应用安装列表
void More::retrievalapps()
{

    //    ui->pushButton_6->setDisabled(true);
    //步骤1: 获取当前文件位置，根据文件为所在位置，截取到上一层的路径，
    appcurrentpath=QCoreApplication::applicationDirPath();
    basepath=appcurrentpath.mid(0,appcurrentpath.lastIndexOf("KSTMAIN"));
    creatdatabase();
    sql_query=new QSqlQuery(database);
    //步骤2：通过遍历文件夹获取，子文件夹以及一个叫version.txt的文件，根据这个文件里的内容获取版本号
    QDir qdir(basepath);
    //    qDebug()<<basepath;
    if(!qdir.exists())
    {
        return;
    }
    qdir.setFilter(QDir::Dirs | QDir::NoSymLinks);
    QFileInfoList list = qdir.entryInfoList();
    int folder_count = list.count();
    if(folder_count <= 0)
    {
        return;
    }


    for(int i=0; i<folder_count; i++)
    {
        QFileInfo file_info = list.at(i);
        if(file_info.fileName()!="."&&file_info.fileName()!=".."){
            for(int i=0;i<apppaths.size();i++){
                if(file_info.fileName()==apppaths.at(i)->getPath()){
                    folderlist.append(file_info.fileName());
                }else{
                    continue;
                }
            }

        }

    }
    //    qDebug()<<QString::number(folderlist.size());
    for(int i=0;i<folderlist.size();i++){
        QDir qdir2(basepath+"/"+folderlist.at(i));
        QFileInfoList sonlist = qdir2.entryInfoList();
        for(int j=0;j<sonlist.size();j++){

            QFileInfo file_info=sonlist.at(j);
            //            qDebug()<<file_info.fileName();
            if(file_info.fileName()=="version.txt"){
                //                qDebug()<<file_info.filePath();
                versionpathlist.append(file_info.filePath());
                break;
            }
        }


    }
    //通过文件所在的文件夹以及文件名，进行数据库修改(添加或修改)
    for(int j=0;j<versionpathlist.size();j++){
        QFile f(versionpathlist.at(j));
        if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << ("打开文件失败");
        }
        QTextStream txtInput(&f);
        QString lineStr;
        QString lineStr2;
        while(!txtInput.atEnd())
        {
            lineStr = txtInput.readAll();
            lineStr2=lineStr;

        }
        f.close();
        QString versionstr=lineStr.replace("\.","");
        int version=versionstr.toInt();
        QStringList temparray=versionpathlist.at(j).split("/version.txt")[0].split("/");
        QString path=temparray[temparray.length()-1];
        //        qDebug()<<path;
        QString sql2="select * from appsversion where apppath='"+path+"'";

        QList<AppsVersion *>appversions2=selectAppsversion(sql2);
        //        qDebug()<<QString::number(appversions2.size());
        if(appversions2.size()==0){

            //步骤3：读取版本号，生成数据库插入的内容，主要的字段需要有：
            //uuid(自动生成),软件名appname,软件路径apppath,
            //完整包版本号apprtmversion,完整包版本号apprtmversionchar，完整包安装时间apprtmupdatetime
            //更新包版本appupdatedversion，更新包版本号appupdatedversionchar，更新包安装时间appupdatedtime
            //软件大小appsize，软件ico，系统appsystem，类型app类型，显示优先priority
            //软件名词，软件版本(根据当前配置文件写入)，

            int priority=0;
            if(path=="KSTPXQ"){
                priority=9;
            }else if(path=="KSTPXX"){
                priority=9;
            }else if(path=="WTKPK"){
                priority=8;
            }
            else if(path=="KSTSCAN"){
                priority=7;
            }else if(path=="KST"){
                priority=6;
            }else if(path=="KSTLX"){
                priority=5;
            }else if(path=="WTKP"){
                priority=4;
            }
            else if(path=="YYEditor"){
                priority=3;
            }
            else if(path=="KSTSP"){
                priority=2;
            }else{
                priority=0;
            }
            QUuid id = QUuid::createUuid();
            QString strId = id.toString();
            QString uuid=strId.replace("-","");
            QFileInfo qfile(f);
            QString size=size_human(qfile.path());
            QString createtime=qfile.created().toString("yyyyMMdd");
            QString installsql=QString("INSERT INTO appsversion"
                                       "(uuid, appname, apppath, "
                                       "apprtmversion,apprtmversionchar,"
                                       "apprtmupdatetime,appupdatedversionchar,appupdatedversion,appupdatedtime, "
                                       "appupdateddesc, appsize, appico, "
                                       "appsystem, appdevice,priority)"
                                       "VALUES('"+uuid+"','"+path+"','"
                                       +path+"',"+QString::number(0)+",'"+QString::number(0)+"',"
                                       +createtime+",'"+lineStr2+"',"+QString::number(version)+","+createtime+
                                       +","+"'应用描述','"+size+"','"+path+".png','"+ipservicepojo->getSystemtype()+"',"
                                       +QString::number(ipservicepojo->getDevicetype())+","+QString::number(priority)+")");

            if(database.isOpen()){
                todosql(installsql);

            }else{
                database.open();
                todosql(installsql);

            }
            closedatabase();

        }else{
            QString sql="select * from appsversion where apppath='"+path+"' and (apprtmversion>="+QString::number(version)+" or appupdatedversion>="+QString::number(version)+")";
            QList<AppsVersion *>appversions=selectAppsversion(sql);
            if(appversions.size()==0){
                //                           //获取的为空的进行安装或修改
                //
                QDateTime dateTime(QDateTime::currentDateTime());
                QString qStr = dateTime.toString("yyyyMMdd");
                QString installsql=QString("UPDATE appsversion SET appupdatedversion ="+QString::number(version)+
                                           ",appupdatedversionchar='"+lineStr2+"', appupdatedtime="+qStr+" where apppath='"+path+"'");
                if(database.isOpen()){
                    todosql(installsql);

                }else{
                    database.open();
                    todosql(installsql);

                }

            }

        }






    }
    recivesuccessforapps();
    reloadinstallservice();


}

void More::creatdatabase()
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
}


QList<AppsVersion *>  More::selectAppsversion(QString sql)
{

    QList<AppsVersion *>appsversions;
    qDebug()<<sql;
    if(database.isOpen()){

        sql_query->exec(sql);

    }else{

        database.open();
        sql_query->exec(sql);

    }
    if(!sql_query->exec())
    {
        qDebug()<<sql_query->lastError();

    }
    else
    {

        while(sql_query->next())
        {

            AppsVersion *appsversion=new AppsVersion();

            appsversion->setId(sql_query->value(0).toInt());

            appsversion->setUuid(sql_query->value(1).toString());

            appsversion->setAppname(sql_query->value(2).toString());

            appsversion->setApppath(sql_query->value(3).toString());

            appsversion->setApprtmversion(sql_query->value(4).toInt());

            appsversion->setApprtmversionchar(sql_query->value(5).toString());

            appsversion->setApprtmupdatetime(sql_query->value(6).toInt());

            appsversion->setAppupdatedversion(sql_query->value(7).toInt());

            appsversion->setAppupdatedversionchar(sql_query->value(8).toString());

            appsversion->setAppupdatedtime(sql_query->value(9).toInt());

            appsversion->setAppupdateddesc(sql_query->value(10).toString());

            appsversion->setAppsize(sql_query->value(11).toString());

            appsversion->setAppico(sql_query->value(12).toString());

            appsversion->setAppsystem(sql_query->value(13).toString());

            appsversion->setAppdevice(sql_query->value(14).toInt());

            appsversions.append(appsversion);
        }
    }
    return appsversions;

}

void More::closedatabase()
{
    database.close();
}

void More::todosql(QString sql)
{
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        // do something创建数据表
        QString create_sql = sql;
        if(!sql_query->exec())
        {
            qDebug()<<sql_query->lastError();
        }
        else
        {
            while(sql_query->next())
            {
                qDebug()<<sql_query->value(1).toString();
            }
        }
        sql_query->prepare(create_sql);
        if(!sql_query->exec())
        {
            qDebug()<<sql;
            qDebug() << "Error: Fail to do." << sql_query->lastError();
        }
        else
        {
            qDebug() << "Table created!";
        }

    }

}

void More::postFileTask(QString filepath,QString filename)//需要的数据
{
    QNetworkAccessManager* manager= new QNetworkAccessManager;
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/html"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\""+ filename + "\"")); // file为后端定义的key，filename即为excel文件名

    QFile *file = new QFile(filepath);//pathbuf1 为文件路径
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(imagePart);
    QString urlstr= ipservicepojo->getUrltype()+"://"+ ipservicepojo->getIp()+":"+QString::number(ipservicepojo->getPort())
            +"/empower/multiUpload";
    //    qDebug()<<urlstr;
    QUrl url(urlstr);
    qDebug()<<filepath;
    QNetworkRequest netReq;
    netReq.setUrl(url);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(send_xlsdata(QNetworkReply*)));
    manager->post(netReq,multiPart);

}
void More::readconfigforIPService()
{
    //读取的时候创建
    QFile qf(appcurrentpath+"/config/ipserviceconfig.ini");
    if(qf.exists()){

        ipservicesetting = new QSettings (appcurrentpath+"/config/ipserviceconfig.ini", QSettings::IniFormat);//构建函数
        ipservicepojo=new IPServicePoJo();
        //读取配置文件内容
        ipservicepojo->setIp(ipservicesetting->value("/ipserviceconfig/ip").toString());
        ipservicepojo->setPort(ipservicesetting->value("/ipserviceconfig/port").toInt());
        ipservicepojo->setOuturl(ipservicesetting->value("/ipserviceconfig/outurl").toString());
        ipservicepojo->setUrltype(ipservicesetting->value("/ipserviceconfig/urltype").toString());
        ipservicepojo->setDevicetype(ipservicesetting->value("/ipserviceconfig/devicetype").toInt());
        ipservicepojo->setSystemtype(ipservicesetting->value("/ipserviceconfig/systemtype").toString());
        delete(ipservicesetting);
    }

}

QList<EmpowerPoJo *> More::getempower(QString filename)
{
    QList<EmpowerPoJo *> qlist;
    readconfigforIPService();
    QString  request=ipservicepojo->getUrltype()+"://"+ipservicepojo->getIp()+
            ":"+QString::number(ipservicepojo->getPort())+"/empower/empowerbytxtname"
            +"?sn="+filename;
    //    qDebug()<<request+"--------------";

    if(IPLive(ipservicepojo->getIp(), ipservicepojo->getPort(),1000)){
        QJsonObject resultobj=getfun(request);
        int code=getJsonObjInt("code",resultobj);
        qDebug()<<"14.code:"+QString::number(code);
        //请求通过
        if(code==1){
            QJsonArray qja=getJsonobjArray("data",resultobj);
            //            qDebug()<<qja.size();
            for(int i=0;i<qja.size();i++){
                EmpowerPoJo *empowerpojo=new EmpowerPoJo();
                empowerpojo->setId(getJsonObjInt("id",qja[i].toObject()));
                empowerpojo->setUuid(getJsonobjQString("uuid",qja[i].toObject()));
                empowerpojo->setSn(getJsonobjQString("sn",qja[i].toObject()));
                empowerpojo->setLic(getJsonobjQString("lic",qja[i].toObject()));
                empowerpojo->setSnpath(getJsonobjQString("snpath",qja[i].toObject()));
                empowerpojo->setLicpath(getJsonobjQString("licpath",qja[i].toObject()));
                empowerpojo->setLicpath(getJsonobjQString("licpath",qja[i].toObject()));
                empowerpojo->setLicsize(getJsonobjQString("licsize",qja[i].toObject()));
                empowerpojo->setLicmd5code(getJsonobjQString("licmd5code",qja[i].toObject()));
                qlist.append(empowerpojo);
            }
        }else if(code==50001){
            qDebug()<<"15.data empty";

        }
    }
    return qlist;
}
QList<Driverpojo *> More::getdriver(QString filename){

    QList<Driverpojo *> qlist;
    readconfigforIPService();
    QString  request=ipservicepojo->getUrltype()+"://"+ipservicepojo->getIp()+
            ":"+QString::number(ipservicepojo->getPort())+"/driver/driverlist?searchvalue="+filename;
    //    qDebug()<<request+"--------------";

    if(IPLive(ipservicepojo->getIp(), ipservicepojo->getPort(),1000)){
        QJsonObject resultobj=getfun(request);
        int code=getJsonObjInt("code",resultobj);
        qDebug()<<"14.code:"+QString::number(code);
        //请求通过
        if(code==1){
            QJsonArray qja=getJsonobjArray("data",resultobj);
            //            qDebug()<<qja.size();
            for(int i=0;i<qja.size();i++){
                Driverpojo *driverpojo=new Driverpojo();
                driverpojo->setUuid(getJsonobjQString("uuid",qja[i].toObject()));
                driverpojo->setDrivername(getJsonobjQString("drivername",qja[i].toObject()));
                driverpojo->setDriversize(getJsonobjQString("driversize",qja[i].toObject()));
                driverpojo->setType(getJsonobjQString("type",qja[i].toObject()));
                driverpojo->setUpdatetime(getJsonobjQString("updatetime",qja[i].toObject()));
                driverpojo->setDownloadtime(getJsonObjInt("downloadtime",qja[i].toObject()));
                driverpojo->setPath(getJsonobjQString("path",qja[i].toObject()));
                driverpojo->setMd5code(getJsonobjQString("md5code",qja[i].toObject()));
                qlist.append(driverpojo);
            }
        }else if(code==50001){
            qDebug()<<"15.data empty";

        }
    }
    return qlist;
}
void More::readconfigforsystemsetting()
{
    systemsettings=new QSettings (appcurrentpath+"/config/systemsetting.ini", QSettings::IniFormat);
    rote=systemsettings->value("systemconfig/rote").toInt();
    device= systemsettings->value("systemconfig/device").toInt();
    lange=systemsettings->value("systemconfig/lange").toString();
}

QString More::size_human(const QString filePath)
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
quint64 More::getflodersize(const QString filePath)
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


int More::getJsonObjInt(QString key, QJsonObject qjo)
{
    return qjo.value(key).toInt();
}

QJsonArray More::getJsonobjArray(QString key, QJsonObject qjo)
{
    return qjo.value(key).toArray();
}

QString More::getJsonobjQString(QString key, QJsonObject qjo)
{
    return qjo.value(key).toString();
}


QJsonObject More::getfun(QString fullRequest)
{
    qDebug()<<fullRequest;
    QNetworkRequest request;

    request.setUrl(QUrl(fullRequest));
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()),&eventLoop, SLOT(quit()));
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    //对请求的返回异常进行处理

    //解析返回的Json结果
    QByteArray replyData = reply->readAll();
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(replyData, &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug()<<"Data acquisition failed, data format (JSON) incorrect";

    }

    QJsonObject rootObj = jsonDoc.object();

    return rootObj;

}
bool More::IPLive(QString ip, int port, int timeout) {
    QTcpSocket tcpClient;
    tcpClient.abort();
    tcpClient.connectToHost(ip, port);
    //100毫秒没有连接上则判断不在线
    return tcpClient.waitForConnected(timeout);
}

bool More::getdownfun(QString fullrequest, QString fileName)
{
    QNetworkRequest request;
    qDebug()<<fullrequest;
    request.setUrl(QUrl(fullrequest));
    //发送Get请求
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()),&eventLoop, SLOT(quit()));
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    QString folderpath=appcurrentpath+"/lib/SSN/zip";
    QDir qdir(folderpath);
    if(!qdir.exists()){
        qdir.mkdir(folderpath);
    }
    fileName=folderpath+"/"+fileName;
    QFile qfile(fileName);
    if(!qfile.exists()){
        qfile.open(QIODevice::ReadWrite);
    }
    qfile.write(reply->readAll());
    qfile.close();
    return true;
}

bool More::unzip(QString fileName){
    QString folderpath=appcurrentpath+"/lib/SSN/unzip";
    QDir qdir(folderpath);
    if(!qdir.exists()){
        qdir.mkdir(folderpath);
    }
    QFile qfile(fileName);
    //    qDebug()<<fileName;
    QString commond="chmod 777 -R  "+fileName;
    system(commond.toLocal8Bit());
    if(qfile.exists()){
        //         QStringList temprsult =JlCompress::extractDir(fileName,appcurrentpath+"/lib/SSN/unzip");
        if(qfile.fileName().endsWith(".zip")){
            QString commond="unzip -o "+fileName +" -d "+folderpath;
            int result=system(commond.toLocal8Bit());

            if(!(System_Check(result))){
                QString rmcommond2="rm  -f "+fileName;
                const char* commond2=rmcommond2.toUtf8().data();
                system(commond2);
                return true;
            }else{
                return false;
            }
        }
    }else{
        qDebug()<<"file is no exists";
    }

    return false;
}
int More::copyfile(QString orginfile,QString pathfile){
    QString copycommond="cp  -rf "+orginfile+" "+pathfile;
    qDebug()<<copycommond;
    const char* commond=copycommond.toUtf8().data();
    int result=system(commond);


    return result;


}
int More:: System_Check(int result)
{
    if((-1 != result) && (WIFEXITED(result)) && (!(WEXITSTATUS(result))))

        return 0;
    else

        return -1;
}
void More::showmessage(QString txt,QString shellname){

    QMessageBox *msgBox=new QMessageBox(QMessageBox::Information, tr("警告"),txt, QMessageBox::Yes, this);
    msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
    QFont qfont;
    qfont.setPixelSize(20);
    msgBox->setFont(qfont);
    msgBox->setMinimumWidth(200);
    msgBox->setMinimumHeight(200);
    msgBox->setBaseSize(200,200);
    msgBox->setFixedSize(200,200);
    msgBox->setButtonText(QMessageBox::Yes,QString(tr("确定")));  //方式1
    QDesktopWidget *desktop=QApplication::desktop();
    int screenWidth = desktop->width();
    int screenHeight = desktop->height();

    // 计算弹窗的中心位置
    int x = (screenWidth - msgBox->width()) / 2;
    int y = (screenHeight - msgBox->height()) / 2;
    msgBox->move(x,y);

    QAbstractButton *button=msgBox->button(QMessageBox::Yes);
    button->setMinimumWidth(100);
    button->setFont(qfont);
    button->setMinimumHeight(50);
    int result2 = msgBox->exec();
    if(result2==QMessageBox::Yes){
        msgBox->close();
        ui->pushButton_5->setText(tr("未授权"));
        if(shellname!=nullptr){
            QString commond="sh "+ appcurrentpath+"/shellcode/"+ shellname;
            const char*code=commond.toLocal8Bit();
            system(code);
        }
    }else{
        QTimer::singleShot(2000, msgBox,[=]{
            msgBox->close();
            if(shellname!=nullptr){
                QString commond="sh "+ appcurrentpath+"/shellcode/"+ shellname;
                const char*code=commond.toLocal8Bit();
                system(code);
            }
        });
    }


}

void More::downloadYYSN(){



    QList<Driverpojo *> driverlist=getdriver("YYSN_");
    QString fullrequest= ipservicepojo->getUrltype()+"://"+ ipservicepojo->getIp()+":"+QString::number(ipservicepojo->getPort())
            +"/driver/download?path=Driver/YYSN_rtm_arm_13.1.0.1001_20240327.zip";
    QString drivername=driverlist.at(driverlist.size()-1)->getDrivername();
    bool successdownload=getdownfun(fullrequest,drivername);
    bool succesunzip=false;
    if(successdownload){
        ui->pushButton_5->setText(tr("解压中..."));
        ui->pushButton_5->setFlat(true);
        ui->pushButton_5->setStyleSheet("QPushButton{color:red;}");
        succesunzip=unzip(appcurrentpath+"/lib/SSN/zip/"+drivername);
        QFile file(appcurrentpath+"/lib/SSN/unzip/"+drivername+"/YYSN");
        QFile file2(appcurrentpath+"/lib/SSN/unzip/YYSN");
        int result=0;
        if(file.exists()){
            result=copyfile(appcurrentpath+"/lib/SSN/unzip/"+drivername+"/YYSN",appcurrentpath+"/lib/SSN");
        }else if(file2.exists()){
            result=copyfile(appcurrentpath+"/lib/SSN/unzip/YYSN",appcurrentpath+"/lib/SSN");
        }else{
            showmessage(tr("出错了，请重试"),nullptr);
        }
        if(!(System_Check(result))){
            QString  commod="chmod 777 -R "+basepath;
            int result=system(commod.toUtf8().data());
            QString commond="cd "+appcurrentpath+"/lib/SSN && ./YYSN ";
            QFile qfile(appcurrentpath+"/lib/SSN/YYSN");
            if(qfile.exists()){
                const char* code=commond.toUtf8().data();
                system(code);
                QDir dir2(appcurrentpath+"/lib/SSN/");
                QFileInfoList list2 = dir2.entryInfoList ();// 获取文件信息列表
                for(int i=0;i<list2.size();i++){
                    QFileInfo qfileinfo=list2.at(i);
                    qDebug()<<qfileinfo.fileName();
                    if(qfileinfo.fileName().startsWith("YYSN_")&&qfileinfo.fileName().endsWith(".txt")){
                        postFileTask(qfileinfo.absolutePath()+"/"+qfileinfo.fileName(),qfileinfo.fileName());
                        ui->pushButton_5->setText(tr("受理中..."));
                        ui->pushButton_5->setFlat(true);
                        ui->pushButton_5->setStyleSheet("QPushBUtton{color:red;}");
                    }
                }
            }
        }
    }
}
QList<apppathspojo *>More::getapppaths(){

    //请求服务

    selectappPaths();
    QString  request=ipservicepojo->getUrltype()+"://"+ipservicepojo->getIp()+
            ":"+QString::number(ipservicepojo->getPort())+"/zip/getPaths";
    if(IPLive(ipservicepojo->getIp(), ipservicepojo->getPort(), 1000)){
        QJsonObject resultobj=getfun(request);
        int code=getJsonObjInt("code",resultobj);
        qDebug()<<"14.请求结果:"+QString::number(code);
        //请求通过
        if(code==1){
            QJsonArray qja=getJsonobjArray("data",resultobj);
            qDebug()<<qja.size();
            qDebug()<<qja;
            for(int i=0;i<qja.size();i++){

                QString path=qja[i].toString();

                if(path!=""){
                    for(int j=0;j<apppaths.size();j++){

                        if(path==apppaths.at(j)->getPath()){
                            break;
                        }else
                            if(j==apppaths.size()-1){
                                //数据库插入
                                QString insertpathssql="INSERT INTO  apppaths (path) values('"+path+"');";
                                if(database.isOpen()){
                                    todosql(insertpathssql);
                                }else{
                                    database.open();
                                     todosql(insertpathssql);
                                }

                            }

                    }


                }

            }
            closedatabase();


        }else if(code==50001){
            qDebug()<<"15.数据为空";
            qDebug()<<"数据为空";
        }


    }
    return apppaths;
}

void   More::selectappPaths(){

    if(!database.isOpen()){
        database.open();
    }
    sql_query->exec("select * from apppaths");
    if(!sql_query->exec())
    {
        qDebug()<<sql_query->lastError();

    }
    else
    {

        while(sql_query->next())
        {
            apppathspojo * apppath=new apppathspojo();
            apppath->setId(sql_query->value(0).toInt());
            apppath->setPath(sql_query->value(1).toString());
            apppaths.append(apppath);

        }
    }

}
