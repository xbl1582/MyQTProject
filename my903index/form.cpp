#include "form.h"
#include "ui_form.h"
#include <QFrame>
#include <QLabel>
#include<QPixmap>
#include<QDebug>
#include<QJsonParseError>
#include<QJsonObject>
#include<QJsonArray>
#include <QToolButton>
#include <QPalette>
#include<QPainter>
#include <QLabel>
#include<QtSvg/QSvgWidget>
#include<QtSvg/QSvgRenderer>
#include<mylabel.h>
#include<myjsonbuttondata.h>
#include<QPropertyAnimation>
#include<QProcess>
#include<QTransform>
#include <QApplication>
#include<QScreen>
#include<mytitlelabel.h>
#include<QImage>
#include<QMessageBox>
#include<qloadingwidget.h>
#include <QDateTime>
#include <QTimer>
#include<QThread>
#include<QDialog>
#include<QFormLayout>
#include<QLineEdit>
#include<QDialogButtonBox>
#include<QHostAddress>
#include<QHostInfo>
#include<QNetworkInterface>
#include<QButtonGroup>
#include <qregexp.h>
#include <leecleartooll.h>
#include<QTranslator>
Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
       this->screenwidth=screenRect.width();
        this->screenhight=screenRect.height();
         ui->setupUi(this);
         ui->gridLayout->setSizeConstraint(QLayout::SetFixedSize);
         Qt::WindowFlags m_flags = windowFlags();
         this->setWindowFlags(m_flags &~Qt::WindowStaysOnTopHint);
          createinit(QCoreApplication::applicationDirPath()+"/image/firstmain.png");
          tr2 = new QTimer(this);//创建一个定时器

          timer2=new QTimer(this);
          connect(timer2, &QTimer::timeout,
                  this,[=]{
              checkmemory();
          });
   timer2->start(1000);
//        定义触发槽函数
        connect(tr2,&QTimer::timeout,this,&Form::OnTimerCountdown);
        BeginCountdown();



}

Form::~Form()
{
    delete ui;
}

void Form::initTranslationWidget()
{

    QList<QWidget *>qflist=greantbutton(QCoreApplication::applicationDirPath()+"/json/mybuttons.json");


     translationWidget = new TranslationWidget(qflist,this);



     //    translationWidget->setMoveEnabled(true);
}
//jishiq
void Form::BeginCountdown()
{
    if (tr2->isActive() == false)
    {
       //当前定时器未激活时，激活定时器
       tr2->start(1000);
    }
}
//jishiqi
void Form::OnTimerCountdown()
{

        QProcess process;
         process.start("lsusb");
         process.waitForFinished();
         QByteArray output = process.readAllStandardOutput();
         QString str_output = output;
         //
         if(str_output.indexOf("3216:1042") >= 0)
         {
             if (tr2->isActive() == true)
             {
                         tr2->stop();
             }
             delete tr2;
             tr2= nullptr;


             initTranslationWidget();
             ui->gridLayout->addWidget(translationWidget);
             createinit(QCoreApplication::applicationDirPath()+"/image/bg.png");

         }



}
//检测内存
void Form::checkmemory()
{


    //memory

    //设置内存大小数值

    QProcess *process=new QProcess();
     QString workspace=QCoreApplication::applicationDirPath()+"/shellcode";
     QString shcode="sh GetDisk.sh";
     process->setWorkingDirectory(workspace);
     process->start(shcode);
     if(process->waitForFinished()){
         process->close();
     }
     QFile f(QCoreApplication::applicationDirPath()+"/shellcode/GetDisk_1.txt");
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

       q3->setText(memory);
    if(numsizedangwei=="G"){
        //
        if(numsize.toInt()<=1){
            if(isshowmessage&&tr2==nullptr){
                QFont qf;
                qf.setPixelSize(30);
             msgBox=new QMessageBox(QMessageBox::Warning, tr("警告"),tr( "存储空间过低请，即时清除空间"), QMessageBox::Ok, this);


            msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
             msgBox->setFont(qf);
             msgBox->setMinimumWidth(200);
             msgBox->setMinimumHeight(200);
             msgBox->setBaseSize(200,200);

              msgBox->setFixedSize(200,200);
             msgBox->setStyleSheet("QLabel{"

                                   "}");
            int result2 = msgBox->exec();
        }
        }
    }else{
               if(isshowmessage &&tr2==nullptr){
                   QFont qf;
                   qf.setPixelSize(30);
       msgBox=new QMessageBox(QMessageBox::Warning, tr("警告"), tr("存储空间过低请，即时清除空间"), QMessageBox::Ok, this);
       msgBox->setFont(qf);
        msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
         msgBox->setMinimumWidth(200);
         msgBox->setMinimumHeight(200);
         msgBox->setBaseSize(200,200);

          msgBox->setFixedSize(200,200);
         msgBox->setStyleSheet("QLabel{"
                               "min-width: 200px;"
                               "min-height: 100px; "
                               "}");
        int result2 = msgBox->exec();
    }
    }
    isshowmessage=false;



}




