#include "folderwidget.h"
#include "ui_folderwidget.h"

FolderWidget::FolderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FolderWidget)
{
    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
       this->screenwidth=screenRect.width();
        this->screenheight=screenRect.height();

        ui->setupUi(this);

    initFolderWidget();

}

FolderWidget::~FolderWidget()
{
    delete ui;
}
//目录页初始化
void FolderWidget::initFolderWidget()
{

       this->setWindowTitle(tr("Directory Management"));
    //读取文件夹
    ui->horizontalWidget->resize(this->screenwidth,this->screenheight);
   QFont qf;
   qf.setPointSize(30);




   //
   //读取到ini位置
    QString inipath=QCoreApplication::applicationDirPath()+readjsonreturnpath("inipath");
   QSettings *settings;//申明一个QSetting类函数
   settings = new QSettings (inipath, QSettings::IniFormat);//构建函数


   QString iniupdateitem=readjsonreturnpath("iniupdateitem2");

   QString foldertitle=settings->value(iniupdateitem).toString();
   QStringList qlist2=foldertitle.split("/");
   QString foldername2=qlist2.at(qlist2.size()-1);
   foldertitle=QCoreApplication::applicationDirPath()+readjsonreturnpath("parentpath")+foldername2;

//    //一个图片预览列表
   QStringList buttonstring={tr("Pre-P"),tr("Next-P"),tr("WM"),tr("BE"),tr("ALL"),tr("Cal"),tr("Exp"),tr("del"),tr("clear"),tr("ref"),tr("close")};
   //

   QStringList buttonicon={
       QCoreApplication::applicationDirPath()+"/icon/shanyiye.png",
       QCoreApplication::applicationDirPath()+"/icon/xiayiye.png",
       QCoreApplication::applicationDirPath()+"/icon/shuiyin2.png",
       QCoreApplication::applicationDirPath()+"/icon/yebianju2.png",
       QCoreApplication::applicationDirPath()+"/icon/xuanze2.png",
        QCoreApplication::applicationDirPath()+"/icon/quxiao.png",
       QCoreApplication::applicationDirPath()+"/icon/daochu2.png",
        QCoreApplication::applicationDirPath()+"/icon/deleter2.png",
       QCoreApplication::applicationDirPath()+"/icon/deleter.png",
  QCoreApplication::applicationDirPath()+"/icon/huanyuan",
       QCoreApplication::applicationDirPath()+"/icon/tuichu3.png"
   };
   label=new Mylabel();
   qf.setPixelSize(15);
   label->setMaximumHeight(15);
   QString choosetext="Page:"+QString::number(page)+QString("/")+QString::number(allpage)+" "
           +"selected:"+QString::number(choose)+"/"+QString::number(allimage)+"";

   label->setText(choosetext);
   label->setStyleSheet("margin-left:20px;color:rgba(255,255,255,1); margin-top:-30px;");
   label->setFont(qf);
   ui->horizontalLayout_4->setMargin(-20);
   ui->horizontalLayout_4->addWidget(label);
   filename=new Mylabel();
   filename->setText(" ");
   filename->setFont(qf);
   filename->setMinimumWidth(400);
   filename->setMaximumWidth(15);
   filename->setStyleSheet("margin-left:-120px;color:rgba(255,255,255,1); margin-top:-30px;");
   ui->horizontalLayout_4->addWidget(filename);
   shengchengbutton(buttonstring,buttonicon);


   trw=new TranslationWidget(qlist);
   initwidget(trw,this->screenwidth-100,this->screenwidth-100,this->screenheight-100,this->screenheight-100,"background-color:rgba(255,255,255,0.3);border-radius:20px;color:rgba(255,255,255,1)");
   ui->horizontalLayout_2->addWidget(trw);





    //设置背景图片
    QPixmap pixmap2 = QPixmap(QCoreApplication::applicationDirPath()+"/image/bg.jpg").scaled(this->screenwidth,this->screenheight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    QPalette pal=this->palette();
    pal.setBrush(QPalette::Background,QBrush(pixmap2));
    this->setAutoFillBackground(true);
    setPalette(pal);
    initfolder();
     readImageFile();
}

void FolderWidget::initfolder()
{
    QString inipath=QCoreApplication::applicationDirPath()+readjsonreturnpath("inipath");
    QSettings *settings;//申明一个QSetting类函数
    settings = new QSettings (inipath, QSettings::IniFormat);//构建函数


    QString iniupdateitem=readjsonreturnpath("iniupdateitem2");

    QString foldertitle=settings->value(iniupdateitem).toString();
    QStringList qlist2=foldertitle.split("/");
    QString foldername2=qlist2.at(qlist2.size()-1);
    foldertitle=QCoreApplication::applicationDirPath()+readjsonreturnpath("parentpath")+foldername2;
    folderpath=foldertitle;
    qDebug()<<folderpath;
    if(folderpath==nullptr){

        oldfolderpath=folderpath;

        readImageFile();
    }
}
//读取文件夹
void FolderWidget::readfolder()
{
        string_list.clear();
        curreindex=0;
        QFileDialog *fileDialog = new QFileDialog(this);
        Qt::WindowFlags m_flags=fileDialog->windowFlags();
          fileDialog ->setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
        fileDialog->setFileMode(QFileDialog::Directory);
        fileDialog->exec();
        auto selectDir = fileDialog->selectedFiles();
        if (selectDir.size()>0)
        {
            qDebug() << "Dir Path:" << selectDir.at(0);
            folderpath=selectDir.at(0);
            ui->horizontalLayout_2->removeWidget(trw);
            readImageFile();

        }


}


void FolderWidget::readImageFile()
{
         QDir dir(folderpath);
        if(dir.exists()){
            //设置过滤器
               QStringList filters;
               filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.png")<<QString("*.tiff")<<QString("*.tif")<<QString("*.gif")<<QString("*.bmp");
               dir.setFilter (QDir::Files | QDir::NoSymLinks); // 设置类型过滤器，只为文件格式
               dir.setNameFilters (filters);  // 设置文件名称过滤器，只为 filters 格式（后缀为.jpeg 等图片格式）
               uint dir_count = dir.count();
               if(dir_count<=0){

                    qDebug()<<"执行";
                    qlist.clear();
                    delete(trw);
                    trw=new TranslationWidget(qlist);
                    trw->setMoveEnabled(false);
                    initwidget(trw,this->screenwidth-100,this->screenwidth-100,this->screenheight-100,this->screenheight-100,"background-color:rgba(255,255,255,0.3);border-radius:20px;color:rgba(255,255,255,1)");
                    ui->horizontalLayout_2->addWidget(trw);
                   return;
               }
                  // 获取分隔符
                  //QChar separator = QDir::separator();
                  QChar separator = QChar('/');
                  if(!folderpath.contains(separator))
                  {
                      separator = QChar('\\');
                  }
                  QChar last_char =folderpath.at(folderpath.length()-1);
                  if(last_char == separator)
                  {
                      separator = QChar();
                  }

                  for(int i=string_list.size(); i<dir_count; i++)
                  {


                      QString file_name = dir [i];  // 文件名称
                      QString file_path = folderpath + separator + file_name;   // 文件全路径
                      string_list.append(file_path);
                  }

                    delete(trw);
                    qlist.clear();
                    if(string_list.size()>0){
                         //string_list.size()%9>=1&&string_list.size()/9>0?(string_list.size()/9)+1:
                          pagesize=string_list.size()%15!=0?string_list.size()/15+1:string_list.size()/15;

                          QListWidget *qlw;
//
                           for(int i=0;i<=pagesize;i++){
                               qlw=new QListWidget();
                               qlw->setDragEnabled(false);
                               qlw->setAcceptDrops(false);

                               qlw->setMovement(QListView::Static);
                               qlw->setViewMode(QListView::IconMode);
                               qlw->setIconSize(QSize(120,120));
                               qlw->setStyleSheet("border-radius:20px");
                               qlw->setSpacing(30);
                               initwidget(qlw,this->screenwidth-100,this->screenwidth-100,this->screenheight-100,this->screenheight-100,"background-color:rgba(255,255,255,0.3);border-radius:20px;color:rgba(255,255,255,1); ");
                               qlist.append(qlw);
                           }
                           trw=new TranslationWidget(qlist);
                           trw->setMoveEnabled(false);
                           initwidget(trw,this->screenwidth-100,this->screenwidth-100,this->screenheight-140,this->screenheight-140,"background-color:rgba(255,255,255,0.3);border-radius:20px;color:rgba(255,255,255,1)");
                           ui->horizontalLayout_2->addWidget(trw);

                           for(int i=currentpage*15;i<currentpage*15+15&&i<string_list.size();i++){


                                    temlistwidget=(QListWidget*)qlist.at(0);
                                      QIcon icon(string_list.at(i));
                                      QListWidgetItem *imageitem=new QListWidgetItem();
                                      QString name=string_list.at(i);
                                      QStringList qsl=name.split("/");
                                      folderpath2=name.replace(qsl.at(qsl.size()-1),"");
                                      imageitem->setText(qsl.at(qsl.size()-1));
                                     imageitem->setIcon(icon);
                                     imageitem->setSizeHint(QSize(140,125));
                                     temlistwidget->addItem(imageitem);
                                     temlistwidget->repaint();
                            }

                           connect(temlistwidget,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(clickImage(QListWidgetItem *)));



                    }
                    allimage=string_list.size();
                    choose=choose_list.size();
                    QString choosetext=tr("page:")+QString::number(page+1)+QString("/")+QString::number(pagesize)+" "
                            +tr("selected:")+QString::number(choose)+"/"+QString::number(allimage)+"";

                    label->setText(choosetext);

        }else{
            qDebug()<<"文件不存在"<<endl;
        }



}
//加载图片
void FolderWidget::loadingimage(QScrollBar *qsb)
{
        readImageFile();
}
void FolderWidget::initwidget(QWidget *q,int minwidth,int maxwidth,int minheight,int maxheight,QString style)
{
    q->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    q->setMinimumWidth(minwidth);
    q->setMinimumHeight(minheight);
    q->setMaximumWidth(maxwidth);
    q->setMaximumHeight(maxheight);
    q->setStyleSheet(style);
}
void FolderWidget::intilabel(QWidget *q,
                             Mylabel *label,
                             QString path,
                             int imagewith,
                             int imageheight, int maginw, int maginh, int minwidth, int maxwidth, int minheight, int maxheight)
{
//qDebug()<<path;
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
//    label->setText(path);


}

void FolderWidget::changeUpPage()
{
     if(string_list.size()>0){

    qDebug()<<"上一页"+QString::number(pagesize);

    if(FolderWidget::page!=0){
        FolderWidget::page-=1;
        currentpage=page;
        qDebug()<<currentpage<<endl;
        int size=(qlist.at(currentpage)->findChildren<QListWidget*>()).size();
        if(size==0){
            readImageFile();

        }
        changeChooseColor();
    }
  buttons.at(0)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/shanyiye.png"));
     }else{

               buttons.at(0)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/shanyiye2.png"));


//         QMessageBox msgBox(QMessageBox::Warning, tr("该目录内容为空"), tr("该目录内容为空,请重新选择目录"), QMessageBox::Ok, this);
//         msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
//         int result = msgBox.exec();
     }

}

void FolderWidget::changeNextPage()
{
    if(string_list.size()>0){

    qDebug()<<"下一页"+QString::number(pagesize);
    qDebug()<<FolderWidget::page;
     if(FolderWidget::page<pagesize-1){

            FolderWidget::page+=1;
            currentpage=page;
            int size=(qlist.at(currentpage)->findChildren<QListWidget*>()).size();
            if(size==0){
                readImageFile();

            }

        changeChooseColor();
     }
      buttons.at(1)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/xiayiye.png"));

    }else{
        buttons.at(1)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/xiayiye2.png"));
//        QMessageBox msgBox(QMessageBox::Warning, tr("该目录内容为空"), tr("该目录内容为空,请重新选择目录"), QMessageBox::Ok, this);
//        msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
//        int result = msgBox.exec();
    }

}

void FolderWidget::addwaterimage()
{
    if(choose_list.size()>0){
    buttons.at(2)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/shuiyin.png"));
    QDialog dialog(this);
    Qt::WindowFlags m_flags=dialog.windowFlags();
      dialog.setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
    dialog.setWindowFlags(Qt::FramelessWindowHint);
    dialog.setStyleSheet("background-color:rgba(250,250,250,1);border-radius:20px;padding:15px;");

    dialog.resize(QSize(500,200));

    QFormLayout form(&dialog);

    // Value1
    QString value1 = QString(tr("Watermark content:"));
    QLineEdit *spinbox1=new QLineEdit(&dialog);
    spinbox1->setMinimumHeight(10);
    QFont qf;
    qf.setPixelSize(20);
    spinbox1->setFont(qf);
    spinbox1->setStyleSheet("border:1px solid rgba(0,0,0,0.5);");
    form.addRow(value1, spinbox1);
    //选择保存水印文件夹
//    QString value2=QString("位置");
//    qpbopenfload=new QPushButton();

    QString waterpath=QCoreApplication::applicationDirPath()+readjsonreturnpath("waterinitImapgepath");
    QDateTime time=QDateTime::currentDateTime();
    QString str = time .toString("yyyy_MM_dd_hh_mm_ss");
    QString folderpath=waterpath+"watermark_"+str;
     QDir qdir(folderpath);
    if(!qdir.exists()){
        qdir.mkdir(folderpath);
    }
     waterfolderpath=folderpath;

//      qpbopenfload->setText(waterfolderpath);
//    form.addRow(value2, qpbopenfload);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
            Qt::Horizontal, &dialog);

        buttonBox.button(QDialogButtonBox::Ok)->setText(tr("OK"));
         buttonBox.button(QDialogButtonBox::Ok)->setStyleSheet("background-color:#91C8E4;color:rgba(255,255,255,1);margin-right:100px;");

         buttonBox.button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
           buttonBox.button(QDialogButtonBox::Cancel)->setStyleSheet("background-color:rgba(0,0,0,0.8);color:rgba(255,255,255,1);margin-right:100px;");

    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {

        QDateTime time=QDateTime::currentDateTime();

        uint time_t=time.toTime_t();
        waterfolderpath=waterfolderpath+"/"+QString::number(time_t);
        QDir dir(waterfolderpath);
        if(!dir.exists()){
            dir.mkdir(waterfolderpath);
        }
        for(QString choseimage:choose_list){
             watermarkImage(spinbox1->text(),choseimage);
        }
        suaxing();
        bool re=QFile::remove(waterpathtemp);
        Qt::WindowFlags m_flags =this->windowFlags();
    //     Qt::WindowStaysOnTopHint
          this->setWindowFlags(m_flags |Qt::WindowStaysOnTopHint);

    }

  }else{
        buttons.at(2)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/shuiyin2.png"));
//        QMessageBox msgBox(QMessageBox::Warning, tr("请选择要添加的水印"), tr("请选择要添加的水印选项"), QMessageBox::Ok, this);
//        msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
//        int result = msgBox.exec();
    }
}
void FolderWidget::changeChooseColor(){
    int i=temlistwidget->count();
    int pagesize=choose_list.size()/15;

    for(int k=0;k<pagesize&&page!=pagesize;k++){

            for(int o=0;o<15;o++){
                if(choose_list.size()>k*15+o){
                QStringList filenames=choose_list.at(k*15+o).split("/");
                QString filename=filenames.at(filenames.size()-1);
                QListWidgetItem *tempqlwi=temlistwidget->item(o);
                if(filename==tempqlwi->text()){
                    tempqlwi->setTextColor("#C23373");
                   tempqlwi->setBackgroundColor("#8CC0DE");
                }
            }
            }
     }

    if(page==pagesize){
        for(int p=0;p<choose_list.size()%15; p++){
            QListWidgetItem *tempqlwi=temlistwidget->item(p);
                tempqlwi->setTextColor("#C23373");
               tempqlwi->setBackgroundColor("#8CC0DE");
            }
        }

}













