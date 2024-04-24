#include "shop.h"
#include "shop.h"
#include "shop.h"
#include "shop.h"
#include "shop.h"
#include "ui_shop.h"
#include "QScroller"

#include <qjsondocument.h>


Shop::Shop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Shop)
{
    ui->setupUi(this);
    //     connect(this, SIGNAL(sendlange(QString,int)), this, SLOT(receivelange(QString,int )));

}

Shop::~Shop()
{
    delete ui;
}
void Shop::initshop()
{
    QScroller::grabGesture(ui->scrollArea->viewport(),QScroller::LeftMouseButtonGesture);
    appcurrentpath=QCoreApplication::applicationDirPath();
    readconfigforIPService();
    readconfigforSystem();
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

void Shop::getshopitem()
{
    if(list.size()==0){
        ui->label->setVisible(true);
        QPixmap pixmap = QPixmap(appcurrentpath+"/image/empty.png").scaled(600,600,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
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

        qbl->setContentsMargins(0,0,0,20);

        for(int i=0;i<list.size();i++){
            QString urlpath=ipservicepojo->getUrltype()+"://"
                    +ipservicepojo->getIp()+":"+QString::number(ipservicepojo->getPort())
                    +dowonloadpath+"?path="+list.at(i)->getPath()+"/"+list.at(i)->getZipname();
            ShopItem *shopitem=new ShopItem();

            shopitem->setMinimumWidth(this->width()-50);
            shopitem->setMaximumHeight(200);
            //            shopitem->setMinimumWidth(this->screenhight);
            //              qbl->setAlignment(shopitem,Qt::AlignTop);
            connect(shopitem, SIGNAL(insertsuccesstoshop()), this, SLOT(reciveinsertsuccessforshopitem()));

            shopitem->iniShopItempath(urlpath,list.at(i));
            qbl->setAlignment(Qt::AlignTop);

            QString logpath=appcurrentpath+"/image/logo/"+list.at(i)->getZipname().split("_")[0]+".png";
            if(lange=="en"){
                logpath=appcurrentpath+"/image/logo/"+list.at(i)->getZipname().split("_")[0]+"en.png";
            }
            QFile qfile(logpath);
            if(!qfile.exists()){
                logpath=appcurrentpath+"/image/logo/Default.png";
            }
            shopitem->changelogo(logpath);
            shopitem->changeappname(list.at(i)->getZipname().split("_")[0]);
            shopitem->changeappversion(""+list.at(i)->getVersionchar()+"");
            shopitem->changeappupdatedtime(""+QString::number(list.at(i)->getUpdateloadtime()));
            //            qDebug()<<list.at(i)->getVersion();
            shopitem->changeappsize(list.at(i)->getZipsize());
            //            shopitem->setMinimumWidth(this->screenwidth);
            //            shopitem->setMaximumHeight(250);
            AppsVersion *appsversion =selectAppsversion(list.at(i)->getPath());
            if(appsversion!=nullptr&&appsversion->getApppath()==list.at(i)->getPath()){
                shopitem->changepushbuttonstate(false,true,tr("已安装"));
            }
            qlist.append(shopitem);
            qbl->addWidget(shopitem);
            //            shopitem->

        }
        ui->scrollAreaWidgetContents->setLayout(qbl);
        //          ui->scrollAreaWidgetContents->setMinimumWidth(this->screenwidth-200);
        ui->scrollAreaWidgetContents->setMinimumHeight((list.size()+1)*260);
    }


}

bool Shop::IPLive(QString ip, int port, int timeout) {
    QTcpSocket tcpClient;
    tcpClient.abort();
    tcpClient.connectToHost(ip, port);
    //100毫秒没有连接上则判断不在线
    return tcpClient.waitForConnected(timeout);
}

void Shop::getbestnewapprtm()
{
    QString  request=ipservicepojo->getUrltype()+"://"+ipservicepojo->getIp()+
            ":"+QString::number(ipservicepojo->getPort())+getbeastnewrtmapp
            +"?type="+QString::number(ipservicepojo->getDevicetype())+"&"+"os="+ipservicepojo->getSystemtype()
            +"&versiontype="+"rtm";
    ;
    if(IPLive(ipservicepojo->getIp(), ipservicepojo->getPort(),1000)){
        QJsonObject resultobj=getfun(request);
        int code=getJsonObjInt("code",resultobj);
        qDebug()<<"14.请求结果:"+QString::number(code);
        //请求通过
        if(code==1){
            QJsonArray qja=getJsonobjArray("data",resultobj);
            qDebug()<<qja.size();
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
        getshopitem();
    }else{
        timecount=0;
        getshopitem();
    }



}

void Shop::readconfigforIPService()
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
        getbeastnewrtmapptype=ipservicesetting->value("/router/ getbeastnewrtmapptype").toString();
        getbestnewupdatedapp=ipservicesetting->value("/router/ getbestnewupdatedapp").toString();
        getbestnewupdatedapptype=ipservicesetting->value("/router/getbestnewupdatedapptype").toString();
        updatederrorpath=ipservicesetting->value("/router/updatederrorpath").toString();
        updatederrorpathtype=ipservicesetting->value("/router/updatederrorpathtype").toString();
        gettododatapath=ipservicesetting->value("/router/gettododatapath").toString();
        gettododatapathtype=ipservicesetting->value("/router/gettododatapathtype").toString();
        dowonloadpath=ipservicesetting->value("/router/dowonloadpath").toString();

        delete(ipservicesetting);
    }
}

void Shop::readconfigforSystem()
{
    systemsettings=new QSettings (appcurrentpath+"/config/systemsetting.ini", QSettings::IniFormat);
    rote=systemsettings->value("systemconfig/rote").toInt();
    device= systemsettings->value("systemconfig/device").toInt();
    lange=systemsettings->value("systemconfig/lange").toString();
}

void Shop::onTimeout()
{
    timecount-=1;
    if(list.size()>0||timecount<=0){
        bool isRunning = timer->isActive();
        getbestnewapprtm();
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

void Shop::reciveinsertsuccessforshopitem()
{
    qDebug()<<"--------------------reciveinsertsuccessforshopitem-----------------------";
    emit successtoapps();
}


QJsonObject Shop::getfun(QString fullRequest)
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

QString Shop::readJsonfloadname(QString key,QString jsonpath)
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



AppsVersion *  Shop::selectAppsversion(QString path)
{
    AppsVersion *appsversion=nullptr;
    sql_query.exec("select * from appsversion where apppath='"+path+"'");
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();

    }
    else
    {
        appsversion=new AppsVersion();
        while(sql_query.next())
        {
            appsversion->setId(sql_query.value(0).toInt());
            appsversion->setUuid(sql_query.value(1).toString());
            appsversion->setAppname(sql_query.value(2).toString());
            appsversion->setApppath(sql_query.value(3).toString());
            appsversion->setApprtmversion(sql_query.value(4).toInt());
            appsversion->setApprtmupdatetime(sql_query.value(5).toInt());
            appsversion->setAppupdatedversion(sql_query.value(6).toInt());
            appsversion->setAppupdatedtime(sql_query.value(7).toInt());
            appsversion->setAppupdateddesc(sql_query.value(8).toString());
            appsversion->setAppsize(sql_query.value(9).toString());
            appsversion->setAppico(sql_query.value(10).toString());
            appsversion->setAppsystem(sql_query.value(11).toString());
            appsversion->setAppdevice(sql_query.value(12).toInt());
        }
    }
    return appsversion;

}




int Shop::getJsonObjInt(QString key, QJsonObject qjo)
{
    return qjo.value(key).toInt();
}

QJsonArray Shop::getJsonobjArray(QString key, QJsonObject qjo)
{
    return qjo.value(key).toArray();
}

QString Shop::getJsonobjQString(QString key, QJsonObject qjo)
{
    return qjo.value(key).toString();
}

void Shop::onResize(QSize size)
{
    this->resize(size);
    ui->verticalWidget->resize(this->size());
    ui->scrollArea->resize(this->size());
    ui->scrollAreaWidgetContents->resize(this->size());
    for(int i=0;i<qlist.size();i++){
        qlist.at(i)->resize(QSize(this->size().width()-20,200));
    }

}


