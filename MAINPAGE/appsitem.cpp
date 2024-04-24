#include "appsitem.h"
#include "ui_appsitem.h"

appsitem::appsitem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::appsitem)
{
    ui->setupUi(this);

    this->iniAppsItem();


    //    ui->pushButton_4->setIcon(QIcon(appcurrentpath+"/image/log.png"));
    //    ui->pushButton_4->setIconSize(QSize(50,50));

}

appsitem::~appsitem()
{
    delete ui;
}

void appsitem::iniAppsItem()
{
    appcurrentpath=QCoreApplication::applicationDirPath();
     basepath=appcurrentpath.mid(0,appcurrentpath.lastIndexOf("KSTMAIN"));

    rtmlist=new QList<Zipobj *>();
    readconfigforIPService();
    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
    this->screenwidth=screenRect.width();
    this->screenhight=screenRect.height();
    //0.3
    //    this->setMinimumWidth(this->screenwidth-this->screenwidth*0.3);
    //    ui->gridWidget->setMinimumWidth(this->screenwidth-this->screenwidth*0.3);

    ui->progressBar->setVisible(false);
    creatdatabase();
    sql_query=new QSqlQuery(database);
    ipservicepojo=new IPServicePoJo();
    connect(ui->pushButton_3,&QPushButton::clicked,this,[=](){
        //打开应用服务
        if(!checkupdated){
            installItem(requestur,zipobj);
        }else{
            checkupdatedfun(appsversion);
        }

    });

    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        //卸载服务
        msgBox=new QMessageBox(QMessageBox::Warning, tr("警告"),tr( "确定卸载?"), QMessageBox::Yes|QMessageBox::No, this);
        msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
        QFont qf;
        qf.setPixelSize(20);
        msgBox->setFont(qf);
        msgBox->setMinimumWidth(200);
        msgBox->setMinimumHeight(200);
        msgBox->setBaseSize(200,200);

        msgBox->setFixedSize(200,200);
        msgBox->setButtonText(QMessageBox::Yes,QString(tr("确定")));  //方式1
        msgBox->setButtonText(QMessageBox::No,QString(tr("取消")));  //方式1
        QAbstractButton *button=msgBox->button(QMessageBox::Yes);
        button->setMinimumWidth(100);
        button->setFont(qf);
        button->setMinimumHeight(50);
        QAbstractButton *button2=msgBox->button(QMessageBox::No);
        button2->setMinimumWidth(100);
        button2->setFont(qf);
        button2->setMinimumHeight(50);

        int result2 = msgBox->exec();
        bool falg=false;
        if(result2==QMessageBox::Yes){
            falg=uninstall(appsversion);
        }


        if(falg){
            //卸载完成发送消息,给app.cpp
            msgBox=new QMessageBox(QMessageBox::Information, tr("警告"),tr( "卸载成功"), QMessageBox::Yes, this);
            QDesktopWidget *desktop=QApplication::desktop();
            int screenWidth = desktop->width();
            int screenHeight = desktop->height();

            // 计算弹窗的中心位置
            int x = (screenWidth - msgBox->width()) / 2;
            int y = (screenHeight - msgBox->height()) / 2;
            msgBox->move(x,y);
            msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
            QFont qf;
            qf.setPixelSize(20);
            msgBox->setFont(qf);
            msgBox->setMinimumWidth(200);
            msgBox->setMinimumHeight(200);
            msgBox->setBaseSize(200,200);
            msgBox->setFixedSize(200,200);
            msgBox->setButtonText(QMessageBox::Yes,QString(tr("确定")));  //方式1
            msgBox->setButtonText(QMessageBox::No,QString(tr("取消")));  //方式1
            QAbstractButton *button=msgBox->button(QMessageBox::Yes);
            button->setMinimumWidth(100);
            button->setFont(qf);
            button->setMinimumHeight(50);
            //   msgBox->setStyleSheet("QLabel{"

            //                         "}");
            int result2 = msgBox->exec();
            if(result2==QMessageBox::Yes){
                emit sendToappAndloadapp();
                ui->pushButton_3->setDisabled(true);
                ui->pushButton_3->setFlat(true);
                ui->pushButton_3->setText("已卸载");
                ui->pushButton_3->setStyleSheet("border:none;color:black;");
                ui->pushButton->setDisabled(true);
            }
        }
    });
    connect(ui->pushButton_2,&QPushButton::clicked,this,[=]{

        showappsiteminfo(appsversion);

    });

    connect(ui->pushButton_4,&QPushButton::clicked,this,[=]{



    });

}