void FolderWidget::clickImage(QListWidgetItem *item)
{

      int index=choose_list.indexOf(folderpath2+item->text());
      if(index!=-1){
          item->setTextColor("#EEEDED");
           item->setBackgroundColor("transparent");
          choose_list.removeAt(index);
          if(choose_list.size()==0){
                buttons.at(2)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/shuiyin2.png"));
                buttons.at(3)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/yebianju2.png"));
               buttons.at(6)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/daochu2.png"));
                 buttons.at(7)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/deleter2.png"));
          }
      }else{

           item->setTextColor("#C23373");
           item->setBackgroundColor("#8CC0DE");
          choose_list.append(folderpath2+item->text());
            buttons.at(6)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/daochu.png"));
            buttons.at(2)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/shuiyin.png"));
            buttons.at(3)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/yebianju.png"));
             buttons.at(7)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/deleter.png"));

      }
      allimage=string_list.size();
      choose=choose_list.size();
      int pagesize=string_list.size()%15>0?(string_list.size()/15)+1:string_list.size()/15;
      QString choosetext=tr("page:")+QString::number(page+1)+QString("/")+QString::number(pagesize)+" "
              +tr("selected:")+QString::number(choose)+"/"+QString::number(allimage)+" ";
      label->setText(choosetext);
      filename->setText(item->text());
      filename->setMaximumHeight(15);
//    qDebug()<<item->text();
}

