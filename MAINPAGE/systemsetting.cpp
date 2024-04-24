#include "systemsetting.h"
#include "ui_systemsetting.h"

SystemSetting::SystemSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemSetting)
{
    ui->setupUi(this);
    this->initsystemsetting();

}

SystemSetting::~SystemSetting()
{
    delete ui;
}

void SystemSetting::initsystemsetting()
{

    QRect screenRect=QGuiApplication::primaryScreen()->geometry();

    appcurrentpath=QCoreApplication::applicationDirPath();
    basepath=appcurrentpath.mid(0,appcurrentpath.lastIndexOf("KSTMAIN"));
    //初始化系统基础设计setting
    systemsettings = new QSettings (appcurrentpath+"/config/systemsetting.ini", QSettings::IniFormat);//构建函数
    //初始化初始化网络配置文件
    servicesettings =new QSettings(appcurrentpath+"/config/ipserviceconfig.ini", QSettings::IniFormat);
    //    ui->pushButton_3->setVisible(false);

    ui->pushButton_4->setIconSize(QSize(50,50));
    connect( ui->pushButton_4,&QPushButton::clicked,this,[=](){
        testInternet();
    });

    //创建数据库
    creatdatabase();
    //


    //读取系统基础配置
    device=systemsettings->value("/systemconfig/device").toInt();
    admin=systemsettings->value("/systemconfig/admin").toInt();
    if(admin!=1){
        if(device==1){
            ui->radioButton_3->click();
            ui->radioButton_2->setVisible(false);
            ui->radioButton->setVisible(false);
        }else if(device==2){
            ui->radioButton_2->click();
            ui->radioButton_3->setVisible(false);
            ui->radioButton->setVisible(false);
        }else if (device==3){
            ui->radioButton->click();
            ui->radioButton_2->setVisible(false);
            ui->radioButton_3->setVisible(false);
        }
    }else{
        if(device==1){
            ui->radioButton_3->click();
        }else if(device==2){
            ui->radioButton_2->click();
        }else if (device==3){
            ui->radioButton->click();
        }
    }

    rote=systemsettings->value("/systemconfig/rote").toInt();
    lange=systemsettings->value("/systemconfig/lange").toString();
    if(lange=="cn"){
        ui->comboBox->setCurrentText(tr("简体中文"));

    }else if(lange=="en"){
        ui->comboBox->setCurrentText("English");
    }
    //添加设备类型选择
    bgGroup = new QButtonGroup(this);
    bgGroup->addButton( ui->radioButton,3);
    bgGroup->addButton( ui->radioButton_2,2 );
    bgGroup->addButton( ui->radioButton_3,1 );
    connect(bgGroup,SIGNAL(buttonClicked(int)),this,SLOT(slot_RtnGroupChanged(int)));
    //添加语言选项
    ui->comboBox->addItem(tr("简体中文"));
    ui->comboBox->addItem("English");
    ui->comboBox_2->addItem("http");
    ui->comboBox_2->addItem("https");
    ui->comboBox_3->addItem(QString::number(0));
    ui->comboBox_3->addItem(QString::number(90));
    ui->comboBox_3->addItem(QString::number(180));
    ui->comboBox_3->addItem(QString::number(270));
    ui->comboBox->setCurrentText("简体中文");
    connect(ui->comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(on_comboBox_currentIndexChanged(QString)));
    //    ui->spinBox->setMaximum(360);
    //    ui->spinBox->setMinimum(0);

    //保存设置
    connect( ui->pushButton,&QPushButton::clicked,this,[=](){
        //打开设置页
        savesystemsetting();
    });
    //读取服务设置
    ip=servicesettings->value("/ipserviceconfig/ip").toString();
    port=servicesettings->value("/ipserviceconfig/port").toInt();
    devicetype=servicesettings->value("/ipserviceconfig/devicetype").toInt();
    outurl=servicesettings->value("/ipserviceconfig/outurl").toString();
    urltype=servicesettings->value("/ipserviceconfig/urltype").toString();
    systemtype=servicesettings->value("/ipserviceconfig/systemtype").toString();



    dowonloadpath=servicesettings->value("/router/dowonloadpath").toString();
    dowonloadpathtype=servicesettings->value("/router/dowonloadpathtype").toString();
    getbeastnewrtmapp=servicesettings->value("/router/getbeastnewrtmapp").toString();
    getbeastnewrtmapptype=servicesettings->value("/router/getbeastnewrtmapptype").toString();
    getbestnewupdatedapp=servicesettings->value("/router/getbestnewupdatedapp").toString();

    getbestnewupdatedapptype=servicesettings->value("/router/getbestnewupdatedapptype").toString();
    updatedpath=servicesettings->value("/router/updatedpath").toString();
    updatedpathtype=servicesettings->value("/router/updatedpathtype").toString();
    updatederrorpath=servicesettings->value("/router/updatederrorpath").toString();
    updatederrorpathtype=servicesettings->value("/router/updatederrorpathtype").toString();
    gettododatapath=servicesettings->value("/router/gettododatapath").toString();
    gettododatapathtype=servicesettings->value("/router/gettododatapathtype").toString();

    ui->lineEdit->setText(ip);
    ui->lineEdit_2->setText(QString::number(port));


    int flag=chackInternet(ip, port, 1000);
    if(flag==1){
        ui->label_11->setText(ip+":"+QString::number(port)+tr("---------连接成功"));
        ui->label_11->setStyleSheet("QLabel{color:rgb(8,137,31);font-size:20px;margin-top:20px;}");
    }else if(flag==2){
        ui->label_11->setText(outurl+":"+QString::number(port)+tr("---------连接成功"));
        ui->label_11->setStyleSheet("QLabel{color:rgb(8,137,31);font-size:20px;margin-top:20px;}");
    }else{
        ui->label_11->setText(tr("连接不成功"));
        ui->label_11->setStyleSheet("QLabel{color:rgb(163,54,43);font-size:20px;margin-top:20px;}");
    }

    sql_query=new QSqlQuery(database);
    checkauthorization();


}


