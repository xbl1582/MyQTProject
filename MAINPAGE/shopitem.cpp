#include "shopitem.h"
#include "ui_shopitem.h"

#include <QHttpMultiPart>

#include <asm/bitsperlong.h>

ShopItem::ShopItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShopItem)
{
    ui->setupUi(this);
    //    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
    //    this->screenwidth=screenRect.width();
    //    this->setMinimumWidth(this->screenwidth-this->screenwidth*0.3);
    //    ui->verticalWidget_2->setMinimumWidth(this->screenwidth-this->screenwidth*0.3);
    appcurrentpath=QCoreApplication::applicationDirPath();
    basepath=appcurrentpath.split("/KSTMAIN")[0];
    savepathsetting = new QSettings (appcurrentpath+"/config/savepath.ini", QSettings::IniFormat);//构建函数
    iniShopItem();
    creatdatabase();
    sql_query=new QSqlQuery(database);
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        //打开应用服务
        installItem(requestur,zipobj);
    });

}


ShopItem::~ShopItem()
{
    delete ui;
}

void ShopItem::iniShopItem()
{
    ui->progressBar->setVisible(false);
}

void ShopItem::iniShopItempath(QString requestur2, Zipobj *zipobj2)
{
    requestur=requestur2;
    zipobj=zipobj2;
}

void ShopItem::installItem(QString requesturl,Zipobj *zipob)
{
    ui->progressBar->setVisible(true);
    runThread(requesturl,zipob);

}

void ShopItem::changelogo(QString path)
{
    ui->pushButton_3->setIcon(QIcon(path));
    ui->pushButton_3->setIconSize(QSize(100,100));
}

void ShopItem::changeappversion(QString name)
{
    ui->label->setText(name);
}

void ShopItem::changeappupdatedtime(QString name)
{
    ui->label_5->setText(changeupdatetime2(name));
}
QString ShopItem::changeupdatetime2(QString name){
    return "("+name.mid(0,4)+"-"+name.mid(4,2)+"-"+name.mid(6,2)+")";
}
void ShopItem::changeappname(QString name)
{
    QString appname=changeappname2(name);
    ui->label_2->setText(appname);
}
QString ShopItem::changeappname2(QString appname)
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

void ShopItem::changeappdesc()
{

}

void ShopItem::changeappsize(QString size)
{
    ui->label_3->setText(size);
}

void ShopItem::changepushbuttonstate(bool visiable,bool buttondisable, QString txt)
{
    ui->progressBar->setVisible(visiable);
    ui->pushButton->setDisabled(buttondisable);
    ui->pushButton->setText(txt);
    ui->pushButton->setFlat(true);
}



void ShopItem::runThread(QString requesturl,Zipobj *zipobj)
{
    ui->pushButton->setDisabled(true);
    thread=new MyThread(requesturl,zipobj);

    connect(thread, &MyThread::reportProgress,ui->progressBar, &QProgressBar::setValue);
    connect(thread, &MyThread::done, this, [=]{
        thread->exit();
        ui->progressBar->setVisible(false);
        ui->pushButton->setText(tr("已安装"));
        ui->pushButton->setFlat(true);
        QFile qf(appcurrentpath+"/zip/"+zipobj->getZipname());
        //      qDebug()<<qf.exists();
        if(qf.exists()){

            bool flag=qf.remove();

            if(flag)
                ui->progressBar->setValue(ui->progressBar->value()+5);
        }
        int priority=0;
        if(zipobj->getPath()=="KSTPXQ"){
            priority=9;
        }else if(zipobj->getPath()=="KSTPXX"){
            priority=9;
        }else if(zipobj->getPath()=="WTKPK"){
            priority=8;
        }
        else if(zipobj->getPath()=="KSTSCAN"){
            priority=7;
        }else if(zipobj->getPath()=="KST"){
            priority=6;
        }else if(zipobj->getPath()=="KSTLX"){
            priority=5;
        }else if(zipobj->getPath()=="WTKP"){
            priority=4;
        }
        else if(zipobj->getPath()=="YYEditor"){
            priority=3;
        }
        else if(zipobj->getPath()=="KSTSP"){
            priority=2;
        }else{
            priority=0;
        }
        QString installsql=QString("INSERT INTO appsversion"
                                   "(uuid, appname, apppath, "
                                   "apprtmversion,apprtmversionchar,"
                                   "apprtmupdatetime,appupdatedversionchar,appupdatedversion,appupdatedtime, "
                                   "appupdateddesc, appsize, appico, "
                                   "appsystem, appdevice,priority)"
                                   "VALUES('"+zipobj->getUuid()+"','"+zipobj->getZipname().split("_")[0]+"','"
                +zipobj->getPath()+"',"+QString::number(zipobj->getVersion())+",'"+zipobj->getVersionchar()+"',"
                +QString::number(zipobj->getUpdateloadtime())+",'"+zipobj->getVersionchar()+"',"+QString::number(zipobj->getVersion())+","+QString::number(zipobj->getUpdateloadtime())+
                +","+"'应用描述','"+zipobj->getZipsize()+"','"+zipobj->getZipname().split("_")[0]+".png','"+zipobj->getOs()+"',"
                +QString::number(zipobj->getType())+","+QString::number(priority)+")");

        todosql(installsql);
        closedatabase();

        QString changechmod777="chmod 777 -R "+appcurrentpath.split("/KSTMAIN")[0];
        const char* code=changechmod777.toLocal8Bit();
        system(code);
        for(int i=1;i<=16;i++){
            savepathsetting->setValue("/savepathconfig/"+zipobj->getPath().toLower()+"savepath"+QString::number(i)," ");
            savepathsetting->sync();
        }
        QMessageBox *msgBox=new QMessageBox(QMessageBox::Information, tr("警告"),tr( "安装成功"), QMessageBox::Yes, this);
        QDesktopWidget *desktop=QApplication::desktop();
        int screenWidth = desktop->width();
        int screenHeight = desktop->height();

        // 计算弹窗的中心位置
        int x = (screenWidth - msgBox->width()) / 2;
        int y = (screenHeight - msgBox->height()) / 2;
        msgBox->move(x,y);
        msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
        QFont qfont;
        qfont.setPixelSize(20);
        msgBox->setFont(qfont);
        msgBox->setMinimumWidth(200);
        msgBox->setMinimumHeight(200);
        msgBox->setBaseSize(200,200);
        msgBox->setFixedSize(200,200);
        msgBox->setButtonText(QMessageBox::Yes,QString(tr("确定")));  //方式1
        QAbstractButton *button=msgBox->button(QMessageBox::Yes);
        button->setMinimumWidth(100);
        button->setFont(qfont);
        button->setMinimumHeight(50);
        int result2 = msgBox->exec();
        if(result2==QMessageBox::Yes){

            emit insertsuccesstoshop();
        }



    });
    thread->start();


}
//数据库操作完成后，最好关闭。
void ShopItem::closedatabase()
{
    database.close();
}



void ShopItem::creatdatabase()
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

void ShopItem::todosql(QString sql)
{

    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        // do something创建数据表
        QString create_sql = sql;
        sql_query->exec("select * from appsversion");
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