void FolderWidget::clickImage(QListWidgetItem *item,int allchoose)
{

      int index=choose_list.indexOf(folderpath2+item->text());
      if(allchoose==1){
         if(index=-1){
              buttons.at(5)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/quxiao.png"));
             buttons.at(6)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/daochu.png"));
             buttons.at(5)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/quxiao.png"));
             buttons.at(2)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/shuiyin.png"));
             buttons.at(3)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/yebianju.png"));
              buttons.at(7)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/deleter.png"));
             item->setTextColor("#C23373");
             item->setBackgroundColor("#8CC0DE");
             choose_list.append(folderpath2+item->text());
         }
      }else if(allchoose==2){
           if(index!=-1){
               item->setTextColor("#EEEDED");
                item->setBackgroundColor("transparent");
              choose_list.removeAt(index);
              if(choose_list.size()==0){
                   buttons.at(5)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/quxiao2.png"));
                    buttons.at(2)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/shuiyin2.png"));
                    buttons.at(3)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/yebianju2.png"));
                   buttons.at(6)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/daochu2.png"));
                     buttons.at(7)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/deleter2.png"));
              }
           }

      }


      allimage=string_list.size();
      choose=choose_list.size();
      int pagesize=string_list.size()%15>0?(string_list.size()/15)+1:string_list.size()/15;
      qDebug()<<pagesize;
      QString choosetext=tr("page:")+QString::number(page+1)+QString("/")+QString::number(pagesize)+" "
              +tr("selected:")+QString::number(choose)+"/"+QString::number(allimage)+" ";
      label->setText(choosetext);

//    qDebug()<<item->text();
}