void appsitem::iniAPPsItempath(QString requestur2, Zipobj *zipobj2)
{
    requestur=requestur2;
    zipobj=zipobj2;

}
void appsitem::getappversion(AppsVersion *myappsversion){
    appsversion=myappsversion;
}

void appsitem::showappsiteminfo(AppsVersion *appsversion)
{
    savepathsetting = new QSettings (appcurrentpath+"/config/savepath.ini", QSettings::IniFormat);//构建函数
    if(appsiteminfo!=nullptr){
        delete(appsiteminfo);
        appsiteminfo=nullptr;
    }
    QStringList fpath={};
    QStringList fflie={};
    for(int i=1;i<=16;i++){
        if(i<=8){
            QString fpath2=savepathsetting->value(QString("/savepathconfig/"+appsversion->getAppname().toLower()+"savepath"+QString::number(i))).toString();
            if(fpath2!=nullptr &&fpath2!=" "){
                fpath.append(fpath2);
            }

        }else{

            QString fflie2=savepathsetting->value(QString("/savepathconfig/"+appsversion->getAppname().toLower()+"savepath"+QString::number(i))).toString();
            if(fflie2!=nullptr &&fflie2!=" "){
                fflie.append(fflie2);
            }

        }
    }

    appsiteminfo=new AppsItemInfo(nullptr);
    appsiteminfo->initAppsItemInfo(appsversion,fpath,fflie);
    appsiteminfo->show();

}

void appsitem::installItem(QString requesturl, Zipobj *zipobj)
{
    qDebug()<<"todo";
    ui->progressBar->setVisible(true);
    runThread(requesturl,zipobj);
}

void appsitem::changelogo(QString path)
{
    ui->pushButton_2->setIcon(QIcon(path));
    ui->pushButton_2->setIconSize(QSize(120,120));
}

void appsitem::changeappname(QString name)
{

    ui->pushButton_4->setText(name);
}

void appsitem::changeappversionAndappupdatedtime(QString name)
{
    ui->label->setText(name);
}

void appsitem::changertmversionAndaddrtmtime(QString name){
    // ui->label_7->setText(name);
}

void appsitem::changeappnewversionAndappnewupdatedtime(QString name)
{
    ui->label_5->setText(name);
    if(name=="已经最新"){
        ui->label_5->setStyleSheet("QLabel{color:rgb(100,100,100)}");
    }
}



void appsitem::changeappdesc(QString desc)
{
    //    ui->label_4->setText(desc);
}

void appsitem::changeappsize(QString size)
{
    ui->label_3->setText(size);
}

void appsitem::changepushbuttonstate(bool visiable, bool buttondisable, QString txt)
{
    ui->pushButton_3->setText(txt);
    //    ui->pushButton_3->setDisabled(buttondisable);
    checkupdated=buttondisable;
    ui->progressBar->setVisible(visiable);
    //    ui->pushButton_3->setFlat(buttondisable);
}

