#include "app.h"
#include "ui_app.h"
#include "QScroller"
#include <QUrl>

App::App(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::App)
{
    ui->setupUi(this);

}

App::~App()
{
    delete ui;
}

void App::initapp()
{

    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
    this->screenwidth=screenRect.width();
    this->screenhight=screenRect.height();
    QScroller::grabGesture(ui->scrollArea->viewport(),QScroller::LeftMouseButtonGesture);

    appcurrentpath=QCoreApplication::applicationDirPath();
    readconfigforIPService();
    readconfigforSystem();
    creatdatabase();
    sql_query=new QSqlQuery(database);
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->setInterval(500);
    timer->start();
    movie= new QMovie(appcurrentpath+"/image/loading.gif");
    ui->label->setMovie(movie);
    ui->label->setWindowFlags(Qt::FramelessWindowHint);//无边框
    ui->label->setAttribute(Qt::WA_TranslucentBackground);//背景透明
    movie->start();
}
void App::onTimeout()
{

    timecount-=1;
    if(timecount<=0){
        bool isRunning = timer->isActive();
        getbestnewappupdated();
        if(isRunning){

            timer->stop();
            movie->stop();
            if(timer!=nullptr)
                delete(timer);
            timer=nullptr;
            if(movie!=nullptr)
                delete movie;
            movie=nullptr;
            ui->label->setVisible(false);

        }

    }
}


QList<AppsVersion *>  App::selectAppsversion()
{
    QList<AppsVersion *> list;

    sql_query->exec("select * from appsversion order by priority desc");
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
            list.append(appsversion);
        }
    }
    return list;

}

//数据库操作完成后，最好关闭。
void App::closedatabase()
{
    database.close();
}



void App::creatdatabase()
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

void App::todosql(QString sql)
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