void FolderWidget::on_pushButton_clicked()
{
      trw->setCurrentPage(FolderWidget::page,true);
}
void FolderWidget::watermarkImage(QString watercontent,QString imagepath)
{
    if(watercontent==""){
        QMessageBox msgBox(QMessageBox::Warning, tr("Watermark content cannot be empty"), tr("Watermark content cannot be empty"), QMessageBox::Ok, this);
        msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
        int result = msgBox.exec();
    }else{
     //保存水印位置
     QStringList temp=imagepath.split("/");
     QFile qfile(imagepath);
     //保存原图
     QString foldername=QCoreApplication::applicationDirPath()+readjsonreturnpath("yuanpath")+"/"+temp.at(temp.size()-1);
     qfile.copy(imagepath,foldername);
    //chang the image path type form QString to std:string
     QByteArray tempstr=imagepath.toLocal8Bit();
     std::string waterimagepathstd=std::string(tempstr);
     //chang the image path type form QString to std:string
     QByteArray tempstr2=imagepath.toLocal8Bit();
      std::string imagepathstd=std::string(tempstr2);
      // ready the image that it be processed.
     cv::Mat mat=cv::imread(imagepathstd);
     // get this image width and height
     int cols=mat.cols;
     int rows=mat.rows;
     //create a new image ,it size same the old image that be processed.
     QImage image(cols,rows,QImage::Format_ARGB32);
     // fill an transparent color
      image.fill(Qt::transparent);
      //create a painter ,we need use it to create a watermark
     QPainter painter(&image); //为这个QImage构造一个QPainter
     painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
     QPen pen = painter.pen();
    //setting the pen color
     pen.setColor(qRgba(105,105,105,1));
     // set the pen font size
     QFont font = painter.font();
     font.setBold(true);//加粗
     font.setPixelSize(rows/4);//改变字体大小
     painter.setPen(pen);
     painter.setFont(font);
     // the watermark position
     painter.drawText(image.rect(),Qt::AlignCenter,watercontent);
     //创建水印
       //生成文件名称：输入的文件名+时间戳+文件后缀
     QDateTime time=QDateTime::currentDateTime();
     uint time_t=time.toTime_t();
     //读取json
       QString waterpathtemp=QCoreApplication::applicationDirPath()+readjsonreturnpath("watertemppath")+"/"+watercontent+QString::number(time_t)+".png";
      //保存水印图片
     bool istrue=image.save(waterpathtemp);
     if(istrue){
         //将两个图片进行合并然后替换原来图片，展示图片
         //保存添加水印前的原图
         QString waterimagepath=waterpathtemp;
         QByteArray tempstr=waterimagepath.toLocal8Bit();
         std::string watermarkpath=std::string(tempstr);
         cv::Mat watermat=cv::imread(watermarkpath);
         cv::Mat result;
         cv::addWeighted(watermat,0.5,mat,0.85,0,result);
         qDebug()<<"图像融合";
         cv::imwrite(waterimagepathstd,result);

//         QMessageBox::information(this,tr("水印添加成功"),temp2);
//         imsw->doLoadImage(imagepath);

         bool re=QFile::remove(waterpathtemp);
     }else{

           QMessageBox msgBox(QMessageBox::Warning, tr("The watermark storage path does not exist"), tr("The watermark storage path does not exist"), QMessageBox::Ok, this);
           msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
           int result = msgBox.exec();
     }
    }







}