//--
void Form::memorybutton(QDialog *dialog)
{
    dialog->close();

    leeCleartooll * l=new leeCleartooll (tr("扫描"),{
                                             readJsonfloadname("scannsavepath"),
                                             readJsonfloadname("scannsavepath2"),
                                             readJsonfloadname("scannsavepath3"),
                                             readJsonfloadname("scannsavepath4"),
                                         readJsonfloadname("scannsavepath4"),
                                         readJsonfloadname("scannsavepath5"),
                                         readJsonfloadname("scannsavepath6"),
                                         readJsonfloadname("scannsavepath7")},
    {readJsonfloadname("votesavepath8")}
                                         ,tr("计票"),{readJsonfloadname("votesavepath"),
                                                    readJsonfloadname("votesavepath2"),
                                                    readJsonfloadname("votesavepath3"),
                                                readJsonfloadname("votesavepath4"),
                                                    readJsonfloadname("votesavepath5"),
                                                    readJsonfloadname("votesavepath6"),
                                                    readJsonfloadname("votesavepath7")},
    {readJsonfloadname("votesavepath8")},
                                         tr("测评"),{
                                           readJsonfloadname("wtkpsavepath"),
                                             readJsonfloadname("wtkpsavepath2"),
                                             readJsonfloadname("wtkpsavepath3"),
                                             readJsonfloadname("wtkpsavepath4"),
                                             readJsonfloadname("wtkpsavepath5"),
                                             readJsonfloadname("wtkpsavepath6"),
                                             readJsonfloadname("wtkpsavepath7")
                                         },
    { readJsonfloadname("wtkpsavepath8")});
    l->openWindow();
}
void Form::memorybutton()
{

    leeCleartooll * l=new leeCleartooll (tr("扫描"),{
                                             readJsonfloadname("scannsavepath"),
                                             readJsonfloadname("scannsavepath2"),
                                             readJsonfloadname("scannsavepath3"),
                                             readJsonfloadname("scannsavepath4"),
                                         readJsonfloadname("scannsavepath4"),
                                         readJsonfloadname("scannsavepath5"),
                                         readJsonfloadname("scannsavepath6"),
                                         readJsonfloadname("scannsavepath7")},
    {readJsonfloadname("votesavepath8")}
                                         ,tr("计票"),{readJsonfloadname("votesavepath"),
                                                    readJsonfloadname("votesavepath2"),
                                                    readJsonfloadname("votesavepath3"),
                                                readJsonfloadname("votesavepath4"),
                                                    readJsonfloadname("votesavepath5"),
                                                    readJsonfloadname("votesavepath6"),
                                                    readJsonfloadname("votesavepath7")},
    {readJsonfloadname("votesavepath8")},
                                         tr("测评"),{
                                           readJsonfloadname("wtkpsavepath"),
                                             readJsonfloadname("wtkpsavepath2"),
                                             readJsonfloadname("wtkpsavepath3"),
                                             readJsonfloadname("wtkpsavepath4"),
                                             readJsonfloadname("wtkpsavepath5"),
                                             readJsonfloadname("wtkpsavepath6"),
                                             readJsonfloadname("wtkpsavepath7")
                                         },
    { readJsonfloadname("wtkpsavepath8")});
    l->openWindow();
}
//页面初始化
void Form::createinit(QString path)
{

    //终端-开始
        QPushButton *q=new QPushButton();
        q->move(this->screenwidth-100,this->screenhight-100);
        q->setFlat(true);
        q->setStyleSheet("background-image:url('"+QCoreApplication::applicationDirPath()+"/image/ter.png'); ");
        q->resize(30,30);
        connect(q,&QPushButton::clicked,this,[=](){
            ShowTreaim();
        });
        q->setParent(this);

    //终端--结束

    //修改ip
        QPushButton *q2=new QPushButton();
        q2->move(this->screenwidth-150,this->screenhight-100);
        q2->setFlat(true);
        q2->setStyleSheet("background-image:url('"+QCoreApplication::applicationDirPath()+"/image/ip.png'); ");
        q2->resize(35,30);
        connect(q2,&QPushButton::clicked,this,[=](){
            ShowInputBox();
        });
        q2->setParent(this);
    //内存
        q3=new QPushButton();
        q3->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/image/Drive.png"));
        q3->setIconSize(QSize(30,30));
        QFont qf;
        qf.setPointSize(30);
        q3->setFont(qf);
        q3->resize(150,50);
        q3->move(this->screenwidth-300,this->screenhight-100);
        q3->setFlat(true);
        q3->setText(memory);
        q3->setParent(this);
        q3->setVisible(0);
        connect(q3,&QPushButton::clicked,this,[=](){
            memorybutton();
        });
    //语言切换
     qpb2=new QPushButton();
     qpb2->setMinimumWidth(100);
     qpb2->setFlat(true);
     qpb2->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/image/cn.png"));
     qpb2->setIconSize(QSize(30,30));
     qpb2->move(50,this->screenhight-100);
     qpb2->setParent(this);
    connect(qpb2,&QPushButton::clicked,this,[=](){
        changelanguage();
    });


//------------------------------------创建分页的两个按钮--开始--------------------------------------------------
   //左边按钮 业务目前不需要
  /*
        QToolButton *leftButton=new QToolButton();
        QString leftbuttonsvg=":/image/image6.svg";
        setleftrightbutton(leftbuttonsvg,leftButton,20,(this->screenhight/2)+20,50,50);
        leftButton->setParent(this);
        //点击事件
        connect(leftButton,&QToolButton::clicked,this,[=](){leftbuttronFun();});
   */
   //右边按钮 业务目前不需要
    /*
     *   QToolButton *rightButton=new QToolButton();
     *   QString rightbuttonsvg=":/image/image9.svg";
     *   setleftrightbutton(rightbuttonsvg,rightButton,this->screenwidth-80,(this->screenhight/2)+20,50,50);
     *   rightButton->setParent(this);
     *   connect(rightButton,&QPushButton::clicked,this,[=](){rightbuttonFun();});
     */

//------------------------------------创建分页两个按钮--结束----------------------------------------------------

//------------------------------------设置窗口背景图片 开始-----------------------------------------------------
        pixmap = QPixmap(path).scaled(this->screenwidth,this->screenhight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        QPalette pal=this->palette();
        pal.setBrush(QPalette::Background,QBrush(pixmap));
        this->setAutoFillBackground(true);
        setPalette(pal);
//------------------------------------设置窗口背景图片 结束-----------------------------------------------------





//------------------------------------创建标题 开始------------------------------------------------------------
    /*
        QJsonArray array =readjsonFile(MYJSONPATH,"fontlabel");
        if(array.size()>=0){
            jsonlabelsize=array.size();
          QList<mytitlelabel*> list;
           for(int i=0;i< jsonlabelsize;++i){
               if(i>array.size()){
                   break;
               }
               //初始化卡片对象
             mytitlelabel *mjbd=new mytitlelabel(array.at(i).toObject().value("title").toString(),
                                                 array.at(i).toObject().value("fontsize").toInt(),
                                                 array.at(i).toObject().value( "typeface").toString(),
                                                 array.at(i).toObject().value("style").toString(),
                                                 array.at(i).toObject().value( "labelpositionx").toInt(),
                                                 array.at(i).toObject().value("labelpositiony").toInt()
                         );
           list.append(mjbd);

           }
           for(int j=0;j<list.size();j++){
               QLabel  *label=new QLabel();
               QFont ft;
               mytitlelabel *mjbd=list.at(j);

                label->setText(mjbd->gettitle());
                ft.setFamily(mjbd->gettyeface());
                ft.setPointSize(mjbd->getfontsize());
                qDebug()<<mjbd->getstyle();
                label->setStyleSheet(mjbd->getstyle());
                label->setFont(ft);
                label->move((this->screenwidth/2)+mjbd->getlabelpositionx(),(this->screenhight/2)+mjbd->getlabelpositiony());
                label->setParent(this);

           }


      }
      */

     //-----------------------------创建标题 结束---------------------------
}


//------------------------------------------------卡片样式格式化  开始----------------------------------------------------
void Form::setbuttonPicturestyle(QLabel *label, QString strpath, int svgw, int svgh, QString labelstyle, int labelpositionw, int labelpositionh)
{
    //创建图片显示

//     QSvgRenderer * svgRender=new QSvgRenderer();
//     QImage *qimage=new QImage();
//     svgRender->load(strpath);
//     qimage->load(strpath);
//     QPixmap* labelimg1=new QPixmap(svgw,svgh);
//     labelimg1->fill(Qt::transparent);//透明背景
//     QPainter laberp(labelimg1);
//     svgRender->render(&laberp);
//     label->setPixmap(*labelimg1);
      label->resize(svgw,svgh);
     label->setAlignment(Qt::AlignHCenter);

     label->setStyleSheet("border-image: url('"+QCoreApplication::applicationDirPath()+labelstyle+"');color:rgba(255,255,255,1)");
     label->move(labelpositionw,labelpositionh);

}
//------------------------------------------------卡片样式格式化  结束----------------------------------------------------

//------------------------------------------------按钮生成 开始----------------------------------------------------------

QList<QWidget *> Form::greantbutton(QString jsonpath)
{

    //读取json数据
    //通过json生成按钮，且没次只显示3个
    QList<QWidget *> qflist;
    QJsonArray array =readjsonFile(jsonpath,"buttons");
    qDebug()<<jsonpath;
    jsonbuttonsize=array.size();
    QList<myjsonbuttonData*> list;
    for(int i=0;i< jsonbuttonsize;++i){
      myjsonbuttonData *mjbd=new myjsonbuttonData(
                              array.at(i).toObject().value("typeface").toString(),
                              array.at(i).toObject().value("fontsize").toInt(),
                                array.at(i).toObject().value("name").toString(),
                                array.at(i).toObject().value("directorypath").toString(),
                                array.at(i).toObject().value("shellname").toString(),
                                array.at(i).toObject().value("svg").toString(),
                                array.at(i).toObject().value("svgwidth").toInt(),
                                array.at(i).toObject().value("svgheight").toInt(),
                                array.at(i).toObject().value("buttonx").toInt(),
                                array.at(i).toObject().value("buttony").toInt(),
                                array.at(i).toObject().value("labelpositionx").toInt(),
                                array.at(i).toObject().value("labelpositiony").toInt(),
                                array.at(i).toObject().value("buttonstyle").toString(),
                                array.at(i).toObject().value("labelstyle").toString(),
                              array.at(i).toObject().value("bgpath").toString()
                          );
            list.append(mjbd);

             }
    for(int j=0;j<list.size()/3;j++){
     QFrame *qf=new QFrame();
     qf->resize(this->screenwidth-150,this->screenhight-100);
     qflist.append(qf);
    }

    if(list.size()%3>=1){
     QFrame *qf=new QFrame();
     qf->resize(this->screenwidth-150,this->screenhight-100);
     qflist.append(qf);
    }
    allpage=qflist.size();
    int listsize=list.size();
    for(int j=0;j<allpage;j++){
        for(int k=0;k<listsize;k++){

            /*
             * //生成按钮
             *QPushButton *qpb=new QPushButton();
             *QFont qfont;
             *qfont.setFamily(list.at(k)->gettypeface());
             *qfont.setPointSize(list.at(k)->getFontSize());
             *qpb->setText(list.at(k)->getName());
             *qpb->setFont(qfont);
             *qpb->move((k%3)*this->screenwidth/3+list.at(k)->getX(),this->screenhight/3+list.at(k)->getY());
             *qpb->setStyleSheet(list.at(k)->getButtonstyle());
             *qpb->setFlat (true);
             *connect(qpb,&QPushButton::clicked,qflist.at( k/3),[=](){onclickbutton(list.at(k)->getDirectorypath(),list.at(k)->getFuncode());});
             * qpb->setParent(qflist.at( k/3));
            */
          //卡片
             Mylabel *mylabel=new Mylabel;
             qDebug()<<list.at(k);
             setbuttonPicturestyle(mylabel,
                                   list.at(k)->getSvg(),
                                   list.at(k)->getSvgwidth(),
                                   list.at(k)->getSvgheight(),
                                   list.at(k)->getLabelstyle(),
                                   (k%3)*this->screenwidth/3+list.at(k)->getLabelpositionx(),
                                   this->screenhight/3+list.at(k)->getLabelpositionY());
             mylabel->setParent(qflist.at(k/3));

          //卡片点击
          //connect( mylabel,&Mylabel::changestyle,qflist.at( k/3),[=](){changestyle(*mylabel);}             )       ;
         connect(mylabel,&Mylabel::clicked,qflist.at(k/3),
                 [=](){
                    onclickbutton(
                                  list.at(k)->getDirectorypath(),
                                  list.at(k)->getFuncode(),
                                  QCoreApplication::applicationDirPath()+list.at(k)->getBgpath()
                                  );
                        }
                  );


        }
    }
    return qflist;

}

//------------------------------------------------按钮生成 结束---------------------------------------------------------




//------------------------------------------------卡片点击事件 开始-----------------------------------------------
void Form::onclickbutton(QString directory, QString sheelcode,QString bgpath)
{
    QRegExp rx("\\d+");
    rx.indexIn(memory,0);
   QString numsize=rx.cap(0);
   qDebug()<<numsize;

   QString numsizedangwei=memory.at(memory.size()-1);
   qDebug()<<numsizedangwei;
   if(numsizedangwei=="G"){
       //numsize.toInt()<=1
       if(numsize.toInt()<=1){
          q3->setVisible(1);


       }
   }
   if(numsizedangwei=="G"){
       //numsize.toInt()<=1
       if(numsize.toInt()<=1){
           QFont qf;
           qf.setPixelSize(30);

            msgBox=new QMessageBox(QMessageBox::Warning, tr("警告"), "存储空间过低请，即时清除空间", QMessageBox::Ok, this);
            msgBox->setFont(qf);
           msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
            msgBox->setMinimumWidth(200);
            msgBox->setMinimumHeight(200);
            msgBox->setBaseSize(200,200);

             msgBox->setFixedSize(200,200);
            msgBox->setStyleSheet("QLabel{"

                                  "}");
           int result2 = msgBox->exec();
       }else{


           Qt::WindowFlags m_flags=windowFlags();
           //根据是否存在过渡图片路径判断是否需要创建新的窗口
          if(bgpath!=nullptr||bgpath!=""){
              qw=new QLoadingWidget();
              qw->changelanguage(langue);
              qw->doworkspace=directory;
              qw->doshellcode=sheelcode;
              qw->setAttribute(Qt::WA_DeleteOnClose, true);
              qw->changebg(bgpath);
              qw->initWidge(bgpath);

                Qt::WindowFlags qm_flags = qw->windowFlags();
              qw->setWindowFlags(qm_flags | Qt::WindowStaysOnTopHint);
          }
          //创建执行sh文件的程序
         QProcess *process=new QProcess();

           if(bgpath!=nullptr||bgpath!=""){
               //但点击该子窗口时执行

                      this->setWindowFlags(m_flags &~Qt::WindowStaysOnTopHint);
                        qw->showFullScreen();



           }else{
                   QString str=directory+"/"+sheelcode;
                   process->setWorkingDirectory(directory);
                   process->start(str);
   //                qDebug()<<"zhix";
   //                this->setWindowFlags(m_flags &~Qt::WindowStaysOnTopHint);
                   if(process->waitForFinished()){
                      process->close();
                   }else{
   //                     this->setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
                   qDebug()<<process->errorString();
                   }
           }




       }
   }else{
       QFont qf;
       qf.setPixelSize(30);
      msgBox=new QMessageBox(QMessageBox::Warning, tr("警告"), "存储空间过低请，即时清除空间", QMessageBox::Ok, this);
       msgBox->setFont(qf);
       msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
        msgBox->setMinimumWidth(200);
        msgBox->setMinimumHeight(200);
        msgBox->setBaseSize(200,200);

         msgBox->setFixedSize(200,200);
        msgBox->setStyleSheet("QLabel{"

                              "}");
       int result2 = msgBox->exec();
   }







}
//------------------------------------------------卡片点击事件 结束-----------------------------------------------




//------------------------------------------------卡片点击修改样式 开始------------------------------------
//void Form::changestyle(Mylabel &mybale)
//{
//    //    mybale.setStyleSheet("background-color:rgba(0,0,0,0.1);");
//}
//------------------------------------------------卡片点击修改样式 结束--------------------------------



//------------------------------------------------读取json文件数组 开始-----------------------------
QJsonArray Form::readjsonFile(QString filePath,QString youwant)
{
         QJsonArray resultarray;
      //通过Qfile读取json文件
      QFile loadFile(filePath);
      if(!loadFile.open(QIODevice::ReadOnly| QIODevice::Text))
      {

             return resultarray;
      }
        QByteArray array = loadFile.readAll(); //将数据读到array中
         loadFile.close();
        //json错误抛出
        QJsonParseError json_error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(array,&json_error);
        if(json_error.error != QJsonParseError::NoError)//判断是否可以解析
               {
                   qDebug()<<"json error!";
                   return resultarray;
         }
      QJsonObject rootObj = jsonDoc.object();
    if(rootObj.contains(youwant)){
      QJsonValue value = rootObj.value(youwant);
      if(value.isArray())
          {
         resultarray = value.toArray();
         return resultarray;
      }
    }
}
//------------------------------------------------读取json文件 结束-------------------------






//展示终端
void Form::ShowTreaim(){
    QProcess *process=new QProcess();
     QString workspace="./shellcode";
     QString shcode="sh startTerminal.sh";
     process->setWorkingDirectory(workspace);
     process->start(shcode);
     if(process->waitForFinished()){
         process->close();
     }
     //
}


//弹出消息框

void Form::ShowInputBox()
{
    QHostAddress address = QHostAddress(QHostAddress::LocalHost);
    QString localHostName = QHostInfo::localHostName();
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    QString ipv6;

    for (const QNetworkInterface& interface : list) {
            qDebug()<<interface;

           if (interface.type() == QNetworkInterface::Loopback) {
               continue;
           }
        QList<QNetworkAddressEntry> entryList = interface.addressEntries();
        for (const QNetworkAddressEntry& entry : entryList) {

                  if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol) {
                      ipv6 = entry.ip().toString();
                  } else if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                      qDebug()<<entry.ip().toString();
                        if(interface.name()=="eth0")
                           ipv4 = entry.ip().toString();
                        else if (interface.name()=="eth1")
                              ipv42= entry.ip().toString();
                  }
              }
        }

    QDialog dialog(this);
    dialog.setWindowTitle(tr("修改ip地址"));
    dialog.resize(QSize(600,400));
    QGridLayout form(&dialog);

    QFont qf;
    // Value1
    QLabel *val1=new QLabel("ip1: ");
    qf.setPixelSize(30);
    val1->setFont(qf);
    QLineEdit *qle1=new QLineEdit(&dialog);
    qle1->setMinimumHeight(60);
    qle1->setText(ipv4);

    qle1->setFont(qf);
    form.addWidget(val1,1,1,1,1);
    form.addWidget(qle1,1,2,1,6);
    // Value2
    QLabel *val2=new QLabel("ip2:");
    val2->setFont(qf);
    QLineEdit *qle2=new QLineEdit(&dialog);
      qle2->setMinimumHeight(60);
    qle2->setReadOnly(true);
    qle2->setFont(qf);
     qle2->setText(ipv42);
     form.addWidget(val2,2,1,1,1);
     form.addWidget(qle2,2,2,1,6);



    //显示按钮键盘


    for(int i=0;i<12;i++){
        QPushButton *qtb=new QPushButton(&dialog);
        qtb->resize(50,60);
        QFont qf2;
        qf2.setPointSize(18);
        qtb->setFont(qf2);
        qtb->setMinimumHeight(60);
        if(i==0){
            qtb->setText(QString::number(i));
            connect(qtb,&QPushButton::clicked,qtb,[=](){
                   ipv4+=QString::number(i);
                    qle1->setText(ipv4);
            });
            form.addWidget(qtb,10,2,2,2);
        }
        else if(i==10){
            form.addWidget(qtb,10,4,2,2);
            qtb->setText(".");
            connect(qtb,&QPushButton::clicked,qtb,[=](){
                ipv4+=".";
                 qle1->setText(ipv4);

            });

        }else if(i==11){
            qtb->setText("←");
            form.addWidget(qtb,10,6,2,2);
            connect(qtb,&QPushButton::clicked,qtb,[=](){
                int size=ipv4.size();
               ipv4=ipv4.left(size-1);
                qle1->setText(ipv4);
            });

        }else{
             qtb->setText(QString::number(i));
             connect(qtb,&QPushButton::clicked,qtb,[=](){
                 ipv4+=QString::number(i);
                  qle1->setText(ipv4);
             });
             switch (i) {
                case 1:form.addWidget(qtb,8,2,2,2);break;
             case 2:form.addWidget(qtb,8,4,2,2);break;
             case 3: form.addWidget(qtb,8,6,2,2);break;
             case 4:form.addWidget(qtb,6,2,2,2);break;
             case 5:form.addWidget(qtb,6,4,2,2);break;
             case 6: form.addWidget(qtb,6,6,2,2);break;
             case 7:form.addWidget(qtb,4,2,2,2);break;
             case 8:form.addWidget(qtb,4,4,2,2);break;
             case 9:form.addWidget(qtb,4,6,2,2);break;

             }
        }






    }



    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    buttonBox.setStyleSheet("text-align:center");
    buttonBox.button(QDialogButtonBox::Ok)->setText(tr("保存"));
    QFont qf2;
    qf2.setPixelSize(25);
    buttonBox.button(QDialogButtonBox::Ok)->setMinimumHeight(50);