void App::getappitem()
{
    QList<AppsVersion *> list2=selectAppsversion();
    if(list2.size()==0){
        ui->label->setVisible(true);
        pixmap = QPixmap(appcurrentpath+"/image/empty.png").scaled(600,600,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        //qDebug()<<"设置背景图片";
        QPalette pal=ui->label->palette();
        pal.setBrush(QPalette::Background,QBrush(pixmap));
        this->setAutoFillBackground(true);
        setPalette(pal);
        ui->label->setPixmap(pixmap);
        ui->label->repaint();
        ui->scrollAreaWidgetContents->repaint();
    }else{
        //模拟生成软件项
        QVBoxLayout *qbl=new QVBoxLayout();

        for(int i=0;i<list2.size();i++){
            qbl->setAlignment(Qt::AlignTop);

            appsitem *ai=new appsitem();
            connect(ai, SIGNAL(sendToappAndloadapp()), this, SLOT(recivereAppitemInfo()));
            ai->resizeappitem(QSize(this->size().width()-10,200));
            //            ai->setMinimumWidth(this->screenwidth);
            ai->getappversion(list2.at(i));
            QString logpath=appcurrentpath+"/image/logo/"+list2.at(i)->getAppname()+".png";
            if(lange=="en"){
                logpath=appcurrentpath+"/image/logo/"+list2.at(i)->getAppname()+"en.png";
            }
            QFile qfile(logpath);
            if(!qfile.exists()){
                logpath=appcurrentpath+"/image/logo/Default.png";
            }
            if(list2.at(i)->getAppname()=="KSTMAIN"){
                ai->changeuninstalldisable();
            }
            ai->changelogo(logpath);
            ai->changeappversionAndappupdatedtime(""+list2.at(i)->getAppupdatedversionchar().replace("\n","")+"\t("+changedatelabe(list2.at(i)->getAppupdatedtime())+")");
            if(list.size()==0){
                ai->changeappnewversionAndappnewupdatedtime("已经最新");
                ai->changepushbuttonstate(false,true,"检测更新");
                ai->changeappname(changeappname(list2.at(i)->getAppname()));
                ai->changeappsize(list2.at(i)->getAppsize());
                ai->changeappdesc(list2.at(i)->getAppupdateddesc());
                //                ai->setMinimumWidth(this->screenwidth);
                ai->setMinimumHeight(200);
                ai->setMaximumHeight(200);
                qbl->addWidget(ai);
                appsitemlist.append(ai);
            }else{

                for(int k=0;k<list.size();k++){
                    if(list2.at(i)->getApppath()==list.at(k)->getPath()){


                        if(list2.at(i)->getAppupdatedversion()<list.at(k)->getVersion()||list2.at(i)->getAppupdatedtime()<list.at(k)->getUpdateloadtime()){
                            ai->changeappnewversionAndappnewupdatedtime(tr("最新:")+list.at(k)->getVersionchar()+"\t"
                                                                        +"("+changedatelabe(list.at(k)->getUpdateloadtime())+")");
                            ai->changepushbuttonstate(false,false,tr("更新"));
                            ai->changeappsize(list.at(k)->getZipsize());
                            QString  request=ipservicepojo->getUrltype()+"://"+ipservicepojo->getIp()+
                                    ":"+QString::number(ipservicepojo->getPort())+getbestnewupdatedapp
                                    +"?type="+QString::number(ipservicepojo->getDevicetype())+"&"+"os="+ipservicepojo->getSystemtype()
                                    +"&versiontype="+"updated";

                            ai->iniAPPsItempath(request,list.at(k));
                            QString changechmod777="chmod 777 -R "+appcurrentpath.split("/KSTMAIN")[0];
                            const char* code=changechmod777.toLocal8Bit();
                            system(code);
                            emit sendhavenewtoapps(1);

                        }else{
                            ai->changeappnewversionAndappnewupdatedtime(tr("已是最新版本"));
                            ai->changepushbuttonstate(false,true,tr("检测更新"));
                            ai->changeappsize(list2.at(i)->getAppsize());
                        }
                        break;
                    }else{
                        if(k==list.size()-1){
                            ai->changeappnewversionAndappnewupdatedtime(tr("已是最新版本"));
                            ai->changepushbuttonstate(false,true,tr("检测更新"));
                            ai->changeappsize(list2.at(i)->getAppsize());
                        }
                    }



                }


                ai->changeappname(changeappname(list2.at(i)->getAppname()));

                ai->changeappdesc(list2.at(i)->getAppupdateddesc());
                //                ai->setMinimumWidth(this->screenwidth);

                ai->setMinimumHeight(200);
                ai->setMaximumHeight(200);
                qbl->addWidget(ai);

            }

            ui->scrollAreaWidgetContents->setLayout(qbl);
            //           qDebug()<< ui->scrollAreaWidgetContents->width();
            //           ui->scrollAreaWidgetContents->setGeometry(0,0,this->screenwidth, list2.size()*380);
            ui->scrollAreaWidgetContents->setMinimumHeight((list2.size()+1)*200);
        }
    }
}

bool App::IPLive(QString ip, int port, int timeout) {
    QTcpSocket tcpClient;
    tcpClient.abort();
    tcpClient.connectToHost(ip, port);
    //100毫秒没有连接上则判断不在线
    return tcpClient.waitForConnected(timeout);
}

QString App::changeappname(QString appname)
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

void App::getbestnewappupdated()
{
    QString  request=ipservicepojo->getUrltype()+"://"+ipservicepojo->getIp()+
            ":"+QString::number(ipservicepojo->getPort())+getbestnewupdatedapp
            +"?type="+QString::number(ipservicepojo->getDevicetype())+"&"+"os="+ipservicepojo->getSystemtype()
            +"&versiontype="+"updated";
    if(IPLive(ipservicepojo->getIp(), ipservicepojo->getPort(), 1000)){
        QJsonObject resultobj=getfun(request);
        int code=getJsonObjInt("code",resultobj);
        qDebug()<<"14.请求结果:"+QString::number(code);
        //请求通过
        if(code==1){
            QJsonArray qja=getJsonobjArray("data",resultobj);
            //                qDebug()<<qja.size();
            for(int i=0;i<qja.size();i++){
                qDebug()<<"14."+QString::number(i)+"循环获取数据项"+QString::number(i)+"--"+getJsonobjQString("zipname",qja[i].toObject());
                Zipobj *zipobj=new Zipobj();
                zipobj->setUuid(getJsonobjQString("uuid",qja[i].toObject()));
                zipobj->setZipname(getJsonobjQString("zipname",qja[i].toObject()));
                zipobj->setUpdateloadtime(getJsonObjInt("updateloadtime",qja[i].toObject()));
                zipobj->setZipsize(getJsonobjQString("zipsize",qja[i].toObject()));
                zipobj->setType(getJsonObjInt("type",qja[i].toObject()));
                zipobj->setOs(getJsonobjQString("os",qja[i].toObject()));
                zipobj->setVersiontype(getJsonobjQString("versiontype",qja[i].toObject()));
                zipobj->setVersion(getJsonObjInt("version",qja[i].toObject()));
                zipobj->setVersionchar(getJsonobjQString("versionchar",qja[i].toObject()));
                zipobj->setPath(getJsonobjQString("path",qja[i].toObject()));
                zipobj->setMd5code(getJsonobjQString("md5code",qja[i].toObject()));
                list.append(zipobj);
            }

        }else if(code==50001){
            qDebug()<<"15.数据为空";
            qDebug()<<"数据为空";
        }

        timecount=0;
        getappitem();
    }else{
        timecount=0;

        getappitem();
    }


}

void App::readconfigforIPService()
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

        dowonloadpath=ipservicesetting->value("/router/dowonloadpath").toString();
        dowonloadpathtype=ipservicesetting->value("/router/dowonloadpathtype").toString();
        getbeastnewrtmapp=ipservicesetting->value("/router/getbeastnewrtmapp").toString();
        getbeastnewrtmapptype=ipservicesetting->value("/router/getbeastnewrtmapptype").toString();
        getbestnewupdatedapp=ipservicesetting->value("/router/getbestnewupdatedapp").toString();
        getbestnewupdatedapptype=ipservicesetting->value("/router/getbestnewupdatedapptype").toString();
        updatederrorpath=ipservicesetting->value("/router/updatederrorpath").toString();
        updatederrorpathtype=ipservicesetting->value("/router/updatederrorpathtype").toString();
        gettododatapath=ipservicesetting->value("/router/gettododatapath").toString();
        gettododatapathtype=ipservicesetting->value("/router/gettododatapathtype").toString();
        dowonloadpath=ipservicesetting->value("/router/dowonloadpath").toString();

        delete(ipservicesetting);
    }
}