void FolderWidget::closefloader()
{
    QString inipath=QCoreApplication::applicationDirPath()+readjsonreturnpath("inipath");

//    //读取到ini位置
    QSettings *settings;//申明一个QSetting类函数

    settings = new QSettings (inipath, QSettings::IniFormat);//构建函数


    QString iniupdateitem=readjsonreturnpath("iniupdateitem2");
//    //修改原始文件所在位置
//    folderpath=folderpath+"/";
    folderpath=settings->value(iniupdateitem).toString();
    QStringList qlist=folderpath.split("/");
    QString foldername=qlist.at(qlist.size()-1);
    folderpath=QCoreApplication::applicationDirPath()+readjsonreturnpath("parentpath")+foldername;
    QDir dir(folderpath);
    if(dir.exists()){
        //设置过滤器
           QStringList filters;
           filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.tif")<<QString("*.png")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp");
          dir.setFilter (QDir::Files | QDir::NoSymLinks); // 设置类型过滤器，只为文件格式
           dir.setNameFilters (filters);  // 设置文件名称过滤器，只为 filters 格式（后缀为.jpeg 等图片格式）
           int dir_count = dir.count();
            if(dir_count==0){
                this->close();
            }else{
                if(string_list.size()>0){
                    quxiaoxuanze2();
                }
            }
    }