void appsitem::runThread(QString requestur, Zipobj *zipobj)
{
    if(zipobj!=nullptr){
        AppsVersion *appversion=selectAppsversion(zipobj->getPath());
        qDebug()<<appversion->getAppname();
        if(appversion->getAppname()!=""&&zipobj->getPath()!=""){
            getbestnewapprtm(zipobj->getPath());
            if(!rtmlist->isEmpty()){
                for(int i=0;i<rtmlist->size();i++){
                    if(rtmlist->at(i)->getPath()==appversion->getApppath()){
                        if(appversion->getApprtmversion()<rtmlist->at(i)->getVersion()
                                &&appversion->getApprtmupdatetime()<rtmlist->at(i)->getUpdateloadtime()){
                            //完整包存在最新版本先下载完整包
                            qDebug()<<rtmlist->at(i)->getZipname()+"-----------------";
                            thread2=new MyThread(" ",rtmlist->at(i));
                            connect(thread2, &MyThread::reportProgress,ui->progressBar, &QProgressBar::setValue);
                            connect(thread2, &MyThread::done, this, [=]{
                                thread2->exit();
                                QFile qf(appcurrentpath+"/zip/"+zipobj->getZipname());
                                if(qf.exists()){
                                    bool flag=qf.remove();
                                    if(flag)
                                        ui->progressBar->setValue(ui->progressBar->value()+5);
                                }
                                QString installsql=QString("UPDATE appsversion SET apprtmversion ="+QString::number( zipobj->getVersion())+
                                                           ",apprtmversionchar='"+zipobj->getVersionchar()+"', apprtmupdatetime="+QString::number(zipobj->getUpdateloadtime())+" where apppath='"+zipobj->getPath()+"'");
                                qDebug()<<installsql;
                                if(database.isOpen()){
                                    todosql(installsql);
                                    ui->progressBar->setVisible(false);
                                }else{
                                    database.open();
                                    todosql(installsql);
                                    ui->progressBar->setVisible(false);
                                }



                                //更新包
                                thread=new MyThread(requestur,zipobj);
                                connect(thread, &MyThread::reportProgress,ui->progressBar, &QProgressBar::setValue);
                                connect(thread, &MyThread::done, this, [=]{
                                    thread->exit();
                                    ui->progressBar->setVisible(false);
                                    checkupdated=true;
                                    if(checkupdated){
                                        ui->pushButton_3->setText("检测更新");
                                        ui->pushButton_3->setStyleSheet("QPushButton{ background-color: #CCE3F4; "
                                                                        "border-style: outset;"
                                                                        "border-width: 2px;"
                                                                        "border-radius:5px; "
                                                                        "border-color: #CCE3F4;"
                                                                        "font: bold 20px;"
                                                                        "min-width:4em;"
                                                                        "min-height:2em;"
                                                                        "color:white; "
                                                                        "font-family:宋体;"
                                                                        "padding: 5px;}"
                                                                        "QPushButton:pressed{"
                                                                        "background-color: #3652AD;"
                                                                        "border-style: inset;"
                                                                        "}"
                                                                        "QPushButton:!enabled{"
                                                                        "background-color: rgba(100, 100, 100,0);"
                                                                        "border-style: inset;"
                                                                        "}");
                                    }
                                    QFile qf(appcurrentpath+"/zip/"+zipobj->getZipname());
                                    if(qf.exists()){
                                        bool flag=qf.remove();
                                        if(flag)
                                            ui->progressBar->setValue(ui->progressBar->value()+5);

                                    }
                                    QString installsql=QString("UPDATE appsversion SET appupdatedversion ="+QString::number( zipobj->getVersion())+
                                                               ",appupdatedversionchar='"+zipobj->getVersionchar()+"', appupdatedtime="+QString::number(zipobj->getUpdateloadtime())+" where apppath='"+zipobj->getPath()+"'");
                                    if(database.isOpen()){
                                        todosql(installsql);
                                        ui->progressBar->setVisible(false);
                                    }else{
                                        database.open();
                                        todosql(installsql);
                                        ui->progressBar->setVisible(false);
                                    }
                                    ui->label->setText(zipobj->getVersionchar()+"  "+ changedatelabe(zipobj->getUpdateloadtime()));

                                });
                                thread->start();

                            });
                            thread2->start();
                        }else{
                            if(zipobj!=nullptr){
                                //更新包
                                thread=new MyThread(requestur,zipobj);
                                connect(thread, &MyThread::reportProgress,ui->progressBar, &QProgressBar::setValue);
                                connect(thread, &MyThread::done, this, [=]{
                                    thread->exit();
                                    ui->progressBar->setVisible(false);
                                    checkupdated=true;
                                    if(checkupdated){
                                        ui->pushButton_3->setText("检测更新");
                                    }
                                    QFile qf(appcurrentpath+"/zip/"+zipobj->getZipname());
                                    if(qf.exists()){
                                        bool flag=qf.remove();
                                        if(flag)
                                            ui->progressBar->setValue(ui->progressBar->value()+5);

                                    }
                                    QString installsql=QString("UPDATE appsversion SET appupdatedversion ="+QString::number( zipobj->getVersion())+
                                                               ",appupdatedversionchar='"+zipobj->getVersionchar()+"', appupdatedtime="+QString::number(zipobj->getUpdateloadtime())+" where apppath='"+zipobj->getPath()+"'");
                                    if(database.isOpen()){
                                        todosql(installsql);
                                        ui->progressBar->setVisible(false);
                                    }else{
                                        database.open();
                                        todosql(installsql);
                                        ui->progressBar->setVisible(false);
                                    }
                                    ui->label->setText(zipobj->getVersionchar()+"\t"+ changedatelabe(zipobj->getUpdateloadtime()));

                                });
                                thread->start();
                            }else{
                                ui->pushButton_3->setText("检测更新");
                                ui->label_5->setText("已是最新.");
                                ui->label_5->setStyleSheet("QLabel{color:rgb(100,100,100)}");
                            }

                            //                    ui->progressBar->setVisible(false);
                            //                     ui->pushButton_3->setText("检测更新");
                        }
                    }
                    else{
                        continue;
                    }
                }

            }
            else{
                //更新包
                thread=new MyThread(requestur,zipobj);
                connect(thread, &MyThread::reportProgress,ui->progressBar, &QProgressBar::setValue);
                connect(thread, &MyThread::done, this, [=]{
                    thread->exit();
                    ui->progressBar->setVisible(false);
                    checkupdated=true;
                    if(checkupdated){
                        ui->pushButton_3->setText("检测更新");
                        //           ui->pushButton_3->setFlat(true);
                        //           ui->pushButton_3->setDisabled(true);
                    }
                    QFile qf(appcurrentpath+"/zip/"+zipobj->getZipname());
                    if(qf.exists()){
                        bool flag=qf.remove();
                        if(flag)
                            ui->progressBar->setValue(ui->progressBar->value()+5);
                    }
                    QString installsql=QString("UPDATE appsversion SET appupdatedversion ="+QString::number( zipobj->getVersion())+
                                               ",appupdatedversionchar='"+zipobj->getVersionchar()+"', appupdatedtime="+QString::number(zipobj->getUpdateloadtime())+" where apppath='"+zipobj->getPath()+"'");
                    if(database.isOpen()){
                        todosql(installsql);
                        ui->progressBar->setVisible(false);
                    }else{
                        database.open();
                        todosql(installsql);
                        ui->progressBar->setVisible(false);
                    }
                    ui->label_5->setText("已是最新.");
                    ui->label_5->setStyleSheet("QLabel{color:rgb(100,100,100)}");


                });
                thread->start();
            }
            if(appversion->getAppname()=="KSTMAIN"){
                //卸载完成发送消息,给app.cpp
                msgBox=new QMessageBox(QMessageBox::Information, tr("警告"),tr( "更新成功,需要重启设备"), QMessageBox::Yes, this);
                QDesktopWidget *desktop=QApplication::desktop();
                int screenWidth = desktop->width();
                int screenHeight = desktop->height();

                // 计算弹窗的中心位置
                int x = (screenWidth - msgBox->width()) / 2;
                int y = (screenHeight - msgBox->height()) / 2;
                msgBox->move(x,y);
                msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
                QFont qf;
                qf.setPixelSize(20);
                msgBox->setFont(qf);
                msgBox->setMinimumWidth(200);
                msgBox->setMinimumHeight(200);
                msgBox->setBaseSize(200,200);
                msgBox->setFixedSize(200,200);
                msgBox->setButtonText(QMessageBox::Yes,QString(tr("确定")));  //方式1
                QAbstractButton *button=msgBox->button(QMessageBox::Yes);
                button->setMinimumWidth(100);
                button->setFont(qf);
                button->setMinimumHeight(50);
                //   msgBox->setStyleSheet("QLabel{"

                //                         "}");
                int result2 = msgBox->exec();
                if(result2==QMessageBox::Yes){
                    system("reboot");
                }
            }
            QString chmodcommond="chmod 777 -R "+  basepath;
            const char* chmod=chmodcommond.toUtf8().data();
            system(chmod);


            QFile qf(basepath+zipobj->getPath()+"/version.txt");
            if(qf.open(QFile::WriteOnly | QFile::Truncate)){        //文件流写文件
                QTextStream out(&qf);      //通过文本流控制文件写操作
                QString changVersioncontent=zipobj->getVersionchar();
                out<<changVersioncontent<<endl;
            }
            qf.close();


            closedatabase();
        }else{
            ui->progressBar->setVisible(false);
            ui->pushButton_3->setText("检测更新");
            ui->label_5->setText("已是最新.");
            ui->label_5->setStyleSheet("QLabel{color:rgb(100,100,100)}");
        }
    }else{
        ui->progressBar->setVisible(false);
        ui->pushButton_3->setText("检测更新");
        ui->label_5->setText("已是最新.");
        ui->label_5->setStyleSheet("QLabel{color:rgb(100,100,100)}");
    }


}