void SystemSetting::creatdatabase()
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

void SystemSetting::todosql(QString sql)
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
            qDebug() << "Error: Fail to create table." << sql_query->lastError();
        }
        else
        {
            qDebug() << "Table created!";
        }

    }
}
//保存基础设置
void SystemSetting::savesystemsetting()
{
    rote=ui->comboBox_3->currentText().toInt();
    systemsettings->setValue("systemconfig/rote",rote);
    systemsettings->setValue("systemconfig/device",device);
    systemsettings->setValue("systemconfig/lange",lange);

    ip=ui->lineEdit->text();
    port=ui->lineEdit_2->text().toInt();
    //    urltype=ui->lineEdit_5->text();


    servicesettings->value("/router/updatedpath").toString();
    servicesettings->setValue("ipserviceconfig/ip",ip);
    servicesettings->setValue("ipserviceconfig/outurl",outurl);
    servicesettings->setValue("ipserviceconfig/devicetype",device);
    servicesettings->setValue("ipserviceconfig/port",port);
    servicesettings->setValue("ipserviceconfig/systemtype",systemtype);
    servicesettings->setValue("ipserviceconfig/urltype",urltype);


    QString rotate=" echo \"output:all:rotate"+QString::number(rote)+"\" >> /tmp/.weston_drm.conf";
    const char* commondrotate=rotate.toLocal8Bit().constData();
    system(commondrotate);

    emit sendlange(lange, device);



    msgBox=new QMessageBox(QMessageBox::Warning, tr(" "),tr( "保存成功"), QMessageBox::Yes, this);
    msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
    QFont qf;
    qf.setPixelSize(20);
    msgBox->setFont(qf);
    msgBox->setMinimumWidth(200);
    msgBox->setMinimumHeight(200);
    msgBox->setBaseSize(200,200);

    msgBox->setFixedSize(200,200);
    msgBox->setButtonText(QMessageBox::Yes,QString(tr("关闭")));  //方式1
    QAbstractButton *button=msgBox->button(QMessageBox::Yes);
    button->setMinimumWidth(100);
    button->setFont(qf);
    button->setMinimumHeight(50);
    msgBox->setStyleSheet("QLabel{"

                          "}");
    int result2 = msgBox->exec();


}

void SystemSetting::changesetshow()
{
    device=systemsettings->value("/systemconfig/device").toInt();
    if(device==1){
        ui->radioButton_3->click();
    }else if(device==2){
        ui->radioButton_2->click();
    }else if (device==3){
        ui->radioButton->click();
    }
    rote=systemsettings->value("/systemconfig/rote").toInt();
    lange=systemsettings->value("/systemconfig/lange").toString();
    if(lange=="cn"){
        ui->comboBox->setCurrentText(tr("简体中文"));

    }else if(lange=="en"){
        ui->comboBox->setCurrentText("English");
    }
}