this->close();


}
//去黑边
void FolderWidget::quheibian()
{
 if(choose_list.size()>0){
        buttons.at(3)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/yebianju.png"));
     //选择裁剪的模板，然后进行批量裁剪
       QString imagepath=choose_list.at(0);
       QDialog dialog(this);
       Qt::WindowFlags m_flags=dialog.windowFlags();
         dialog.setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
       dialog.setWindowFlags(Qt::FramelessWindowHint);
       dialog.setStyleSheet("background-color:rgba(250,250,250,1);border-radius:20px;padding:15px;");
       QVBoxLayout qvl(&dialog);
       quheibianbutton=new QPushButton();
//       quheibianbutton->setText("选择去黑边保存的路径");
//       quheibianbutton->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/wenjianjia1.png"));
//       quheibianbutton->setIconSize(QSize(20,20));
//       quheibianbutton->setStyleSheet("color:rgba(255,0,0,1)");
       QString heibianpath=QCoreApplication::applicationDirPath()+readjsonreturnpath("quheibianimagepath");
       QDateTime time=QDateTime::currentDateTime();
      QString str = time .toString("yyyy_MM_dd_hh_mm_ss");
       QString folderpath=heibianpath+"quheibian_"+str;

        quheibianfolderpath=folderpath;
        QFont qf;
        qf.setPixelSize(15);
        quheibianbutton->setFont(qf);
        quheibianbutton->setText(tr("Remove the black edge"));

//       quheibianbutton->setText(quheibianfolderpath);
//       connect(quheibianbutton,&QPushButton::clicked,this,[=]{
//           openfolder3();
//       });
       qvl.addWidget(quheibianbutton);



       QByteArray tempstr2=imagepath.toLocal8Bit();
        std::string imagepathstd=std::string(tempstr2);
        cv::Mat ori_image=cv::imread(imagepathstd);
        int width=ori_image.cols<this->screenwidth-170?ori_image.cols:ori_image.cols*0.25;
            int height=ori_image.rows<this->screenheight?ori_image.rows:ori_image.rows*0.25;
              cs=new capturescreen(this,imagepath,width,height,ori_image.cols,ori_image.rows);
              cs->resize(width,height);
               dialog.resize(QSize(ori_image.cols*0.25+20,ori_image.rows*0.25+150));
       qvl.addWidget(cs);
       // Add Cancel and OK button
       QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
               Qt::Horizontal, &dialog);
           buttonBox.button(QDialogButtonBox::Ok)->setText(tr("OK"));
                buttonBox.button(QDialogButtonBox::Ok)->setFont(qf);
            buttonBox.button(QDialogButtonBox::Ok)->setStyleSheet("background-color:#91C8E4;color:rgba(255,255,255,1);margin-right:100px;");

            buttonBox.button(QDialogButtonBox::Cancel)->setText(tr("cancel"));
              buttonBox.button(QDialogButtonBox::Cancel)->setFont(qf);
              buttonBox.button(QDialogButtonBox::Cancel)->setStyleSheet("background-color:rgba(0,0,0,0.8);color:rgba(255,255,255,1);margin-right:100px;");
       qvl.addWidget(&buttonBox);

       QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
       QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
       // Process when OK button is clicked
       if (dialog.exec() == QDialog::Accepted) {
           QDir qdir(folderpath);
          if(!qdir.exists()){
              qdir.mkdir(folderpath);
          }
           if(quheibianfolderpath==""||quheibianfolderpath==nullptr){
//                QString temp=tr("有一个错误","请选择去黑边保存的路径");
//               QMessageBox::warning(this,temp);
           }else{
               //保存去黑边的原始图
    //           QString quheibianiitpath=readjsonreturnpath("quheibianimagepath");
               QStringList tempqsl=imagepath.split("/");
               QFile qfile(imagepath);
               QString tempimagename=tempqsl.at(tempqsl.size()-1);
               QString quheibiansavepaths=quheibianfolderpath+"/"+tempimagename;
               std::string quhebiansavestd=quheibiansavepaths.toStdString();
               cv::imwrite(quhebiansavestd,ori_image);
               //截图后保存
               cs->onSaveTemplateImage(imagepath);
               //模板路径

              //循环choose_list 第二项根据模板进行裁剪
               QByteArray tempstr2=quheibiansavepaths.toLocal8Bit();
                std::string quheibiansavepathstd=std::string(tempstr2);
               cv::Mat mat=cv::imread(quheibiansavepathstd);


               //获取时间戳
//               QDateTime time=QDateTime::currentDateTime();
//               uint time_t=time.toTime_t();
//               //拼接文件夹
//               QString folderpath= quheibianfolderpath+"/"+QString::number(time_t)+"/";
//               QDir dir(folderpath);
//               //文件夹不存在创建文件夹
//               if(!dir.exists()){
//                   dir.mkdir(folderpath);
//               }

//               QFile::copy(quheibiansavepaths,imagepath);
               //将文件夹转std字符串
    //           std::string quheibainitpathstd=quheibianiitpath.toStdString();
               for(int k=0;k<choose_list.size();k++){
                   //读取原始图片
                   QByteArray tempstr0=choose_list.at(k).toLocal8Bit();
                   std::string tempathstd0=std::string(tempstr0);
                   cv::Mat mat2=cv::imread(tempathstd0);
                   QStringList temp=choose_list.at(k).split("/");
                   QString foldername=QCoreApplication::applicationDirPath()+readjsonreturnpath("yuanpath")+"/"+temp.at(temp.size()-1);
                   qfile.copy(choose_list.at(k),foldername);
                   //两个图片比较删除循环内部这张图片多余的部分
                   //保存原始图片在quheibianinit里
                   QStringList tem=choose_list.at(k).split("/");
                   QString tempath=folderpath+tem.at(tem.size()-1);
                   QByteArray tempstr=tempath.toLocal8Bit();
                   std::string tempathstd=std::string(tempstr);
                   cv::imwrite(tempathstd,mat2);
                   //图像裁剪
                   if(mat.cols<mat2.cols&&mat.rows<mat2.rows){

                    cv::Rect rect=cv::Rect(0,0,mat.cols,mat.rows);
                    cv::Mat result=mat2(rect);
                     //删除原始图片
                    QFile::remove(choose_list.at(k));
                    //保存裁剪图片
                    QByteArray tempstr=choose_list.at(k).toLocal8Bit();
                    std::string tempathstd=std::string(tempstr);
                    cv::imwrite(tempathstd,result);
                   }else{
                       continue;
                   }


               }
               QFile::remove(quheibiansavepaths);
               readImageFile();
                cs->deleteLater();
//                QString temp2="请打开文件夹"+ folderpath+"查看";
//                QMessageBox msgBox(QMessageBox::Information, tr("去除黑边成功"), "去除黑边成功", QMessageBox::Ok, this);
//                msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
//                int result = msgBox.exec();
                Qt::WindowFlags m_flags =this->windowFlags();
            //     Qt::WindowStaysOnTopHint
                  this->setWindowFlags(m_flags |Qt::WindowStaysOnTopHint);
            }

       }else{


       }


     }else{
       buttons.at(3)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/yebianju2.png"));
//      QMessageBox msgBox(QMessageBox::Warning, tr("请选择要去除黑边的选项"), tr("请选择要去除黑边的选项"), QMessageBox::Ok, this);
//      msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
//      int result = msgBox.exec();
 }
}
//全选
void FolderWidget::quanxuan()
{
    choose_list.clear();
    buttons.at(6)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/daochu.png"));
    buttons.at(2)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/shuiyin.png"));
    buttons.at(3)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/yebianju.png"));
     buttons.at(7)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/deleter.png"));
      buttons.at(5)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/quxiao.png"));
     for(int i=0;i<string_list.size();i++){
         choose_list.append(string_list.at(i));
     }
     QString choosetext=tr("page:")+QString::number(page+1)+QString("/")+QString::number(pagesize)+" "
             +tr("selected:")+QString::number(choose_list.size())+"/"+QString::number(allimage)+" ";
     label->setText(choosetext);
     changeChooseColor();


}
//取消全选
void FolderWidget::quxiaoxuanze()
{
    if(choose_list.size()>0){
        choose_list.clear();
        int i=temlistwidget->count();
        for(int j=0;j<i;j++){
            QListWidgetItem *tempqlwi=temlistwidget->item(j);
            clickImage(tempqlwi,2);
             tempqlwi->setTextColor("#EEEDED");
             tempqlwi->setBackgroundColor("transparent");

          }
        buttons.at(5)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/quxiao2.png"));
        buttons.at(6)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/daochu2.png"));
        buttons.at(2)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/shuiyin2.png"));
        buttons.at(3)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/yebianju2.png"));
         buttons.at(7)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/deleter2.png"));
    }else{
          buttons.at(5)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/quxiao2.png"));
          buttons.at(6)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/daochu2.png"));
          buttons.at(2)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/shuiyin2.png"));
          buttons.at(3)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/yebianju2.png"));
           buttons.at(7)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/deleter2.png"));
//        QMessageBox msgBox(QMessageBox::Warning, tr("Please select a directory first"), tr("Please select a directory first"), QMessageBox::Ok, this);
//        msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
//        int result = msgBox.exec();
    }
}
void FolderWidget::quxiaoxuanze2()
{
        choose_list.clear();
        if(choose_list.size()==0){

             buttons.at(5)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/quxiao2.png"));
             buttons.at(6)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/daochu2.png"));
             buttons.at(2)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/shuiyin2.png"));
             buttons.at(3)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/yebianju2.png"));
              buttons.at(7)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/deleter2.png"));
        }
        int i=temlistwidget->count();
        for(int j=0;j<i;j++){
            QListWidgetItem *tempqlwi=temlistwidget->item(j);
            filename->setText("");
            clickImage(tempqlwi,2);
             tempqlwi->setTextColor("#EEEDED");
             tempqlwi->setBackgroundColor("transparent");

          }
}
//删除
void FolderWidget::deleteimage()
{
    if(choose_list.size()>0){
        QDialog dialog(this);
        dialog.setWindowFlags(Qt::FramelessWindowHint);
        dialog.setStyleSheet("background-color:rgba(250,250,250,1);border-radius:5px;padding:15px;");
        dialog.resize(QSize(500,200));
        Mylabel *mylabel=new Mylabel();
        QFont qf;
        qf.setPixelSize(20);
        mylabel->setFont(qf);
        mylabel->setText("Confirm to delete the selected picture?");
        QFormLayout form(&dialog);
         QString value1 = QString("");
         form.addRow(value1,mylabel);
         QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                 Qt::Horizontal, &dialog);

             buttonBox.button(QDialogButtonBox::Ok)->setText(tr("OK"));
              buttonBox.button(QDialogButtonBox::Ok)->setFont(qf);
              buttonBox.button(QDialogButtonBox::Ok)->setStyleSheet("background-color:#91C8E4;color:rgba(255,255,255,1);margin-right:100px;");

              buttonBox.button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
               buttonBox.button(QDialogButtonBox::Cancel)->setFont(qf);
                buttonBox.button(QDialogButtonBox::Cancel)->setStyleSheet("background-color:rgba(0,0,0,0.8);color:rgba(255,255,255,1);margin-right:100px;");

         form.addRow(&buttonBox);
         QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
         QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

         // Process when OK button is clicked
         if (dialog.exec() == QDialog::Accepted) {
             for(int i=0;i<choose_list.size();i++){

                   QString code="rm -r "+choose_list.at(i);
                    system(code.toStdString().c_str());

                    for(int j=0;j<15&&j<temlistwidget->count();j++){
                             QListWidgetItem *tempqlwi=temlistwidget->item(j);
                             if(folderpath2+tempqlwi->text()==choose_list.at(i)){
                                 temlistwidget->removeItemWidget(tempqlwi);

                             }
                    }
                    temlistwidget->repaint();
             }

            readImageFile();


         }
    }else{
      buttons.at(7)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/deleter2.png"));
    }
}