void App::readconfigforSystem()
{
    systemsettings=new QSettings (appcurrentpath+"/config/systemsetting.ini", QSettings::IniFormat);
    rote=systemsettings->value("systemconfig/rote").toInt();
    device= systemsettings->value("systemconfig/device").toInt();
    lange=systemsettings->value("systemconfig/lange").toString();
}



QJsonObject App::getfun(QString fullRequest)
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

QString App::readJsonfloadname(QString key,QString jsonpath)
{
    qDebug()<<jsonpath;

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






int App::getJsonObjInt(QString key, QJsonObject qjo)
{
    return qjo.value(key).toInt();
}

QJsonArray App::getJsonobjArray(QString key, QJsonObject qjo)
{
    return qjo.value(key).toArray();
}

QString App::getJsonobjQString(QString key, QJsonObject qjo)
{
    return qjo.value(key).toString();
}


void App::recivereAppitemInfo()
{
    qDebug()<<"---------------------recivereAppitemInfo-----------";
    emit sendToappsAndreload();
    qDebug()<<"---------------------recivereAppitemInfo2-----------";
}
QString  App:: changedatelabe(int date){
    QString strdate=QString::number(date).mid(0,4)+"-"+QString::number(date).mid(4,2)+"-"+QString::number(date).mid(6,2);

    return strdate;

}

void App::onResize(QSize size)
{
    this->resize(size);
    ui->verticalWidget->resize(QSize(this->size().width(),this->size().height()-10));
    ui->scrollArea->resize(QSize(this->size().width(),this->size().height()-10));
    ui->scrollAreaWidgetContents->resize(QSize(this->size().width(),this->size().height()-10));

}