void SystemSetting::onResize(QSize size)
{
    this->resize(size);
    ui->scrollArea->resize(this->size());

}

void SystemSetting::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(ui->comboBox->currentText()=="简体中文"||ui->comboBox->currentText()=="Simplified Chinese")
    {
        lange="cn";
        ui->comboBox->setCurrentText("简体中文");

        changelanguage();
    }

    else if (ui->comboBox->currentText()== "English")
    {
        lange="en";
        ui->comboBox->setCurrentText("English");
        changelanguage();
    }

}

void SystemSetting::slot_RtnGroupChanged(int id)
{
    //    qDebug()<<QString::number(id);
    device=id;

}


int SystemSetting::chackInternet(QString ip, int port, int timeout)
{
    int i=0;

    return IPLive(ip, port,timeout)==true?i=1:(IsWebOk()==true?i=2:i=3);

}
bool SystemSetting::IPLive(QString ip, int port, int timeout) {
    QTcpSocket tcpClient;
    tcpClient.abort();
    tcpClient.connectToHost(ip, port);
    //100毫秒没有连接上则判断不在线
    return tcpClient.waitForConnected(timeout);
}
//判断是否通外网
bool SystemSetting::IsWebOk() {
    //能接通百度IP说明可以通外网
    return IPLive(outurl, 9988);
}

void SystemSetting::changelanguage()
{

    if(lange=="en"){


        m_translator=new QTranslator();
        QString langqmfilepath = appcurrentpath + QString("/language/KSTMAIN_en_AS.qm");
        if(QFile(langqmfilepath).exists())
        {
            m_translator->load(langqmfilepath);
            qApp->installTranslator(m_translator);
            //重新刷新界面
            ui->retranslateUi(this);
        }
        //切换英文

    }else  if(lange=="cn"){


        m_translator=new QTranslator();
        QString  langqmfilepath = appcurrentpath + QString("/language/KSTMAIN_zh_CN.qm");
        qDebug()<<langqmfilepath;
        if(QFile(langqmfilepath).exists())
        {
            m_translator->load(langqmfilepath);
            qApp->installTranslator(m_translator);
            //重新刷新界面
            ui->retranslateUi(this);
        }

    }
    emit sendlange(lange, device);
}

void SystemSetting::ShowTreaim()
{
    system("weston-terminal &");
}

void SystemSetting::testInternet()
{
    QString ip= ui->lineEdit->text();
    int port=ui->lineEdit_2->text().toInt();
    int flag=chackInternet(ip, port, 1000);
    if(flag==1){
        ui->label_11->setText(ip+":"+QString::number(port)+tr("---------连接成功"));
        ui->label_11->setStyleSheet("QLabel{color:rgb(8,137,31);font-size:20px;margin-top:20px;}");
    }else if(flag==2){
        ui->label_11->setText(outurl+":"+QString::number(port)+tr("---------连接成功"));
        ui->label_11->setStyleSheet("QLabel{color:rgb(8,137,31);font-size:20px;margin-top:20px;}");
    }else{
        ui->label_11->setText("连接不成功");
        ui->label_11->setStyleSheet("QLabel{color:rgb(163,54,43);font-size:20px;margin-top:20px;}");
    }

}


//检查ssn
void SystemSetting::checkauthorization()
{

    QDir dir(appcurrentpath+"/lib/SSN/");
    QFile file(appcurrentpath+"/lib/SSN/libyy.so");
    //1.判断存储ssn文件夹是否存在
    qDebug()<<appcurrentpath+"/lib/SSN/:"+(dir.exists()?"true":"false");
    qDebug()<<appcurrentpath+"/lib/SSN/libyy.so:"+(file.exists()?"true":"false");
    if(dir.exists()&&file.exists()){
        ui->pushButton_5->setText(tr("已授权"));
        ui->pushButton_5->setDisabled(true);
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
                                                        //showmessage(tr("授权")+appversions.at(i)->getAppname()+tr("成功"),nullptr);
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
                        ui->pushButton_5->setText("未授权");

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
                                    ui->pushButton_5->setText("受理中...");
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

QList<EmpowerPoJo *> SystemSetting::getempower(QString filename)
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
void SystemSetting::readconfigforIPService()
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
QJsonObject SystemSetting::getfun(QString fullRequest)
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
    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug()<<"Request return exception:"+reply ->error();
        qInfo()<<"请求的返回异常:"+reply->error();
    }

    //解析返回的Json结果
    QByteArray replyData = reply->readAll();
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(replyData, &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        qInfo()<<"数据获取失败,数据格式（json）有误";
        qDebug()<<"Data acquisition failed, data format (JSON) incorrect";

    }

    QJsonObject rootObj = jsonDoc.object();

    return rootObj;

}