void FolderWidget::deleteimage2()
{
    if(string_list.size()>0){


    QDialog dialog(this);
    dialog.setWindowFlags(Qt::FramelessWindowHint);
    dialog.setStyleSheet("background-color:rgba(250,250,250,1);border-radius:5px;padding:15px;");
    dialog.resize(QSize(500,100));
    Mylabel *mylabel=new Mylabel();
    mylabel->setText(tr("Confirm deletion")+QString::number(string_list.size())+tr("pictures?"));
    QFormLayout form(&dialog);
     QString value1 = QString("");
     form.addRow(value1,mylabel);
     QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
             Qt::Horizontal, &dialog);

         buttonBox.button(QDialogButtonBox::Ok)->setText(tr("OK"));
          buttonBox.button(QDialogButtonBox::Ok)->setStyleSheet("background-color:#91C8E4;color:rgba(255,255,255,1);margin-right:100px;");

          buttonBox.button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
            buttonBox.button(QDialogButtonBox::Cancel)->setStyleSheet("background-color:rgba(0,0,0,0.8);color:rgba(255,255,255,1);margin-right:100px;");

     form.addRow(&buttonBox);
     QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
     QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

     // Process when OK button is clicked
     if (dialog.exec() == QDialog::Accepted) {
         QString code="rm -r "+QCoreApplication::applicationDirPath()+"/KST-903/FileContent/Default2/*";
         system(code.toStdString().c_str());
         QString code2="echo '0'>"+QCoreApplication::applicationDirPath()+"/KST-903/NowScanCount";
          system(code2.toStdString().c_str());
          readImageFile();


         }
    }
}
//生成按钮
void FolderWidget::shengchengbutton(QStringList buttontitlearrays,QStringList buttoniconarrays)
{
     QPushButton *button=new QPushButton();

    for(int i=0;i<buttontitlearrays.size();i++){
        qDebug()<<i;
        button=new QPushButton();
        QFont qf;
        qf.setPointSize(15);
         button->setText(buttontitlearrays.at(i));
         QIcon icon(buttoniconarrays.at(i));
         button->setIcon(icon);
         button->setFont(qf);
         button->setIconSize(QSize(25,25));
         switch (i) {
            case 0: connect(button,&QPushButton::clicked,this,[=]{changeUpPage();});break;
            case 1:connect(button,&QPushButton::clicked,this,[=]{changeNextPage();});break;
            case 2:connect(button,&QPushButton::clicked,this,[=]{addwaterimage();});break;
            case 3: connect(button,&QPushButton::clicked,this,[=]{quheibian();});break;
            case 4: connect(button,&QPushButton::clicked,this,[=]{quanxuan();});break;
            case 5:   connect(button,&QPushButton::clicked,this,[=]{quxiaoxuanze();});break;
              case 6:   connect(button,&QPushButton::clicked,this,[=]{daochu();});break;
            case 7: connect(button,&QPushButton::clicked,this,[=]{deleteimage();});break;
             case 8: connect(button,&QPushButton::clicked,this,[=]{deleteimage2();});break;
         case 9:connect(button,&QPushButton::clicked,this,[=]{suaxing();});break;
            case 10:  connect(button,&QPushButton::clicked,this,[=]{closefloader();});break;

         }
         button->setMaximumHeight(25);
         button->setStyleSheet("border-radius:5px; background-color: transparent;color:rgb(255,255,255)");
           buttons.append(button);
          ui->horizontalLayout->addWidget(button);
    }
}
QString FolderWidget::readjsonreturnpath(QString key)
{
    //读取json文件
    QString filepath=QCoreApplication::applicationDirPath()+"/json/pathjson.json";
    QFile loadFile(filepath);
          if(!loadFile.open(QIODevice::ReadOnly| QIODevice::Text))
          {

                 return "";
          }
    QByteArray array = loadFile.readAll(); //将数据读到array中
    loadFile.close();
    //json错误抛出
    QJsonParseError json_error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(array,&json_error);
    if(json_error.error != QJsonParseError::NoError)//判断是否可以解析
     {
           qDebug()<<"json error!";
           return "";
      }
     QJsonObject rootObj = jsonDoc.object();
     QJsonValue foldepath = rootObj.value(key);
     return foldepath.toString();
}
void FolderWidget::openfolder2()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    Qt::WindowFlags m_flags=fileDialog->windowFlags();
      fileDialog->setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
    fileDialog->setWindowTitle(tr("选择要保存扫描结果的路径"));
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->exec();
    auto selectDir = fileDialog->selectedFiles();
    if (selectDir.size()>0)
    {
        qDebug() << "Dir Path:" << selectDir.at(0);

        waterfolderpath=selectDir.at(0);
         qpbopenfload->setText(waterfolderpath);
    }

}
void FolderWidget::openfolder3()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    Qt::WindowFlags m_flags=fileDialog->windowFlags();
      fileDialog->setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
    fileDialog->setWindowTitle(tr("选择要保存扫描结果的路径"));
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->exec();
    auto selectDir = fileDialog->selectedFiles();
    if (selectDir.size()>0)
    {
        qDebug() << "Dir Path:" << selectDir.at(0);

        quheibianfolderpath=selectDir.at(0);
        quheibianbutton->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/wenjianjia1.png"));
        quheibianbutton->setIconSize(QSize(20,20));
        quheibianbutton->setStyleSheet("color:rgba(0,0,0,1)");
        quheibianbutton->setText(quheibianfolderpath);
    }

}
void FolderWidget:: checkisemty(){
    QString inipath=QCoreApplication::applicationDirPath()+readjsonreturnpath("inipath");

//    //读取到ini位置
    QSettings *settings;//申明一个QSetting类函数

    settings = new QSettings (inipath, QSettings::IniFormat);//构建函数


    QString iniupdateitem=readjsonreturnpath("iniupdateitem2");
//    //修改原始文件所在位置
//    folderpath=folderpath+"/";
    folderpath=settings->value(iniupdateitem).toString();
    QStringList qlist=folderpath.split("/");
    QString foldername=qlist.at(qlist.size()-1);
    folderpath=QCoreApplication::applicationDirPath()+readjsonreturnpath("parentpath")+foldername;
    QDir dir(folderpath);
    if(dir.exists()){
        //设置过滤器
           QStringList filters;
           filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.tif")<<QString("*.png")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp");
          dir.setFilter (QDir::Files | QDir::NoSymLinks); // 设置类型过滤器，只为文件格式
           dir.setNameFilters (filters);  // 设置文件名称过滤器，只为 filters 格式（后缀为.jpeg 等图片格式）
           int dir_count = dir.count();
            if(dir_count==0){

            }else{
                if(flag2){
                readImageFile();
                flag2=false;
                }
            }
    }
}

