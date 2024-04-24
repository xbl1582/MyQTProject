#include "scanner.h"
#include "ui_scanner.h"
scanner::scanner(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::scanner)
{

    Qt::WindowFlags m_flags = windowFlags();
    this->setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
    this->screenwidth=screenRect.width();
    this->screenheight=screenRect.height();
    this->setMinimumWidth(screenwidth);
    this->setMinimumHeight(screenheight);

   ui->setupUi(this);
   QString commondstart903Q="sh "+QCoreApplication::applicationDirPath()+"/shellcode/statrtKST903Q.sh";
   const char* code=commondstart903Q.toUtf8().data();
   system(code);
    initScannerIndexPage();

    sunimagewidget=new imagewidget();
    sunscannerwidget=new scannerwidget();


}

scanner::~scanner()
{
    delete ui;
}

void scanner::initScannerIndexPage()
{
    ui->horizontalWidget->resize(screenwidth,screenheight);

    //时间
   timelabel=new Mylabel();
   QString tempstr=tr("SCAN");
   timelabel->setText(tempstr);
   timelabel->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
   QFont qf;
   qf.setPointSize(40);
   qf.setFamily("黑体");
   timelabel->setFont(qf);
    initwidget(timelabel,400,400,100,100,"background-color:transparent;border-radius:20px;color:rgba(255,255,255,1)");
    ui->horizontalLayout_9->addWidget(timelabel);




    //计数
    numlabel=new Mylabel();
    numlabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    timer2=new QTimer(this);
    numlabel->setText(tr("Scanned：")+QString::number(0)+tr("."));
    connect(timer2, &QTimer::timeout,
            this,[=]{
         numlabel->setText(tr("Scanned：")+QString::number(readfloadimage())+tr("   ")+tr("total:")+QString::number(readfloadimage2())+tr("  "));
    });
    timer2->start(1000);

    qf.setPixelSize(30);
    numlabel->setFont(qf);
//    border-image:url(':/image/p3.png');
    initwidget(numlabel,550,550,200,200,"border-radius:20px;color:rgba(255,255,255,1);padding-left:0px;");

    ui->verticalLayout_5->addWidget(numlabel);
    //语言设置
//    qf.setPixelSize(20);
//    ui->comboBox->setFont(qf);
//    ui->comboBox->setStyleSheet("background-color:transparent;color:#FFFFFF;");
//     ui->comboBox->view()->parentWidget()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
//     ui->comboBox->view()->parentWidget()->setAttribute(Qt::WA_TranslucentBackground);
//    ui->comboBox->view()->parentWidget()->setStyleSheet("background-color:rgba(0,0,0,0.5);color:#FFFFFF;");
//    ui->comboBox->addItem("简体中文");
//    ui->comboBox->addItem("English");
//    connect(ui->comboBox, &QComboBox::currentTextChanged, this, [=]{
//        loadLanguage(ui->comboBox->currentText());
//    });



    QFont qf2;
    //22
    qf2.setPixelSize(22);
    //扫描窗
     scannerWidget=new MyWidget();
    title2=new Mylabel();
     QString tempname=tr("Scan");
      title2->setText(tempname);//Scan
      title2->setFont(qf2);
      title2->setStyleSheet("color:rgb(255,255,255);background:transparent;");
      title2->move(75,140);
     initwidget(scannerWidget,200,200,180,180,"background-color:rgba(243,170,96,1);border-radius:20px;");
     ui->verticalLayout_9->addWidget(scannerWidget);
       title2->setParent(scannerWidget);
    //扫描按钮
     scannerlabel=new Mylabel();
     intilabel(scannerWidget,scannerlabel,QCoreApplication::applicationDirPath()+"/icon/xianxingdayinji.png",100,100,50,20,110,110,110,110);


     connect(scannerlabel,&Mylabel::clicked,this,[=]{
         //扫描窗口点击事件
        scannerwidgeclick();
     });
     connect( scannerWidget,&MyWidget::clicked,this,[=]{
         //扫描窗口点击事件
        scannerwidgeclick();
     });



    //图片窗
    imagelWidget=new MyWidget();
    initwidget(imagelWidget,200,200,180,180,"background-color:#F7FFE5;border-radius:20px;");
    title=new Mylabel();
      QString tempname2=tr("processing");
     title->setText(tempname2);//Image Processing

      title->setFont(qf2);
     title->move(45,140);//
     title->setStyleSheet("color:rgb(255,255,255);background:transparent;");
    ui->verticalLayout_8->addWidget(imagelWidget);
      title->setParent(imagelWidget);
    imagelabel=new Mylabel();
    intilabel(imagelWidget,imagelabel,QCoreApplication::applicationDirPath()+"/icon/tupian.png",100,100,50,20,110,110,110,110);
    connect(imagelabel,&Mylabel::clicked,this,[=]{
        //图片窗口点击事件
        imagewidgeclick();
    });
    connect(imagelWidget,&MyWidget::clicked,this,[=]{
        //图片窗口点击事件
        imagewidgeclick();
    });


     //设置窗
      settingWidget=new MyWidget();
      title3=new Mylabel();
      QString tempname3=tr("setting");
       title3->setText(tempname3);//Settings
        title3->setFont(qf2);
       title3->move(65,140);//70
       title3->setStyleSheet("color:rgb(255,255,255);background:transparent;");
      initwidget(settingWidget,200,200,180,180,"background-color:rgba(200,200,200,1);border-radius:20px;");
      ui->verticalLayout_7->addWidget(settingWidget);
       title3->setParent(settingWidget);



     //设置按钮
      settinglabel=new Mylabel();
      intilabel(settingWidget,settinglabel,QCoreApplication::applicationDirPath()+"/icon/shezhi.png",100,100,50,20,110,110,110,110);
      connect(settinglabel,&Mylabel::clicked,this,[=]{
          //设置窗口点击事件
         settingwidgetclick();
      });
      connect(settingWidget,&MyWidget::clicked,this,[=]{
          //设置窗口点击事件
         settingwidgetclick();
      });





     //退出按钮
      outWidget=new MyWidget();
      title4=new Mylabel();
       QString tempname4=tr("close");
       title4->setText(tempname4);//退出Exit
        title4->setFont(qf2);
         title4->move(85,140);//
         title4->setStyleSheet("color:rgb(255,255,255);background:transparent;");
      initwidget(outWidget,200,200,180,180,"background-color:#394867;border-radius:20px;");
      ui->verticalLayout_6->addWidget(outWidget);
       title4->setParent(outWidget);
      outlabel=new Mylabel();
      intilabel(outWidget,outlabel,QCoreApplication::applicationDirPath()+"/icon/tuichu.png",100,100,50,20,110,110,110,110);

      connect(outlabel,&Mylabel::clicked,this,[=]{
          //退出窗口点击事件
         outwidgetclick();
      });
      connect(outWidget,&MyWidget::clicked,this,[=]{
          //退出窗口点击事件
        outwidgetclick();
      });

      timer4=new QTimer(this);

      connect(timer4, &QTimer::timeout,
              this,[=]{
          checkscannfinsh();
      }
      );

    //设置背景图片
    QPixmap pixmap2 = QPixmap(QCoreApplication::applicationDirPath()+"/image/bg.jpg").scaled(this->screenwidth,this->screenheight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    QPalette pal=this->palette();
    pal.setBrush(QPalette::Background,QBrush(pixmap2));
    this->setAutoFillBackground(true);
    setPalette(pal);



}

void scanner::intilabel(QWidget *q,Mylabel *label, QString path, int imagewith, int imageheight, int maginw, int maginh, int minwidth, int maxwidth, int minheight, int maxheight)
{
    qpixmap=new QPixmap(path);
    qpixmap->scaled(imagewith,imageheight,Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    label->move(maginw,maginh);
    label->setScaledContents(true);
    label->setMinimumWidth(minwidth);
    label->setMaximumWidth(maxwidth);
    label->setMinimumHeight(minheight);
    label->setMaximumHeight(maxheight);
    label->setPixmap(*qpixmap);
    label->setParent(q);

}

void scanner::initwidget(QWidget *q,int minwidth,int maxwidth,int minheight,int maxheight,QString style)
{
    q->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    q->setMinimumWidth(minwidth);
    q->setMinimumHeight(minheight);
    q->setMaximumWidth(maxwidth);
    q->setMaximumHeight(maxheight);
    q->setStyleSheet(style);
}
/**
 * @brief scanner::imagewidgeclick
 *  图片框点击触发事件
 *
 */
void scanner::imagewidgeclick()
{
//    qDebug()<<"图片框点击事件"<<endl;
    sunimagewidget->resize(this->screenwidth,this->screenheight);
    Qt::WindowFlags m_flags =sunimagewidget->windowFlags();
//     Qt::WindowStaysOnTopHint
       sunimagewidget->setWindowFlags(m_flags |Qt::WindowStaysOnTopHint);
//    sunimagewidget->showFullScreen();
        sunimagewidget->show();
        if(falg){
             sunimagewidget->initimagewidget();
              sunimagewidget->changelabevalue();

       }
        falg=false;

}
/**
 * @brief scanner::scannerwidgeclick
 *  扫描框点击触发事件
 *
 */
void scanner::scannerwidgeclick()
{
    //点击扫描的时候，弹出文件路径选择框


        QString inipath=QCoreApplication::applicationDirPath()+readJsonfloadname("inipath");
        QString scannerpagepath=QCoreApplication::applicationDirPath()+readJsonfloadname("scannerpageini");
    //    qDebug()<<inipath;
        //读取到ini位置
        QSettings *settings;//申明一个QSetting类函数
         QSettings *settings2;//申明一个QSetting类函数
        settings = new QSettings (inipath, QSettings::IniFormat);//构建函数


        settings2=new QSettings(scannerpagepath,QSettings::IniFormat);
        QString iniupdateitem=readJsonfloadname("iniupdateitem");
         QString iniupdateitem2=readJsonfloadname("iniupdateitem2");
        QString scannerpageitem=readJsonfloadname("scannerpageitem");
        //修改原始文件所在位置
    //    folderpath=folderpath+"/";
        folderpath=settings->value(iniupdateitem2).toString();
        settings2->setValue(scannerpageitem,folderpath);

        //启动扫描仪
    //    system("pwd > /mnt/scannertest.txt");
        QString commond="sh "+QCoreApplication::applicationDirPath()+"/shellcode/openscanner.sh";
        const char*code=commond.toUtf8().data();
        int result=system(code);
         if(!(System_Check(result))){
             if(!timer4->isActive()){
                 timer4->start(1000);
             }
         }else{
             QFont qfs;
             qfs.setPixelSize(30);
             QMessageBox msgBox(QMessageBox::Warning, tr("scan fail"), tr("scan fail"), QMessageBox::Ok, this);
             msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
              msgBox.setMinimumWidth(200);
              msgBox.setMinimumHeight(200);
              msgBox.setBaseSize(200,200);
              msgBox.setFont(qfs);
               msgBox.setFixedSize(200,200);
              msgBox.setStyleSheet("QLabel{"
                                    "min-width: 200px;"
                                    "min-height: 100px; "
                                    "}");
             int result2 = msgBox.exec();
         }

}

/**
 * @brief scanner::settingwidgetclick
 *  设置框点击触发事件
 *
 */
void scanner::settingwidgetclick()
{
//  qDebug()<<"设置框点击事件"<<endl;
  QProcess *process=new QProcess();
   QString workspace=QCoreApplication::applicationDirPath()+readJsonfloadname("settingworkpath");
   process->setWorkingDirectory(workspace);
   process->start("sh settshellcodeen.sh");
   if(process->waitForFinished()){
       process->close();
   }


}
/**
 * @brief scanner::outwidgetclick
 * 退出框点击触发事件
 */
void scanner::outwidgetclick()
{
        if(timer->isActive()){
            timer->stop();
        }
        timer=nullptr;
        delete(timer);
        timer2=nullptr;
        delete(timer2);
        this->close();
        qApp->exit(0);
}

void scanner::showTime()
{
        QTime time = QTime::currentTime();          //获取当前时间
        QString strTime = time.toString("hh:mm");   //将获取的时间转换为字符串 "时:分"
        timelabel->setText(strTime);

}

int scanner::readfloadimage()
{
    QString inipath=QCoreApplication::applicationDirPath()+readJsonfloadname("inipath");

//    //读取到ini位置
    QSettings *settings;//申明一个QSetting类函数

    settings = new QSettings (inipath, QSettings::IniFormat);//构建函数


    QString iniupdateitem=readJsonfloadname("iniupdateitem");
//    //修改原始文件所在位置
//    folderpath=folderpath+"/";
    folderpath=settings->value(iniupdateitem).toString();
    QStringList qlist=folderpath.split("/");
    QString foldername=qlist.at(qlist.size()-1);
    folderpath=QCoreApplication::applicationDirPath()+readJsonfloadname("parentpath")+foldername;

           QProcess process;
           QString workspace=QCoreApplication::applicationDirPath()+"/KST-903/";

           process.setWorkingDirectory(workspace);
           process.start("cat NowScanCount");
           process.waitForFinished();
           QByteArray output=process.readAllStandardOutput();
           QString str_out=output;
           int count_num=str_out.toInt();
           return count_num;

}
int scanner::readfloadimage2()
{
    QString inipath=QCoreApplication::applicationDirPath()+readJsonfloadname("inipath");

//    //读取到ini位置
    QSettings *settings;//申明一个QSetting类函数

    settings = new QSettings (inipath, QSettings::IniFormat);//构建函数


    QString iniupdateitem=readJsonfloadname("iniupdateitem2");
//    //修改原始文件所在位置
//    folderpath=folderpath+"/";
    folderpath=settings->value(iniupdateitem).toString();
    QStringList qlist=folderpath.split("/");
    QString foldername=qlist.at(qlist.size()-1);
    folderpath=QCoreApplication::applicationDirPath()+readJsonfloadname("parentpath")+foldername;
    QDir dir(folderpath);
    if(dir.exists()){
        //设置过滤器
           QStringList filters;
           filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.tif")<<QString("*.png")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp");
          dir.setFilter (QDir::Files | QDir::NoSymLinks); // 设置类型过滤器，只为文件格式
           dir.setNameFilters (filters);  // 设置文件名称过滤器，只为 filters 格式（后缀为.jpeg 等图片格式）
           int dir_count = dir.count();

           return dir_count;
    }


}
QString scanner:: readJsonfloadname(QString key){
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


void scanner::movefiletoresult()
{
    //读取json文件
    QString filepath=QCoreApplication::applicationDirPath()+"/json/pathjson.json";
    QFile loadFile(filepath);;
          if(!loadFile.open(QIODevice::ReadOnly| QIODevice::Text))
          {

                 return ;
          }
    QByteArray array = loadFile.readAll(); //将数据读到array中
    loadFile.close();
    //json错误抛出
    QJsonParseError json_error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(array,&json_error);
    if(json_error.error != QJsonParseError::NoError)//判断是否可以解析
     {
           qDebug()<<"json error!";
           return ;
      }
     QJsonObject rootObj = jsonDoc.object();
     QJsonValue foldepath = rootObj.value("imageinitpath");
      QJsonValue resultimagepath = rootObj.value("resultimagepath");
     QString folderstring=QCoreApplication::applicationDirPath()+foldepath.toString();
      QString resultstring=QCoreApplication::applicationDirPath()+resultimagepath.toString();

     QDir dir(folderstring);
     QDir dir2(resultstring);
     if(!dir2.exists()){
         dir2.mkdir(resultstring);
     }

    if(dir.exists()){
        //设置过滤器
           QStringList filters;
           filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.png")<<QString("*.tif")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp");
           dir.setFilter (QDir::Files | QDir::NoSymLinks); // 设置类型过滤器，只为文件格式
           dir.setNameFilters (filters);  // 设置文件名称过滤器，只为 filters 格式（后缀为.jpeg 等图片格式）
           int dir_count = dir.count();
           if(dir_count<=0){
               return ;
           }else{
               QChar separator = QChar('/');
               if(!folderstring.contains(separator))
               {
                   separator = QChar('\\');
               }
               QChar last_char =folderstring.at(folderstring.length()-1);
               if(last_char == separator)
               {
                   separator = QChar();
               }

               QString newfilepath=resultstring+separator+QString::number(QDateTime::currentMSecsSinceEpoch())+separator;
               QChar h0 = 0x00;
               if ( newfilepath.contains(h0))
               {
                    newfilepath=newfilepath.replace(h0, "");
               }
               QDir temp(newfilepath);
               if(!temp.exists()){
                  temp.mkdir(newfilepath);
               }
               for(int i=0; i<dir_count; i++)
               {
                   QString file_name = dir [i];  // 文件名称
                   QString file_path = folderstring + separator + file_name;   // 文件全路径

                   if ( file_path.contains(h0))
                   {
                        file_path=file_path.replace(h0, "");
                   }
                   QString qnewfilepath=newfilepath+"/"+file_name;
                           if ( qnewfilepath.contains(h0))
                           {
                                qnewfilepath=qnewfilepath.replace(h0, "");
                           }

                   QFile::copy(file_path,qnewfilepath);
                   QFile::remove(file_path);
               }
           }
    }
}
void scanner::checkscannfinsh(){
         QFile qfile(QCoreApplication::applicationDirPath()+"/KST-903/KSTSMStart.yy");
         if(!qfile.exists()){
             QMessageBox *msgBox=new QMessageBox(QMessageBox::Information, tr("Scan Success"), "", QMessageBox::Ok|QMessageBox::Cancel, this);
                msgBox->setText("Scan "+QString::number(readfloadimage())+" page");
                QFont qf;
                qf.setPixelSize(20);
                msgBox->setFont(qf);
                msgBox->setStyleSheet(""
                                      "QPushButton {"
                                                      "background-color:#89AFDE;"
                                                      " border-style: outset;"
                                                      " border-width: 2px;"
                                                      " border-radius: 10px;"
                                                      " border-color: beige;"
                                                      " font: bold 24px;"
                                                      " min-width: 5em;"
                                                      " min-height: 2em;"
                                                      "}");
                  msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
                  if (msgBox->exec() == QMessageBox::Ok){
                      QString str="cp -r "+QCoreApplication::applicationDirPath()+"/KST-903/FileContent/Default/* "+ QCoreApplication::applicationDirPath()+"/KST-903/FileContent/Default2/";
                     const char* commond=str.toLocal8Bit().constData();
                      system(commond);
                      QString str2="rm -rf "+QCoreApplication::applicationDirPath()+"/KST-903/FileContent/Default/*";
                      const char* commond2=str2.toLocal8Bit().constData();
                      system(commond2);
                      timer4->stop();

                    }else{
                      QString str="rm -rf "+QCoreApplication::applicationDirPath()+"/KST-903/FileContent/Default/*";
                      const char* commond=str.toLocal8Bit().constData();
                      system(commond);
                      timer4->stop();
                  }
         }

}
void scanner::loadLanguage(QString type)
{
//m_translator=new QTranslator();
//    QString langqmfilepath;
//    if(ui->comboBox->currentText()=="English")
//    {
//        langqmfilepath = qApp->applicationDirPath() + QString("/language/SacnnerPage_en_en_AS.qm");
//    }
//    else if(ui->comboBox->currentText()=="简体中文")
//    {
//        langqmfilepath = qApp->applicationDirPath() + QString("/language/ScnnerPage_zh_cn_zh_CN.qm");
//    }

//    if(QFile(langqmfilepath).exists())
//    {
//    m_translator->load(langqmfilepath);
//    qApp->installTranslator(m_translator);
//    }
//    else
//        qDebug()<<"[houqd] authclient language file does not exists ...";

//    //重新刷新界面
//    ui->retranslateUi(this);

}

int scanner:: System_Check(int result)
{
    if((-1 != result) && (WIFEXITED(result)) && (!(WEXITSTATUS(result))))

        return 0;
    else

        return -1;
}