QString SystemSetting::readJsonfloadname(QString key,QString jsonpath)
{
    //    qDebug()<<jsonpath;

    QFile loadFile(jsonpath);
    if(!loadFile.open(QIODevice::ReadOnly| QIODevice::Text))
    {

        return 0;
    }
    QByteArray array = loadFile.readAll(); //将数据读到array中
    loadFile.close();
    //json错误抛出
    QJsonParseError json_error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(array,&json_error);
    if(json_error.error != QJsonParseError::NoError)//判断是否可以解析
    {
        qDebug()<<"json error!";
        return 0;
    }
    QJsonObject rootObj = jsonDoc.object();
    QJsonValue foldepath = rootObj.value(key);
    QString folderstring=foldepath.toString();
    return folderstring;
}

int SystemSetting::getJsonObjInt(QString key, QJsonObject qjo)
{
    return qjo.value(key).toInt();
}

QString SystemSetting::getJsonobjQString(QString key, QJsonObject qjo)
{
    return qjo.value(key).toString();
}


QJsonArray SystemSetting::getJsonobjArray(QString key, QJsonObject qjo)
{
    return qjo.value(key).toArray();
}
void SystemSetting::postFileTask(QString filepath,QString filename)//需要的数据
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
    //    qDebug()<<filepath;
    QNetworkRequest netReq;
    netReq.setUrl(url);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(send_xlsdata(QNetworkReply*)));
    manager->post(netReq,multiPart);

}
int SystemSetting:: System_Check(int result)
{
    if((-1 != result) && (WIFEXITED(result)) && (!(WEXITSTATUS(result))))

        return 0;
    else

        return -1;
}
void SystemSetting::showmessage(QString txt,QString shellname){

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
        ui->pushButton_5->setText("未授权");
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
bool SystemSetting::getdownfun(QString fullrequest, QString fileName)
{
    QNetworkRequest request;

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

QList<AppsVersion *>  SystemSetting::selectAppsversion(QString sql)
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

bool SystemSetting::unzip(QString fileName){
    QString folderpath=appcurrentpath+"/lib/SSN/unzip";
    QDir qdir(folderpath);
    if(!qdir.exists()){
        qdir.mkdir(folderpath);
    }
    QFile qfile(fileName);
    qDebug()<<fileName;
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
int SystemSetting::copyfile(QString orginfile,QString pathfile){
    QString copycommond="cp  -rf "+orginfile+" "+pathfile;
    const char* commond=copycommond.toUtf8().data();
    int result=system(commond);

    return result;


}
void SystemSetting::downloadYYSN(){



    QList<Driverpojo *> driverlist=getdriver("YYSN_");
    QString fullrequest= ipservicepojo->getUrltype()+"://"+ ipservicepojo->getIp()+":"+QString::number(ipservicepojo->getPort())
            +"/driver/download?path=Driver/YYSN_rtm_arm_13.1.0.1001_20240327.zip";
    QString drivername=driverlist.at(driverlist.size()-1)->getDrivername();
    bool successdownload=getdownfun(fullrequest,drivername);
    bool succesunzip=false;
    if(successdownload){
        ui->pushButton_5->setText("解压中...");
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
            showmessage("出错了，请重试",nullptr);
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
                    //                    qDebug()<<qfileinfo.fileName();
                    if(qfileinfo.fileName().startsWith("YYSN_")&&qfileinfo.fileName().endsWith(".txt")){
                        postFileTask(qfileinfo.absolutePath()+"/"+qfileinfo.fileName(),qfileinfo.fileName());
                        ui->pushButton_5->setText("受理中...");
                        ui->pushButton_5->setFlat(true);
                        ui->pushButton_5->setStyleSheet("QPushBUtton{color:red;}");
                    }
                }
            }
        }
    }
}
QList<Driverpojo *>SystemSetting::getdriver(QString filename){

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