buttonBox.button(QDialogButtonBox::Ok)->setFont(qf2);
    buttonBox.button(QDialogButtonBox::Ok)->setMinimumWidth(150);
  buttonBox.button(QDialogButtonBox::Ok)->setStyleSheet("background-color:rgb(120,120,255); border-radius:5px;color:rgb(255,255,255);margin-left:30px;");

     buttonBox.button(QDialogButtonBox::Cancel)->setText(tr("取消"));
          buttonBox.button(QDialogButtonBox::Cancel)->setFont(qf2);
      buttonBox.button(QDialogButtonBox::Cancel)->setMinimumHeight(50);
            buttonBox.button(QDialogButtonBox::Cancel)->setMinimumWidth(150);
          buttonBox.button(QDialogButtonBox::Cancel)->setStyleSheet("background-color:rgb(255,0,0); border-radius:5px;color:rgb(255,255,255);margin-right:30px;");
          QPushButton *q3=new QPushButton();
          q3->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/image/Drive.png"));
          q3->setIconSize(QSize(30,30));
          QFont qf3;
          qf.setPointSize(30);
          q3->setFont(qf3);
          q3->resize(150,50);
          q3->move(this->screenwidth-300,this->screenhight-100);
          q3->setFlat(true);
          q3->setText(memory);
          q3->setParent(this);
            QDialog *d=&dialog;
          connect(q3,&QPushButton::clicked,this,[=](){

              memorybutton(d);
          });
          form.addWidget(q3,12,1,1,1);
          form.addWidget(&buttonBox,12,3,1,3);


    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
       qDebug()<<"执行ip修改"<<endl;
       QString ip1=qle1->text();
       QString ip2=qle2->text();
       QFile qf("./shellcode/changIP.sh");
          if(qf.open(QFile::WriteOnly | QFile::Truncate)){        //文件流写文件
                 QTextStream out(&qf);      //通过文本流控制文件写操作
                 QString changIPcontent=
                         "#!/bin/bash\nip address add "+ip1+"/24 dev eth0\n";

                if(ip2!=""){
                    changIPcontent+=+"ip address add "+ip2+"/24 dev eth1\n";
                }
                 out<<changIPcontent<<endl;


            }
        qf.close();

        QProcess *process=new QProcess();
         QString workspace="./shellcode";
         QString shcode="sh changIP.sh";
         process->setWorkingDirectory(workspace);
         process->start(shcode);
         if(process->waitForFinished()){
             process->close();
         }

   }


  }