//数据库操作完成后，最好关闭。
void appsitem::closedatabase()
{
    database.close();
}

bool appsitem::uninstall(AppsVersion *myappsversion)
{



    //删除数据库中的项
    QString sql="delete from appsversion where uuid='"+myappsversion->getUuid()+"'";
    qDebug()<<sql;
    bool flag=deletesql(sql);
    ui->pushButton->setDisabled(true);
    //删除文件夹
    if(flag){
        //        QString killall="killall "+myappsversion->getAppname();
        //        const char * commond=killall.toLocal8Bit();
        //        system(commond);
        QStringList qlist=appcurrentpath.split("/");
        QString apppath;
        for (int i=0;i<qlist.count()-1;i++) {
            apppath+=qlist.at(i)+"/";
        }
        //       qDebug()<<apppath;
        QDir qdir(apppath+myappsversion->getApppath());
        QList<QFileInfo> filelist =qdir.entryInfoList();

        if(qdir.exists()){
            qdir.removeRecursively();
            return true;
        }
    }else{
        return false;
    }
    return true;
}



bool appsitem::deletesql(QString sql)
{
    if(database.isOpen()){
        sql_query->exec(sql);
        if(!sql_query->exec())
        {
            qDebug()<<sql_query->lastError();
            return false;

        }
        else
        {
            qDebug()<<"deleted!";
            closedatabase();
            return true;

        }
    }else{
        database.open();
        sql_query->exec(sql);
        if(!sql_query->exec())
        {
            qDebug()<<sql_query->lastError();
            return false;

        }
        else
        {
            qDebug()<<"deleted!";
            closedatabase();
            return true;

        }
    }

}

