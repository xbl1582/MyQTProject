#include "loadingwidget.h"
#include "ui_loadingwidget.h"

loadingwidget::loadingwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loadingwidget)
{
    ui->setupUi(this);
}

void loadingwidget::setappversion(AppsVersion *appversion)
{
    qDebug()<<appversion->getAppname();
    setWindowFlags(Qt::FramelessWindowHint);//无边框
    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
    this->screenwidth=screenRect.width();
    this->screenhight=screenRect.height();
    //    resize(this->screenwidth,this->screenhight);
    connect(this,SIGNAL(clicked()),this,SLOT(mouseClicked()));
    appcurrentpath=QCoreApplication::applicationDirPath();
    basepath=appcurrentpath.split("/KSTMAIN")[0];
    systemsettings = new QSettings (appcurrentpath+"/config/systemsetting.ini", QSettings::IniFormat);//构建函数
    readconfigforsystemsetting();
    myappversion=appversion;
    if(lange=="cn"){
        initWidge(appcurrentpath+"/image/"+myappversion->getAppname()+"Loading.jpg");
    }else{
        initWidge(appcurrentpath+"/image/"+myappversion->getAppname()+"Loadingen.jpg");
    }


    //    ui->setupUi(this);

}



loadingwidget::~loadingwidget()
{
    delete ui;
}
void loadingwidget::initWidge(QString imagepath)
{
    imagepath2=imagepath;
    pixmap = QPixmap(imagepath).scaled(this->screenwidth,this->screenhight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    QPalette pal=this->palette();
    pal.setBrush(QPalette::Background,QBrush(pixmap));
    this->setAutoFillBackground(true);
    setPalette(pal);
    //计时
    ql=new QLabel();
    ql->resize(50,50);
    QFont font;
    font.setPointSize(20);
    ql->setFont(font);
    ql->setAlignment (Qt :: AlignCenter);
    ql->setStyleSheet("border:2px solid rgb(173,194,181); border-radius:25px; color:rgb(173,194,181);text-align:center;");
    ql->move(this->screenwidth-100,40);
    ql->setParent(this);
    m_nBeginCount=2;
    ql->setText(QString::number(m_nBeginCount));
    timer = new QTimer(this);//创建一个定时器
    //定义触发槽函数
    connect(timer, &QTimer::timeout, this, &loadingwidget::OnTimerCountdown);
    BeginCountdown();



}


void loadingwidget::mouseReleaseEvent(QMouseEvent *ev)
{
    emit clicked();
}



void loadingwidget::mouseClicked()
{
    if(timer!=nullptr){
        if (timer->isActive() == true)
        {
            timer->stop();
        }

        delete timer;
        timer= nullptr;
    }
    QString code;
    if(lange=="en"){
        code ="sh "+appcurrentpath+"/shellcode/"+"run"+myappversion->getAppname().toLower()+"en.sh";

    }else{
        code="sh "+appcurrentpath+"/shellcode/"+"run"+myappversion->getAppname().toLower()+".sh";

    }
    const char* commond=code.toLocal8Bit().constData();
    int result=system(commond);

    if(!System_Check(result)){
        //成功
        QTime _Timer = QTime::currentTime().addMSecs(2000);
        while( QTime::currentTime() < _Timer )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        this->close();


    }else{
        //失败
        errormassage(myappversion->getAppname()+"启动失败");

    }




}


//可以定义一个函数用于触发定时器操作，安全做法
void loadingwidget::BeginCountdown()
{
    if (timer->isActive() == false)
    {
        //当前定时器未激活时，激活定时器
        timer->start(1000);
    }
}


void loadingwidget::OnTimerCountdown()
{
    //定时器，没触发一次，时间减1，直到最后一次倒计时为0秒时，停止定时器
    m_nBeginCount -=1;
    //使用QLabel控件显示倒计时的时间
    ql->setText(""+QString::number(m_nBeginCount));
    if(m_nBeginCount == 0)
    {
        //说明，定时器最后一次触发，可以做停止以及其它操作，假设，现在做停止定时器操作
        if (timer->isActive() == true)
        {
            timer->stop();
        }
        delete timer;
        timer= nullptr;
        this->close();
    }else if(m_nBeginCount==1){


        //       启动程序
        QString code;

        if(lange=="en"){
            QFile file(appcurrentpath+"/shellcode/"+"run"+myappversion->getAppname().toLower()+"en.sh");
            if(!file.exists()){
                generscript();
            }
            code ="sh "+appcurrentpath+"/shellcode/"+"run"+myappversion->getAppname().toLower()+"en.sh";

        }else{
            QFile file(appcurrentpath+"/shellcode/"+"run"+myappversion->getAppname().toLower()+".sh");
            if(!file.exists()){
                generscript();
            }
            code="sh "+appcurrentpath+"/shellcode/"+"run"+myappversion->getAppname().toLower()+".sh";

        }

        const char* commond=code.toLocal8Bit().constData();
        int result=system(commond);
        if(!System_Check(result)){

            //成功
            QTime _Timer = QTime::currentTime().addMSecs(2000);
            while( QTime::currentTime() < _Timer )
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            this->close();

        }else{
            //失败
            errormassage(myappversion->getAppname()+"启动失败");
        }




    }

}

void loadingwidget::changelanguage(QString lange)
{
    this->lange=lange;

}

void loadingwidget::changebg(QString imagepath)
{


}
void loadingwidget::readconfigforsystemsetting()
{
    systemsettings=new QSettings (appcurrentpath+"/config/systemsetting.ini", QSettings::IniFormat);
    rote=systemsettings->value("systemconfig/rote").toInt();
    device= systemsettings->value("systemconfig/device").toInt();
    lange=systemsettings->value("systemconfig/lange").toString();
}

void loadingwidget:: generscript(){
    QFile qfile(appcurrentpath+"/shellcode/run"+myappversion->getAppname().toLower()+".sh");
    QFile qfile2(appcurrentpath+"/shellcode/run"+myappversion->getAppname().toLower()+"en.sh");
    QFile qfile3(basepath+"/"+myappversion->getApppath()+"/"+myappversion->getAppname()+"_EN");
    if(!qfile.exists()){
        if(qfile.open(QFile::WriteOnly | QFile::Truncate)){        //文件流写文件
            QTextStream out(&qfile);      //通过文本流控制文件写操作
            QString appname=myappversion->getAppname()=="KSTPXX"?"KSTPXQ":myappversion->getAppname();
            QString   runapp="#!/bin/bash\n COUNT=$(ps -ef |grep "+appname+" |grep -v \"grep\" |wc -l)\n"
                                                                           "if [ $COUNT -eq 0 ]; then \n"
                                                                           "cd "+basepath+"/"+myappversion->getApppath()+"\n"
                                                                                                                         "./"+appname+" &\n"
                                                                                                                                      "else \n"
                                                                                                                                      "echo is RUN \n"
                                                                                                                                      "fi";


            out<<runapp<<endl;
        }
        qfile.close();
    }
    if(!qfile2.exists()){
        if(qfile3.exists()){
            if(qfile2.open(QFile::WriteOnly | QFile::Truncate)){        //文件流写文件
                QTextStream out2(&qfile2);      //通过文本流控制文件写操作
                QString appname=myappversion->getAppname()=="KSTPXX"?"KSTPXQ":myappversion->getAppname();
                QString   runapp="#!/bin/bash\n COUNT=$(ps -ef |grep "+appname+"|grep -v \"grep\" |wc -l)\n"
                                                                               "if [ $COUNT -eq 0 ]; then \n"
                                                                               "cd "+basepath+"/"+myappversion->getApppath()+"\n"
                                                                                                                             "./"+appname+"_EN"+" &\n"
                                                                                                                                                "else \n"
                                                                                                                                                "echo is RUN \n"
                                                                                                                                                "fi";


                out2<<runapp<<endl;
            }
            qfile2.close();
        }else{
            if(qfile2.open(QFile::WriteOnly | QFile::Truncate)){        //文件流写文件
                QTextStream out2(&qfile2);      //通过文本流控制文件写操作
                QString appname=myappversion->getAppname()=="KSTPXX"?"KSTPXQ":myappversion->getAppname();
                QString   runapp="#!/bin/bash\n COUNT=$(ps -ef |grep "+appname+"|grep -v \"grep\" |wc -l)\n"
                                                                               "if [ $COUNT -eq 0 ]; then \n"
                                                                               "cd "+basepath+"/"+myappversion->getApppath()+"\n"
                                                                                                                             "./"+appname+" &\n"
                                                                                                                                          "else \n"
                                                                                                                                          "echo is RUN \n"
                                                                                                                                          "fi";


                out2<<runapp<<endl;
            }
        }

        qfile2.close();
    }

}

int loadingwidget:: System_Check(int result)
{
    if((-1 != result) && (WIFEXITED(result)) && (!(WEXITSTATUS(result))))
        return 0;
    else
        return -1;
}

void loadingwidget::  errormassage(QString txt){
    msgBox=new QMessageBox(QMessageBox::Warning, tr("警告"),tr(txt.toLocal8Bit()), QMessageBox::Ok, this);
    msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
    QFont qf;
    qf.setPixelSize(20);
    msgBox->setFont(qf);
    msgBox->setMinimumWidth(200);
    msgBox->setMinimumHeight(200);
    msgBox->setBaseSize(200,200);

    msgBox->setFixedSize(200,200);
    msgBox->setStyleSheet("QLabel{"

                          "}");
    int result2 = msgBox->exec();
}