//-----------------------------------------------------------------------------------------------------------------------------





//-----------------------------------------------窗口分页点击事件  开始---------------------------------------
void Form::on_pushButton_clicked(int page)
{
    translationWidget->setCurrentPage(page,true);
}


//-----------------------------------------------窗口分页点击事件  结束---------------------------------------





//----------------------------------------------消息对话框  业务不需要------------------------------------------
void Form::messageBox()
{

}
//----------------------------------------------消息对话框  业务不需要------------------------------------------




//----------------------------------------------按钮点击事件 业务不需要------------------------------------------
/*void Form::onclickbutton(QString directory, QString sheelcode)
{

    QProcess *process=new QProcess();
        QPropertyAnimation *mMainWidgetHideAnimation_ = new QPropertyAnimation(this, "geometry");
        mMainWidgetHideAnimation_ ->setDuration(200);
        QRect startRect = QRect(450,300,1000,450);
        QRect stopRect = QRect(500,300, 1000,450);
        mMainWidgetHideAnimation_ ->setStartValue(startRect);
        mMainWidgetHideAnimation_ ->setEndValue(stopRect);
        mMainWidgetHideAnimation_->start();
         connect(mMainWidgetHideAnimation_, &QPropertyAnimation::finished, [=] {
                   this->setWindowState(Qt::WindowMinimized);
             QString str=directory+"/"+sheelcode;
             process->setWorkingDirectory(directory);

             process->start(str);
             if(process->waitForStarted()){
             qDebug()<<"success";
             }else{
             qDebug()<<"error";
             qDebug()<<process->errorString();
             }

         });
         process->close();
}
*/
//----------------------------------------------按钮点击事件 业务还不需要----------------------------------------











