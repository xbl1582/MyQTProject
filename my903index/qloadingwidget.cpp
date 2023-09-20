#include "qloadingwidget.h"
#include <QApplication>
#include<QScreen>
#include<QDebug>
#include<QProcess>
#include<QThread>
#include<QTime>
#include<QLabel>
#include<QMovie>
#include<QTimer>
#include<QTime>
#include<QMessageBox>
#include<QDebug>
QLoadingWidget::QLoadingWidget(QWidget *parent) : QWidget(parent)
{
        setWindowFlags(Qt::FramelessWindowHint);//无边框
        QRect screenRect=QGuiApplication::primaryScreen()->geometry();
        this->screenwidth=screenRect.width();
        this->screenhight=screenRect.height();
        resize(this->screenwidth,this->screenhight);
        connect(this,SIGNAL(clicked()),this,SLOT(mouseClicked()));

}

void QLoadingWidget::initWidge(QString imagepath)
{

    qDebug()<<imagepath<<"-------------------------";


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
    m_nBeginCount=5;
    ql->setText(QString::number(m_nBeginCount));
    timer = new QTimer(this);//创建一个定时器
    //定义触发槽函数
    connect(timer, &QTimer::timeout, this, &QLoadingWidget::OnTimerCountdown);
    BeginCountdown();




}


void QLoadingWidget::mouseReleaseEvent(QMouseEvent *ev)
{

            emit clicked();

   //    this->close();
}



void QLoadingWidget::mouseClicked()
{


     if(open){
         open=false;
           if(timer!=nullptr){
             if (timer->isActive() == true)
                     {
                         timer->stop();
                     }

              delete timer;
              timer= nullptr;
        }
         QProcess *process=new QProcess();
         QString str=doworkspace+"/"+doshellcode;
         process->setWorkingDirectory(QCoreApplication::applicationDirPath()+"/shellcode");
          QString code;
         if(language=="en"){
            code=doshellcode.split(".").at(0)+"en.sh";

         }else{
             code=doshellcode;
         }
          process->start("sh "+code);
         if(process->waitForStarted()){

            QTime _Timer = QTime::currentTime().addMSecs(2000);
            while( QTime::currentTime() < _Timer )
                   QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
             this->close();
            if(process->waitForFinished()){
                open=true;
            }else{
                open=false;
            }

         }else{
             QTime _Timer = QTime::currentTime().addMSecs(3000);
             while( QTime::currentTime() < _Timer ){
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
             }
              this->close();
             open=true;
         }

     }

}


//可以定义一个函数用于触发定时器操作，安全做法
void QLoadingWidget::BeginCountdown()
{
   if (timer->isActive() == false)
   {
      //当前定时器未激活时，激活定时器
      timer->start(1000);
   }
}


void QLoadingWidget::OnTimerCountdown()
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
       QProcess *process=new QProcess();
       QString str=doworkspace+"/"+doshellcode;
       process->setWorkingDirectory(QCoreApplication::applicationDirPath()+"/shellcode");
       QString code;
       if(language=="en"){
           code=doshellcode.split(".").at(0)+"en.sh";

       }else{
        code=doshellcode;
       }
       process->start("sh "+code);
       if(process->waitForStarted()){
          QTime _Timer = QTime::currentTime().addMSecs(2000);
          while( QTime::currentTime() < _Timer )
                 QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
           this->close();

       }else{
           QTime _Timer = QTime::currentTime().addMSecs(3000);
           while( QTime::currentTime() < _Timer ){
                  QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
           }
            this->close();
       }

   }

}

void QLoadingWidget::changelanguage(QString lange)
{
    this->language=lange;

}

void QLoadingWidget::changebg(QString imagepath)
{


}