QString appsitem::getappsize()
{

}

quint64 appsitem::getflodersize(const QString filePath)
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

QString appsitem::size_human(const QString filePath)
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

void appsitem::checkupdatedfun(AppsVersion *myappversion)
{
    readconfigforIPService();
    QString  request=ipservicepojo->getUrltype()+"://"+ipservicepojo->getIp()+
            ":"+QString::number(ipservicepojo->getPort())+getbestnewupdatedapp
            +"?type="+QString::number(ipservicepojo->getDevicetype())+"&"+"os="+ipservicepojo->getSystemtype()
            +"&versiontype="+"updated"+"&path="+myappversion->getApppath();
    if(IPLive(ipservicepojo->getIp(), ipservicepojo->getPort(), 1000)){
        QJsonObject resultobj=getfun(request);
        int code=getJsonObjInt("code",resultobj);
        qDebug()<<"14.请求结果:"+QString::number(code);
        //请求通过
        if(code==1){
            QJsonArray qja=getJsonobjArray("data",resultobj);
            //            qDebug()<<qja.size();
            zipobj=nullptr;
            if(qja.size()!=0){
                zipobj =new Zipobj();
            }
            for(int i=0;i<qja.size();i++){

                qDebug()<<"14."+QString::number(i)+"循环获取数据项"+QString::number(i);
                qDebug()<<"14.1"+getJsonobjQString("zipname",qja[i].toObject());

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

            }
            if(zipobj!=nullptr){
                //                qDebug()<<myappversion->getAppupdatedtime();
                qDebug()<<zipobj->getUpdateloadtime();
                //                qDebug()<<myappversion->getAppupdatedversion();
                //                qDebug()<<zipobj->getVersion();
                if(myappversion->getAppupdatedtime()<zipobj->getUpdateloadtime()&&myappversion->getAppupdatedversion()<zipobj->getVersion()){
                    changeappnewversionAndappnewupdatedtime(tr("最新:")+zipobj->getVersionchar()+"\t "+""+QString::number(zipobj->getUpdateloadtime())+"");
                    checkupdated=false;
                    ui->pushButton_3->setText(tr("更新"));
                }else{
                    checkupdated=true;
                    ui->pushButton_3->setText(tr("检测更新"));
                    ui->label_5->setText(tr("已是最新."));
                    ui->label_5->setStyleSheet("QLabel{color:rgb(100,100,100)}");
                }
            }
            else{

                checkupdated=true;
                ui->pushButton_3->setText(tr("检测更新"));
                ui->label_5->setText(tr("已是最新."));
                ui->label_5->setStyleSheet("QLabel{color:rgb(100,100,100)}");

            }
        }else if(code==50001){
            checkupdated=true;
            ui->pushButton_3->setText(tr("检测更新"));

            qDebug()<<"15.数据为空";
            qDebug()<<"数据为空";
        }

    }





}