//---------------------------------------------设置左右分页按钮样式 业务目前不需要-------------------------------------------------
void Form::setleftrightbutton(QString svgpath,QToolButton *button, int x, int y, int buttonsizex, int buttonsizey)
{
    QSvgRenderer * svgRender=new QSvgRenderer();
    svgRender->load(svgpath);
    QPixmap* labelimg1=new QPixmap(buttonsizex,buttonsizey);

    labelimg1->fill(Qt::transparent);//透明背景
    QPainter *qpainter=new QPainter(labelimg1);
    svgRender->render(qpainter);
    QIcon *q=new QIcon();
    q->addPixmap(*labelimg1);

    button->setIcon(*q);
    button->setIconSize(QSize(buttonsizex,buttonsizey));
    button->resize(buttonsizex,buttonsizey);
    button->setAutoFillBackground(Qt::transparent);
    button->setStyleSheet("border-radius:20px;");
    button->move(x,y);
}
//---------------------------------------------设置左右分页按钮样式 业务目前不需要-------------------------------------------------

//---------------------------------------------左边翻页- 业务目前不需要------------------------------------------------------------------
void Form::leftbuttronFun()
{

    if(currentpage>0){
        on_pushButton_clicked(--currentpage);
    }else{
         on_pushButton_clicked(0);
    }
}
//---------------------------------------------左边翻页- 业务目前不需要-------------------------------------------------------------