void FolderWidget::daochu()
{
    if(choose_list.size()<=0){
       buttons.at(6)->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/daochu2.png"));
         return;
    }else{

    //判断媒体是否存在
    QString path1="/media/udisk0";
    QString path2="/media/udisk1";
    QDateTime time=QDateTime::currentDateTime();
    uint time_t=time.toTime_t();
    QString path3="/output_"+QString::number(time_t);
    QDir qdir(path1);
    QDir qdir2(path2);
    if(qdir.exists()){
        QDir qdir3(path1+path3);
        if(!qdir3.exists()){
            qdir3.mkdir(path1+path3);

            for(int j=0;j<choose_list.size();j++){
                QFile qf(choose_list.at(j));
                if(qf.exists()){
                    QStringList  filelist=choose_list.at(j).split("/");

                    QString filepath=path1+path3+"/"+filelist.at(filelist.size()-1);
                    qf.copy(filepath);
                    qDebug()<<"output";
                }
            }
        }
        QMessageBox msgBox(QMessageBox::Warning, tr("导出成功"), tr("导出成功"), QMessageBox::Ok, this);
        msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
         int result = msgBox.exec();
    }else if(qdir2.exists()){
        QDir qdir3(path2+path3);
        if(!qdir3.exists()){
            qdir3.mkdir(path2+path3);

            for(int j=0;j<choose_list.size();j++){
                QFile qf(choose_list.at(j));
                if(qf.exists()){
                    QStringList  filelist=choose_list.at(j).split("/");

                    QString filepath=path1+path3+"/"+filelist.at(filelist.size()-1);
                    qf.copy(filepath);
                    qDebug()<<"output";
                }
            }
        }
        QMessageBox msgBox(QMessageBox::Warning, tr("导出成功"), tr("导出成功"), QMessageBox::Ok, this);
        msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
         int result = msgBox.exec();
    }else{
        QMessageBox msgBox(QMessageBox::Warning, tr("找不到U盘所在位置，请重试"), tr("找不到U盘所在位置，请重试"), QMessageBox::Ok, this);
        msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
         int result = msgBox.exec();
   }
    }
}

void FolderWidget::suaxing()
{
     readImageFile();
}