void appsitem::getbestnewapprtm(QString path)
{
    readconfigforIPService();
    QString  request=ipservicepojo->getUrltype()+"://"+ipservicepojo->getIp()+
            ":"+QString::number(ipservicepojo->getPort())+getbeastnewrtmapp
            +"?type="+QString::number(ipservicepojo->getDevicetype())+"&"+"os="+ipservicepojo->getSystemtype()
            +"&versiontype="+"rtm&path="+path;
    ;
    qDebug()<<request+"--------------";
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
                rtmlist->append(zipobj);
            }

        }else if(code==50001){
            qDebug()<<"15.数据为空";
            qDebug()<<"数据为空";
        }


    }else{

    }



}

void appsitem::creatdatabase()
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

void appsitem::todosql(QString sql)
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
void appsitem::changeuninstalldisable(){
    ui->pushButton->setDisabled(true);
}


void appsitem::readconfigforIPService()
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
        getbestnewupdatedapp=ipservicesetting->value("/router/getbestnewupdatedapp").toString();
        getbestnewupdatedapptype=ipservicesetting->value("/router/getbestnewupdatedapptype").toString();
        getbeastnewrtmapp=ipservicesetting->value("/router/getbeastnewrtmapp").toString();
        //        qDebug()<<ipservicepojo->getUrltype();


        delete(ipservicesetting);
    }
}


QString appsitem::readJsonfloadname(QString key,QString jsonpath)
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






int appsitem::getJsonObjInt(QString key, QJsonObject qjo)
{
    return qjo.value(key).toInt();
}

QJsonArray appsitem::getJsonobjArray(QString key, QJsonObject qjo)
{
    return qjo.value(key).toArray();
}

QString appsitem::getJsonobjQString(QString key, QJsonObject qjo)
{
    return qjo.value(key).toString();
}


QJsonObject appsitem::getfun(QString fullRequest)
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
bool appsitem::IPLive(QString ip, int port, int timeout) {
    QTcpSocket tcpClient;
    tcpClient.abort();
    tcpClient.connectToHost(ip, port);
    //100毫秒没有连接上则判断不在线
    return tcpClient.waitForConnected(timeout);
}
AppsVersion*  appsitem::selectAppsversion(QString path)
{
    AppsVersion * appsversion=new AppsVersion();;
    if(!database.isOpen()){
        database.open();
    }
    sql_query->exec("select * from appsversion  where apppath='"+path+"'");
    qDebug()<<"select * from appsversion  where apppath='"+path+"'";
    if(!sql_query->exec())
    {
        qDebug()<<sql_query->lastError();

    }
    else
    {

        while(sql_query->next())
        {
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

        }
    }
    return appsversion;




}
QString  appsitem:: changedatelabe(int date){
    QString strdate=QString::number(date).mid(0,4)+"-"+QString::number(date).mid(4,2)+"-"+QString::number(date).mid(6,2);

    return strdate;

}

void appsitem::resizeappitem(QSize size)
{
    this->setMinimumWidth(size.width());
    this->setMaximumHeight(size.height());
}

QList<apppathspojo *>appsitem::getapppaths(){
    selectappPaths();
    //请求服务
    readconfigforIPService();
    QString  request=ipservicepojo->getUrltype()+"://"+ipservicepojo->getIp()+
            ":"+QString::number(ipservicepojo->getPort())+"/getPaths";
    if(IPLive(ipservicepojo->getIp(), ipservicepojo->getPort(), 1000)){
        QJsonObject resultobj=getfun(request);
        int code=getJsonObjInt("code",resultobj);
        qDebug()<<"14.请求结果:"+QString::number(code);
        //请求通过
        if(code==1){
            QJsonArray qja=getJsonobjArray("data",resultobj);
            qDebug()<<qja.size();

            for(int i=0;i<qja.size();i++){
                QString path=getJsonobjQString("path",qja[i].toObject());
                for(int j=0;j<apppaths.size();j++){

                    if(path==apppaths.at(i)->getPath()){
                        break;
                    }else{
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

void   appsitem::selectappPaths(){

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