//---------------------------------------------右边翻页- 业务目前不需要-------------------------------------------------------------
void Form::rightbuttonFun()
{

    if(currentpage<allpage){
        on_pushButton_clicked(++currentpage);
    }

}
//---------------------------------------------右边翻页- 业务目前不需要-------------------------------------------------------------

QString Form:: readJsonfloadname(QString key){
    //读取json文件
    QString filepath=QCoreApplication::applicationDirPath()+"/json/pathjson.json";
    QFile loadFile(filepath);
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

void Form::changelanguage()
{

    if(langue=="cn"){


        m_translator=new QTranslator();
        QString langqmfilepath = qApp->applicationDirPath() + QString("/language/translation_en_AS.qm");
            if(QFile(langqmfilepath).exists())
            {
            m_translator->load(langqmfilepath);
            qApp->installTranslator(m_translator);
            }
            else
            qDebug()<<"[houqd] authclient language file does not exists ...";

            if(translationWidget!=nullptr){
            delete(translationWidget);
            translationWidget=nullptr;
            QList<QWidget *>qflist=greantbutton(QCoreApplication::applicationDirPath()+"/json/mybuttonsen.json");


             translationWidget = new TranslationWidget(qflist,this);
              ui->gridLayout->addWidget(translationWidget);
            }
            pixmap = QPixmap(QCoreApplication::applicationDirPath()+"/image/bgen.png").scaled(this->screenwidth,this->screenhight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            QPalette pal=this->palette();
            pal.setBrush(QPalette::Background,QBrush(pixmap));
            this->setAutoFillBackground(true);
            setPalette(pal);



            //重新刷新界面
            ui->retranslateUi(this);

        //切换英文
        langue="en";
    }else{


        m_translator=new QTranslator();
        QString  langqmfilepath = qApp->applicationDirPath() + QString("/language/translation_cn_zh_CN.qm");
            if(QFile(langqmfilepath).exists())
            {
            m_translator->load(langqmfilepath);
            qApp->installTranslator(m_translator);
            }
            else
            qDebug()<<"[houqd] authclient language file does not exists ...";
            if(translationWidget!=nullptr){
            delete(translationWidget);
            translationWidget=nullptr;
            QList<QWidget *>qflist=greantbutton(QCoreApplication::applicationDirPath()+"/json/mybuttons.json");


             translationWidget = new TranslationWidget(qflist,this);
              ui->gridLayout->addWidget(translationWidget);
            }

            pixmap = QPixmap(QCoreApplication::applicationDirPath()+"/image/bg.png").scaled(this->screenwidth,this->screenhight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            QPalette pal=this->palette();
            pal.setBrush(QPalette::Background,QBrush(pixmap));
            this->setAutoFillBackground(true);
            setPalette(pal);

        qpb2->setText("简体中文");
        //切换中文
        langue="cn";
    }
}
