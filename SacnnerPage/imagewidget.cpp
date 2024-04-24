#include "imagewidget.h"
#include "ui_imagewidget.h"
#include "QCheckBox"
#include "QRadioButton"
#include "QButtonGroup"
#include "QList"

#include <ui_imagewidget.h>
// 二值化阈值
#define GRAY_THRESH 150

// 直线上点的个数
#define HOUGH_VOTE 50
//#pragma comment(lib,"opencv_world330.lib")

/**
 * @brief imagewidget::imagewidget
 * @param parent
 *  *  图片展示页面，
 *   大致上该页面有两个部分，一部分是图片的展示，另一部分是图片的处理
 *      图片展示区：
 *          顶部工具栏：
 *          左部工具栏：
 *
 *          中间右边展示区:
 *              左划上一页
 *              右划下一页
 *              手势放大与缩小
 *              隐藏工具栏
 *
 *      图片目录：
 *          图片回显如何做到不卡
 *          图片选中回到图片展示处理区
 *      图片处理：
 *          （水印+黑边+校正）
 *
 *
 *      划分小问题：第一步把页面搭出来，第二部件多个功能分解成多个小函数。
 *          图片放大与缩小功能
 *          图片旋转功能
 *          图片切换功能
 */
using namespace cv;
using namespace  std;
imagewidget::imagewidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imagewidget)
{
    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
       this->screenwidth=screenRect.width();
        this->screenheight=screenRect.height();
    Qt::WindowFlags m_flags =this->windowFlags();
//     Qt::WindowStaysOnTopHint
       this->setWindowFlags(m_flags |Qt::WindowStaysOnTopHint);
    this->setMinimumWidth(screenwidth);
    this->setMinimumHeight(screenheight);
    buttonlsit=new QList<QPushButton*>();
    ui->setupUi(this);
    folderwidget=new FolderWidget();
    sunscannerwidget=new scannerwidget();
    timer2=new QTimer(this);
    connect(timer2, &QTimer::timeout,
            this,[=]{
        checkfolderisempty();
    });
    timer4=new QTimer(this);

    connect(timer4, &QTimer::timeout,
            this,[=]{
        checkscannfinsh();
    }
    );

}

imagewidget::~imagewidget()
{
    delete ui;
}
/**
 * @brief imagewidget::initimagewidget
 *  页面初始化
 *      改页面有4个布局：两个垂直，两个水平
 *
 *
 *
 */
void imagewidget::initimagewidget()
{

    ui->horizontalWidget_2->resize(this->screenwidth,this->screenheight);


    page=0;
     this->setWindowTitle(tr("图片处理页"));
    ui->horizontalLayout->setStretch(2,5);
     //第一个按钮 放大 缩小
    //创建工具框
    toolbarwidget=new MyWidget();
    initwidget(toolbarwidget,150,150,this->screenheight,this->screenheight,"background-color:rgba(255,255,255,0.3);border-radius:20px;color:rgba(255,255,255,1);margin-top:0px;");
    Mylabel *temlabel=new Mylabel();
    temlabel->resize(150,this->screenheight);
    temlabel->setStyleSheet("");
    QVBoxLayout *qvbl=new QVBoxLayout();

    QString titles[] ={tr("上一页"),tr("下一页"),tr("放大"),tr("缩小"),tr("旋转"),tr("水印"),tr("黑边"),tr("校正"),tr("恢复")};
       QString icons[]={QCoreApplication::applicationDirPath()+"/icon/shanyiye2.png",
                     QCoreApplication::applicationDirPath()+"/icon/xiayiye.png",
                     QCoreApplication::applicationDirPath()+"/icon/bebig.png",
                     QCoreApplication::applicationDirPath()+"/icon/besmall.png",
                     QCoreApplication::applicationDirPath()+"/icon/xuanzhuan.png",
                     QCoreApplication::applicationDirPath()+"/icon/shuiyin.png",
                     QCoreApplication::applicationDirPath()+"/icon/waibianju.png",
                     QCoreApplication::applicationDirPath()+"/icon/jiaozheng.png",
                     QCoreApplication::applicationDirPath()+"/icon/huanyuan.png"

                    };
     QString styles [] ={"background-color:transparent;border-bottom:solid 1px;margin-top:-20px;",
                        "background-color:transparent;border-bottom:solid 1px;margin-top:-20px",
                         "background-color:transparent;border-bottom:solid 1px;margin-top:-10px;",
                         "background-color:transparent;border-bottom:solid 1px;margin-top:-10px;",
                         "background-color:transparent;border-bottom:solid 1px;margin-top:-10px;",
                         "background-color:transparent;border-bottom:solid 1px;margin-top:-10px;",
                         "background-color:transparent;border-bottom:solid 1px;margin-top:-10px;",
                         "background-color:transparent;border-bottom:solid 1px;margin-top:-10px;",
                        "background-color:transparent;border-bottom:solid 1px;margin-top:-10px;"


                         };
     for(unsigned int i=0;i<sizeof (titles) / sizeof (QString);i++){
          initlefttool(titles[i],icons[i],styles[i], qvbl,temlabel,i);
     }

    temlabel->setParent(toolbarwidget);
    ui->horizontalLayout_2->addWidget(toolbarwidget);




    //平滑窗口
    imageshowwidget=new MyWidget();
    imageshowwidget->setStyleSheet("overflow:hidden;");
    temlabel2=new Mylabel();
    temlabel2->resize(this->screenwidth-200,this->screenheight-100);
    initwidget(imageshowwidget,this->screenwidth-200,this->screenwidth-200,this->screenheight-90,this->screenheight-90,"background-color:rgba(255,255,255,0.5);border-radius:20px;color:rgba(255,255,255,1);");
    //读取文件夹获取图片的个数+将每个图片存储在转换在每个Qwidget


    QFont qf;



   qpb=new QPushButton();

   label=new Mylabel();
   qf.setPixelSize(30);
   label->setStyleSheet("color:rgba(255,255,255,1);margin-top:30px;");
   label->setFont(qf);
   QString choosetext=tr("当前页: ")+QString::number(page+1)+QString("/")+QString::number(string_list.size())+"\n";
   label->setText(choosetext);
   connect(label,&Mylabel::clicked,this,[=]{
//        jumpPage();
   });
     ui->horizontalLayout_3->addWidget(label);
   ui->horizontalLayout_3->setAlignment(label,Qt::AlignLeft);




  shuxing=new QPushButton();

  shuxing->setFont(qf);
  shuxing->setMaximumHeight(40);
  shuxing->setText(tr("属性"));
  shuxing->setFlat(true);
  shuxing->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/shuxing.png"));
  shuxing->setIconSize(QSize(30,30));
   shuxing->setStyleSheet("background-color:transparent;color:rgb(255,255,255)");
   connect(shuxing,&QPushButton::clicked,this,[=]{openshuxing();});
   ui->horizontalLayout_3->addWidget(shuxing);
   ui->horizontalLayout_3->setAlignment(shuxing,Qt::AlignLeft);


   huanyaun=new QPushButton();

   huanyaun->setFont(qf);
   huanyaun->setMaximumHeight(40);
   huanyaun->setText(tr("还原"));
   huanyaun->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/huanyuan.png"));
  huanyaun->setIconSize(QSize(30,30));
    huanyaun->setFlat(true);
    huanyaun->setStyleSheet("background-color:transparent;color:rgb(255,255,255)");
    connect(huanyaun,&QPushButton::clicked,this,[=]{openhuanyaun();});
    ui->horizontalLayout_3->addWidget(huanyaun);
    ui->horizontalLayout_3->setAlignment(huanyaun,Qt::AlignLeft);


    shaomiao=new QPushButton();

    shaomiao->setFont(qf);
    shaomiao->setMaximumHeight(40);
    shaomiao->setText(tr("扫描"));
     shaomiao->setFlat(true);
    shaomiao->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/dayinji.png"));
    shaomiao->setIconSize(QSize(30,30));
        shaomiao->setStyleSheet("background-color:transparent;color:rgb(255,255,255)");
    connect(shaomiao,&QPushButton::clicked,this,[=]{scannerImage();});
    ui->horizontalLayout_3->addWidget(shaomiao);
    ui->horizontalLayout_3->setAlignment(shaomiao,Qt::AlignLeft);





    mulu=new QPushButton();
    mulu->setStyleSheet("background-color:transparent;color:rgb(255,255,255)");
    mulu->setFont(qf);
    mulu->setMaximumHeight(30);
    mulu->setText(tr("列表"));
    mulu->setFlat(true);
    mulu->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/wenjianjia1.png"));
    mulu->setIconSize(QSize(30,30));
    connect( mulu,&QPushButton::clicked,this,[=]{openfolder();
});
    ui->horizontalLayout_3->addWidget( mulu);
    ui->horizontalLayout_3->setAlignment( mulu,Qt::AlignLeft);
    shangyicheng=new QPushButton();
    shangyicheng->setStyleSheet("background-color:transparent;color:rgb(255,255,255)");
    shangyicheng->setFont(qf);
    shangyicheng->setMaximumHeight(40);
    shangyicheng->setText(tr("返回"));
    shangyicheng->setFlat(true);
    shangyicheng->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/tuichu2.png"));
    shangyicheng->setIconSize(QSize(30,30));
    connect(shangyicheng,&QPushButton::clicked,this,[=]{backhome();});
    ui->horizontalLayout_3->addWidget(shangyicheng);
    ui->horizontalLayout_3->setAlignment(shangyicheng,Qt::AlignLeft);

//    //属性显示
//    shuxingtext=new Mylabel();
//    shuxingtext->setFont(qf);
//    ui->horizontalLayout_3->addWidget(shuxingtext);

    qvbox=new QVBoxLayout();
    imagesize=string_list.size();
    temlabel2->setLayout(qvbox);
    translationwidget=new TranslationWidget(imagelistwidget);
    translationwidget->setMoveEnabled(false);
    temlabel2->setParent(imageshowwidget);
    ui->horizontalLayout_4->addWidget(imageshowwidget);
    readfloadimage();

    //设置背景图片
    QPixmap pixmap2 = QPixmap(QCoreApplication::applicationDirPath()+"/image/bg.jpg").scaled(this->screenwidth,this->screenheight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    QPalette pal=this->palette();
    pal.setBrush(QPalette::Background,QBrush(pixmap2));
    this->setAutoFillBackground(true);
    setPalette(pal);
    QString choosetext2=tr("当前页: ")+QString::number(page+1)+QString("/")+QString::number(string_list.size())+"\n";
    label->setText(choosetext2);
    timer3=new QTimer(this);
    connect(timer3, &QTimer::timeout,
            this,[=]{
        checksize();
    });
    timer3->start(1000);
}
void imagewidget::changelabevalue(){
    QString choosetext=tr("当前页: ")+QString::number(page+1)+QString("/")+QString::number(string_list.size())+"\n";
    label->setText(choosetext);
}
void imagewidget::checkfolderisempty()
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
                readfloadimage();

            }

    }

}


//初始化窗口
void imagewidget::initwidget(QWidget *q,int minwidth,int maxwidth,int minheight,int maxheight,QString style)
{
    q->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    q->setMinimumWidth(minwidth);
    q->setMinimumHeight(minheight);
    q->setMaximumWidth(maxwidth);
    q->setMaximumHeight(maxheight);
    q->setStyleSheet(style);
}
//初始化工具栏
void imagewidget::initlefttool(QString text,QString imagepath,QString style,  QVBoxLayout *qvbl,Mylabel *temlabel,int index)
{
    QPushButton *qpb=new QPushButton();
    buttonlsit->append(qpb);
    QFont qf;
    qf.setPixelSize(30);
    qpb->setFont(qf);
    qpb->setText(text);
    qpb->setIcon(QIcon(imagepath));
    qpb->setIconSize(QSize(40,40));
    qpb->setMinimumHeight(70);
    qpb->setMaximumHeight(70);
    qpb->setStyleSheet(style);
    qvbl->addWidget(qpb);
    temlabel->setLayout(qvbl);
    connect(qpb,&QPushButton::clicked,this,[=]{
        switch (index) {
             //上一页
            case 0: changeUpPage();break;
            //下一页
            case 1:changeNextPage();break;
            case 2:beBigImage();break;
            case 3:beSmallImage();break;
            case 4:rotateImage();break;
            case 5:openwaterqpb();break;
            case 6: blackEdgeImage();break;
            case 7:checkImage();break;
            case 8:huifu();break;

        }
    });

}
//初始化窗口页面
void imagewidget::initwidgetpage(QString imagepath)
{

    isw=new imagesonwidget();
    isw->doLoadImage(imagepath);
    isw->setMinimumWidth(this->screenwidth-220);
    isw->setMinimumHeight(this->screenheight-120);
    isw->setMaximumWidth(this->screenwidth-220);
    isw->setMaximumHeight(this->screenheight-120);
    isw->setStyleSheet("background-color:transparent;padding-left:10px;padding-right:10px; ");
    imagelistwidget.append(isw);
    imagepaths.append(imagepath);

}
//上一页
void imagewidget::changeUpPage()
{

    if(imagewidget::page>0&&string_list.size()>0){
        if(page>=0){
            QString tempcurrpage;
            if(page<=9){
                tempcurrpage=" "+QString::number(page);
            }else{
                tempcurrpage=QString::number(page);
            }
            QString choosetext=tr("当前页:")+tempcurrpage+QString("/")+QString::number(string_list.size())+"\n";
            label->setText(choosetext);


            if(page==oldallsize&&oldallsize>0&&oldallsize-2>=0){
                qDebug()<<page<<"shangyiye";

                 allsize=oldallsize;
                 oldallsize-=2;

               readfloadimage();

            }
        }
        imagewidget::page-=1;
        if(page==0){
             changecolor(0,"/icon/shanyiye");
        }else{
        changecolor2();
        }

        on_pushButton_clicked();
    }else{
        if(string_list.size()==0){

            changecolor(0,"/icon/shanyiye");


        }else{
             changecolor(0,"/icon/shanyiye");

        }
    }

}
//下一页
void imagewidget::changeNextPage()
{

    qDebug()<<imagewidget::imagesize;

     if(imagewidget::page!=string_list.size()-1&&string_list.size()>0){
            qDebug()<<page;
            imagewidget::page+=1;
            if(page<=string_list.size()-1){
                QString tempcurrpage;
                if(page<9){
                    tempcurrpage="  "+QString::number(page+1);
                }else{
                    tempcurrpage=QString::number(page+1);
                }
                QString choosetext=tr("当前页:")+tempcurrpage+QString("/")+QString::number(string_list.size())+"\n";
                label->setText(choosetext);
            if(page==allsize&&string_list.size()>allsize){

                oldallsize=allsize;
                allsize+=2;
              readfloadimage();

            }
            }
            if(page==string_list.size()-1){
                changecolor(1,"/icon/xiayiye");
            }else{
             changecolor2();
            }
    on_pushButton_clicked();
     }else{

         if(string_list.size()==0){
                  changecolor(1,"/icon/xiayiye");


         }else{
                   changecolor(1,"/icon/xiayiye");

         }
     }

}
//图片放大
void imagewidget::beBigImage()
{

    if(string_list.size()>0){
         imagesonwidget *imsw=qobject_cast<imagesonwidget *>(imagelistwidget.at(page%2));
         imsw->doZoomInImage();
      changecolor2();
    }else{
      changecolor(2,"/icon/bebig");


    }

}
//图片缩小
void imagewidget::beSmallImage()
{

     if(imagelistwidget.size()>0){
         qDebug()<<page;
    imagesonwidget *imsw=qobject_cast<imagesonwidget *>(imagelistwidget.at(page%2));
    imsw->doZoomOutImage();
   changecolor2();
     }else{

           changecolor(3,"/icon/besmall");
     }
}
//旋转图片
void imagewidget::rotateImage()
{

     if(string_list.size()>0){
         qDebug()<<page%2;
     imagesonwidget *imsw=qobject_cast<imagesonwidget *>(imagelistwidget.at(page%2));
     imsw->doRotate();
     changecolor2();
     }else{
         changecolor(4,"/icon/xuanzhuan");
     }
}
//水印生成水印图
void imagewidget::watermarkImage(QString watercontent, int fontsize,int angle,bool allscreen,QString waterimagepath)
{

 if(string_list.size()>0){
    imagesonwidget *imsw=qobject_cast<imagesonwidget *>(imagelistwidget.at(page%2));
     QString imagepath=imsw->m_imagepath;
     //保存水印位置
     QStringList temp=imagepath.split("/");
     QString waterimagepath2=waterimagepath+"/"+temp.at(temp.size()-1);
     QByteArray tempstr=waterimagepath2.toLocal8Bit();
     string waterimagepathstd=string(tempstr);
     QByteArray tempstr2=imagepath.toLocal8Bit();
     string imagepathstd=std::string(tempstr2);
     cv::Mat mat=cv::imread(imagepathstd);
     int cols=mat.cols;
     int rows=mat.rows;
     QImage image(cols,rows,QImage::Format_ARGB32);
      image.fill(Qt::transparent);
     QPainter painter(&image); //为这个QImage构造一个QPainter
     if(allscreen){
          painter.setCompositionMode(QPainter::CompositionMode_Screen);
     }else{
         painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
     }

     QPen pen = painter.pen();
//     /
     pen.setColor(qRgba(105,105,105,1));
     QFont font = painter.font();
     font.setBold(true);//加粗
     font.setPixelSize(fontsize*10);//改变字体大小

     painter.setPen(pen);
     painter.setFont(font);
     if(allscreen){
         painter.translate(0,0);          // 第1步：变换旋转中心到所绘制文字左下角
         painter.rotate(angle);
         for(int i=0;i<cols/(fontsize*10);i++){
            for(int j=0;j<rows/(fontsize*10);j++){
                painter.drawText(i*(fontsize*20)+150,j*(fontsize*10),watercontent);

            }

           }


     }else{

          painter.drawText(image.rect(),Qt::AlignCenter,watercontent);
     }

     //创建水印
       //生成文件名称：输入的文件名+时间戳+文件后缀

     QDateTime time=QDateTime::currentDateTime();
     QString str = time .toString("yyyy_MM_dd_hh_mm_ss");
     //读取json
       QString waterpathtemp=QCoreApplication::applicationDirPath()+readjsonreturnpath("watertemppath")+"/"+watercontent+str+".png";
      //保存水印图片
     bool istrue=image.save(waterpathtemp);

     if(istrue){
         //将两个图片进行合并然后替换原来图片，展示图片
         //保存添加水印前的原图
         QString waterinitimagepath=QCoreApplication::applicationDirPath()+readjsonreturnpath("yuanpath");

         QStringList qltemp=imagepath.split("/");
         QString waterinitimagename=qltemp.at(qltemp.size()-1);
         bool fal=QFile::copy(imagepath,waterinitimagepath+"/"+waterinitimagename);

         QString waterimagepath=waterpathtemp;
         QByteArray tempstr=waterimagepath.toLocal8Bit();
         string watermarkpath=string(tempstr);
         cv::Mat watermat=cv::imread(watermarkpath);
         cv::Mat result;
         cv::addWeighted(watermat,0.2,mat,0.95,0,result);

         cv::imwrite(imagepathstd,result);
         imsw->doLoadImage(imagepath);
         huifu();
     }else{
         QMessageBox msgBox(QMessageBox::Warning, tr("水印存储路径不存在"), tr("水印存储路径不存在"), QMessageBox::Ok, this);
         msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
         int result = msgBox.exec();
     }
     bool re=QFile::remove(waterpathtemp);
 }else{
               changecolor(5,"/icon/shangyiye");

 }

}
//去黑边
void imagewidget::blackEdgeImage()
{

    if(string_list.size()>0){
    temp=new QWidget();
    temp->resize(this->screenwidth,this->screenheight);
    temp->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    temp->setWindowFlags(Qt::FramelessWindowHint);
    temp->setAttribute(Qt::WA_TranslucentBackground, true);
    Qt::WindowFlags m_flags=windowFlags();
    temp->setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
    temp->show();
//    qDebug()<<"黑边";
    QDialog dialog(temp);
    dialog.setWindowFlags(Qt::FramelessWindowHint);
    dialog.setStyleSheet("background-color:rgba(250,250,250,1);border-radius:20px;padding:15px;");
    QVBoxLayout qvl(&dialog);
    //
    quheibianbutton=new QPushButton();
    quheibianbutton->setText(tr("去黑边"));
    QFont qf;
    qf.setPixelSize(15);
    quheibianbutton->setFont(qf);
    qvl.addWidget(quheibianbutton);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
            Qt::Horizontal, &dialog);

    //读取图片位置
    imagesonwidget *imsw=qobject_cast<imagesonwidget *>(imagelistwidget.at(page%2));
     QString imagepath=imsw->m_imagepath;
     QByteArray tempstr=imagepath.toLocal8Bit();
     string imagepathstd=std::string(tempstr);
     cv::Mat ori_image=cv::imread(imagepathstd);
     //图片预览
     int width=ori_image.cols<this->screenwidth-170?ori_image.cols:ori_image.cols*0.25;
         int height=ori_image.rows<this->screenheight?ori_image.rows:ori_image.rows*0.25;
           cs=new capturescreen(this,imagepath,width,height,ori_image.cols,ori_image.rows);
           cs->resize(width,height);
            dialog.resize(QSize(ori_image.cols*0.25+20,ori_image.rows*0.25+150));
    qvl.addWidget(cs);
    // Add Cancel and OK button
    qf.setPixelSize(20);
     buttonBox.button(QDialogButtonBox::Ok)->setFont(qf);
        buttonBox.button(QDialogButtonBox::Ok)->setText(tr("保存"));
         buttonBox.button(QDialogButtonBox::Ok)->setStyleSheet("background-color:#91C8E4;color:rgba(255,255,255,1);margin-right:20px;");

         buttonBox.button(QDialogButtonBox::Cancel)->setText(tr("取消"));
         buttonBox.button(QDialogButtonBox::Cancel)->setFont(qf);
           buttonBox.button(QDialogButtonBox::Cancel)->setStyleSheet("background-color:rgba(0,0,0,0.8);color:rgba(255,255,255,1);margin-right:2s0px;");
    qvl.addWidget(&buttonBox);


    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        //判断文件夹是否存在
            QString  quheibianinitimagepath=QCoreApplication::applicationDirPath()+readjsonreturnpath("yuanpath");
            QStringList qltemp=imagepath.split("/");
            QString quheibianinitimagename=qltemp.at(qltemp.size()-1);
             bool fal=QFile::copy(imagepath,quheibianinitimagepath+"/"+quheibianinitimagename);

            cs->onSaveTemplateImage(imagepath);
            cs->deleteLater();
            temp->close();
            temp->deleteLater();
            imsw->doLoadImage(imagepath);
            huifu();

    }else{
       cs->deleteLater();
      temp->close();
     temp->deleteLater();

    }
}
else{

           changecolor(6,"/icon/quheibian");


    }



}
//恢复
void imagewidget::huifu(){

    if(string_list.size()>0){
        imagesonwidget *imsw=qobject_cast<imagesonwidget *>(imagelistwidget.at(page%2));
        QString imagepath=imsw->m_imagepath;
        imsw->doPresetImage();
    changecolor2();
    }else{
            changecolor(8,"/icon/huifu");

    }
}
//切换颜色
void imagewidget::changecolor2(){
    huanyaun->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/huanyuan.png"));
   huanyaun->setIconSize(QSize(30,30));
    if(buttonlsit->size()>0){
        QString icons[]={QCoreApplication::applicationDirPath()+"/icon/shanyiye.png",
                      QCoreApplication::applicationDirPath()+"/icon/xiayiye.png",
                      QCoreApplication::applicationDirPath()+"/icon/bebig.png",
                      QCoreApplication::applicationDirPath()+"/icon/besmall.png",
                      QCoreApplication::applicationDirPath()+"/icon/xuanzhuan.png",
                      QCoreApplication::applicationDirPath()+"/icon/shuiyin.png",
                      QCoreApplication::applicationDirPath()+"/icon/waibianju.png",
                      QCoreApplication::applicationDirPath()+"/icon/jiaozheng.png",
                      QCoreApplication::applicationDirPath()+"/icon/huanyuan.png"

                     };
       for(int j=0;j<buttonlsit->size();j++){
           QPushButton *qpb=buttonlsit->at(j);
           qpb->setIcon(QIcon(icons[j]));
       }
    }

}
void imagewidget::changecolor(int i,QString iconname)
{
    QString icons[]={QCoreApplication::applicationDirPath()+"/icon/shanyiye.png",
                  QCoreApplication::applicationDirPath()+"/icon/xiayiye.png",
                  QCoreApplication::applicationDirPath()+"/icon/bebig.png",
                  QCoreApplication::applicationDirPath()+"/icon/besmall.png",
                  QCoreApplication::applicationDirPath()+"/icon/xuanzhuan.png",
                  QCoreApplication::applicationDirPath()+"/icon/shuiyin.png",
                  QCoreApplication::applicationDirPath()+"/icon/waibianju.png",
                  QCoreApplication::applicationDirPath()+"/icon/jiaozheng.png",
                  QCoreApplication::applicationDirPath()+"/icon/huanyuan.png"

                 };
   for(int j=0;j<buttonlsit->size();j++){
       QPushButton *qpb=buttonlsit->at(j);
       qpb->setIcon(QIcon(icons[j]));
   }
   if(buttonlsit->size()!=0){
   QPushButton *qpb=buttonlsit->at(i);
   qpb->setIcon(QIcon(QCoreApplication::applicationDirPath()+iconname+"2.png"));
   }
}
//校正
void imagewidget::checkImage()
{

    if(string_list.size()>0){
        imagesonwidget *imsw=qobject_cast<imagesonwidget *>(imagelistwidget.at(page%2));
        QString imagepath=imsw->m_imagepath;
        imsw->doPresetImage();
        QString jiaozhengpath=QCoreApplication::applicationDirPath()+readjsonreturnpath("yuanpath");
        QStringList qltemp=imagepath.split("/");
        QString jiaozhenginitimagename=qltemp.at(qltemp.size()-1);
        bool falg=QFile::copy(imagepath,jiaozhengpath+"/"+jiaozhenginitimagename);
        QString savepare=imagepath;
        dojiaozheng(imagepath, imagepath);
        imsw->doLoadImage(imagepath);
        huifu();
  changecolor2();
    }else{
        changecolor(7,"/icon/jiaozheng");

     }


}
//扫描
void imagewidget::scannerImage()
{

    QString inipath=QCoreApplication::applicationDirPath()+readjsonreturnpath("inipath");
    QString scannerpagepath=readjsonreturnpath("scannerpageini");
    //读取到ini位置
    QSettings *settings;//申明一个QSetting类函数
     QSettings *settings2;//申明一个QSetting类函数
    settings = new QSettings (inipath, QSettings::IniFormat);//构建函数

    settings2=new QSettings(scannerpagepath,QSettings::IniFormat);
    QString iniupdateitem=readjsonreturnpath("iniupdateitem2");
    QString scannerpageitem=readjsonreturnpath("scannerpageitem");
    //修改原始文件所在位置
    folderpath=settings->value(iniupdateitem).toString();
    settings2->setValue(scannerpageitem,folderpath);

    //启动扫描仪
    QProcess *process=new QProcess();
     QString workspace=QCoreApplication::applicationDirPath()+readjsonreturnpath("scannerworkspace");
     QString shcode="sh "+readjsonreturnpath("scannershellname");
     process->setWorkingDirectory(workspace);
     process->start(shcode);

     if(process->waitForFinished()){
         process->close();
        timer4->start(1000);
     }else{
         QMessageBox msgBox(QMessageBox::Warning, tr("扫描失败"), tr("扫描失败"), QMessageBox::Ok, this);
         msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
         int result2 = msgBox.exec();
     }

}

int imagewidget::readfloadimage2()
{
    QString inipath=QCoreApplication::applicationDirPath()+readjsonreturnpath("inipath");

//    //读取到ini位置
    QSettings *settings;//申明一个QSetting类函数
    settings = new QSettings (inipath, QSettings::IniFormat);//构建函数
    QString iniupdateitem=readjsonreturnpath("iniupdateitem2");
//    //修改原始文件所在位置
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

           return dir_count;
    }
}

//打开目录
void imagewidget::openfolder()
{
//    qDebug()<<"目录";

//
    folderwidget->resize(screenwidth,screenheight);
    Qt::WindowFlags m_flags=windowFlags();
    folderwidget->setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
     folderwidget->initfolder();
     folderwidget->show();
     folderwidget->suaxing();
}

//选择跳转页数
void imagewidget::jumpPage(){
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::FramelessWindowHint);
    dialog.setStyleSheet("background-color:rgba(250,250,250,1);border-radius:5px;padding:15px;");
    dialog.resize(QSize(500,150));
    QFormLayout form(&dialog);
    // Value1
    QString value1 = QString(tr("跳转页数:"));
    QSpinBox *spinbox1=new QSpinBox(&dialog);
    spinbox1->setValue(1);
    spinbox1->setMinimumHeight(10);
    QFont qf;
    qf.setPixelSize(15);
    spinbox1->setFont(qf);
    spinbox1->setStyleSheet("border:1px solid rgba(0,0,0,0.5);");
    form.addRow(value1, spinbox1);
    //选择保存水印文件夹

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
            Qt::Horizontal, &dialog);

        buttonBox.button(QDialogButtonBox::Ok)->setText("确定");
         buttonBox.button(QDialogButtonBox::Ok)->setStyleSheet("background-color:#91C8E4;color:rgba(255,255,255,1);margin-right:100px;");

         buttonBox.button(QDialogButtonBox::Cancel)->setText("取消");
           buttonBox.button(QDialogButtonBox::Cancel)->setStyleSheet("background-color:rgba(0,0,0,0.8);color:rgba(255,255,255,1);margin-right:100px;");

    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        if(imagewidget::page>=0&&string_list.size()>0){
            int jumppage=spinbox1->value();
            if(jumppage>string_list.size()-1){
                QMessageBox msgBox(QMessageBox::Warning, tr("没有这么多啦"), tr("没有这么多啦"), QMessageBox::Ok, this);
                msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
                int result = msgBox.exec();
            }else{

                imagewidget::page=jumppage;
                QString choosetext="当前页:"+QString::number(imagewidget::page+1)+QString("/")+QString::number(string_list.size())+"\n";
                label->setText(choosetext);
                on_pushButton_clicked();
            }


        }
    }
}
//打开属性
void imagewidget::openshuxing()
{
    if(string_list.size()>0){
    imagesonwidget *imsw=qobject_cast<imagesonwidget *>(imagelistwidget.at(page-oldallsize));
     QString imagepath=imsw->m_imagepath;
     QByteArray tempstr2=imagepath.toLocal8Bit();
     string imagepathstd=std::string(tempstr2);
     cv::Mat ori_image=cv::imread(imagepathstd);
    QStringList list = imagepath.split("/");
    QString imagename=list.at(list.size()-1);
    QString imagesize=QString::fromStdString(to_string(ori_image.cols))+"x"+QString::fromStdString(to_string(ori_image.rows));



    QDialog dialog(this);
    dialog.setWindowTitle(tr("属性"));
    dialog.setWindowFlags(Qt::FramelessWindowHint);
    dialog.setStyleSheet("background-color:rgba(250,250,250,1);border-radius:20px;padding:15px;");

    dialog.resize(QSize(800,450));

    QFormLayout form(&dialog);

    // Value1
    QFont qf;
    qf.setPixelSize(20);
    QString value1 = QString(tr("文件名:"));
    Mylabel *lab1=new Mylabel();
    lab1->setFont(qf);
    lab1->setText(value1);
    Mylabel *mylabel=new Mylabel();
    mylabel->setText(imagename);
    mylabel->setMinimumHeight(10);
    qf.setPixelSize(20);
     mylabel->setFont(qf);
     mylabel->setStyleSheet("border:1px solid rgba(0,0,0,0.5);");
    form.addRow(lab1,  mylabel);


    QString value2 = QString(tr("文件位置:"));
    Mylabel *mylabel2=new Mylabel();
    Mylabel *lab2=new Mylabel();
    lab2->setFont(qf);
    lab2->setText(value2);
    mylabel2->setText(imagepath.replace(imagename,""));
    mylabel2->setMinimumHeight(10);
    mylabel2->setFont(qf);
     mylabel2->setStyleSheet("border:1px solid rgba(0,0,0,0.5);");
    form.addRow(lab2,  mylabel2);

    QString value3= QString(tr("分辨率:"));
    Mylabel *mylabel3=new Mylabel();
    Mylabel *lab3=new Mylabel();
    lab3->setFont(qf);
    lab3->setText(value3);
    mylabel3->setText(imagesize);
    mylabel3->setMinimumHeight(10);
    mylabel3->setFont(qf);
     mylabel3->setStyleSheet("border:1px solid rgba(0,0,0,0.5);");
    form.addRow(lab3,  mylabel3);

    QString value4= QString(tr("创建时间:"));
    Mylabel *mylabel4=new Mylabel();
    Mylabel *lab4=new Mylabel();
    lab4->setFont(qf);
    lab4->setText(value4);
    QFileInfo qfileinfo(imagepath);
     QDateTime created =qfileinfo.birthTime();

     QDateTime lastModified = qfileinfo.lastModified();//最后修改时间
    mylabel4->setText(created.toString("yyyy-MM-dd hh:mm:ss"));
    mylabel4->setMinimumHeight(10);
    mylabel4->setFont(qf);
     mylabel4->setStyleSheet("border:1px solid rgba(0,0,0,0.5);");
    form.addRow(lab4,  mylabel4);

     QString value5= QString(tr("修改时间:"));
    Mylabel *mylabel5=new Mylabel();
    Mylabel *lab5=new Mylabel();
    lab5->setFont(qf);
    lab5->setText(value5);
    mylabel5->setText(lastModified.toString("yyyy-MM-dd hh:mm:ss"));
    mylabel5->setMinimumHeight(10);
    mylabel5->setFont(qf);
     mylabel5->setStyleSheet("border:1px solid rgba(0,0,0,0.5);");
    form.addRow(lab5,  mylabel5);

    //选择保存水印文件夹

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok,
            Qt::Horizontal, &dialog);
    buttonBox.button(QDialogButtonBox::Ok)->setFont(qf);
        buttonBox.button(QDialogButtonBox::Ok)->setText(tr("关闭"));
          buttonBox.button(QDialogButtonBox::Ok)->setMinimumWidth(200);
           buttonBox.button(QDialogButtonBox::Ok)->setMinimumHeight(50);
         buttonBox.button(QDialogButtonBox::Ok)->setStyleSheet("background-color:#91C8E4;color:rgba(255,255,255,1);margin-right:300px;");

         form.addRow(&buttonBox);
         QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
         QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

            if (dialog.exec() == QDialog::Accepted) {
                dialog.close();
            }



    }else{

    shuxing->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/shuxing2.png"));

    }
}
//打开水印
void imagewidget::openwaterqpb()
{
    if(string_list.size()>0){


    QDialog dialog(this);
    dialog.setWindowFlags(Qt::FramelessWindowHint);
    dialog.setStyleSheet("QDialog{background-color:rgba(250,250,250,1);border-radius:20px;padding:15px;border:1px solid rgba(0,0,0,0.5);}");

    dialog.resize(QSize(500,350));
    QFont qf;
    qf.setPixelSize(15);
    QFormLayout form(&dialog);

    // Value1
    QString value1 = QString(tr("水印内容:"));
    Mylabel *qm1=new Mylabel();
    qm1->setText(value1);
    qm1->setFont(qf);
    QLineEdit *spinbox1=new QLineEdit(&dialog);
    spinbox1->setMinimumHeight(20);



    spinbox1->setFont(qf);
    spinbox1->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,0.5);border-radius:20px;padding:15px;margin-top:20px;}");
    form.addRow(qm1, spinbox1);

    QString value2 = QString(tr("字体大小:"));
    Mylabel *qm2=new Mylabel();
    qm2->setText(value2);
    qm2->setFont(qf);
    QSpinBox *spinBox = new QSpinBox(this);
    spinBox->setStyleSheet("QSpinBox{border:1px solid rgba(0,0,0,0.5);border-radius:20px;padding:15px; margin-top:20px;}"
                           "QSpinBox::up-button{subcontrol-position:right;subcontrol-origin:border;width: 40px;height: 40px;image: url("+QCoreApplication::applicationDirPath()+"/icon/xiayiye1.png);}"
                           "QSpinBox::down-button{subcontrol-position:left;subcontrol-origin:border;width: 40px;height: 40px;image: url("+QCoreApplication::applicationDirPath()+"/icon/shangyiye.png);}");
    spinBox->setRange(20, 80); // 变化范围
    spinBox->setSingleStep(1); // 步长
    spinBox->setValue(5); // 起始默认值
    spinBox->setFont(qf);
    form.addRow(qm2,spinBox);

    QString value3 = QString(tr("旋转角度:"));
    Mylabel *qm3=new Mylabel();
    qm3->setText(value3);
    qm3->setFont(qf);
    QSpinBox *spinBox2 = new QSpinBox(this);
    spinBox2->setStyleSheet("QSpinBox{border:1px solid rgba(0,0,0,0.5);border-radius:20px;padding:15px;margin-top:20px;}"
                            "QSpinBox::up-button{subcontrol-position:right;subcontrol-origin:border;width: 40px;height: 40px;image: url("+QCoreApplication::applicationDirPath()+"/icon/xiayiye1.png);}"
                            "QSpinBox::down-button{subcontrol-position:left;subcontrol-origin:border;width: 40px;height: 40px;image: url("+QCoreApplication::applicationDirPath()+"/icon/shangyiye.png);}");
    spinBox2->setRange(0, 360); // 变化范围
    spinBox2->setSingleStep(10); // 步长
    spinBox2->setValue(0); // 起始默认值
    spinBox2->setFont(qf);
    form.addRow(qm3,spinBox2);

    QString value4 = QString(tr("是否全屏:"));
    Mylabel *qm4=new Mylabel();
    qm4->setText(value4);
    qm4->setFont(qf);
    QHBoxLayout *qhbl=new QHBoxLayout();




    //is one
    QRadioButton *radio1=new QRadioButton();
    radio1->setChecked(true);
    radio1->setText(tr("单个"));
    radio1->setFont(qf);
       qhbl->addWidget(radio1);

    //all page
    QRadioButton *radio2=new QRadioButton();
    radio2->setText(tr("全屏"));
    radio2->setFont(qf);
       qhbl->addWidget(radio2);

       connect(radio1, &QRadioButton::toggled, [=](bool isChecked){
              if (isChecked == true){
                  qDebug() << "单选框被选中了";
                  issreen=false;
              }
              else if (isChecked == false){
                  qDebug() << "单选框失去焦点了";
                  issreen=true;
              }
          });

       connect(radio2, &QRadioButton::toggled, [=](bool isChecked){
              if (isChecked == true){
                  qDebug() << "单选框被选中了";
                issreen=true;
              }else if (isChecked == false){
                  qDebug() << "单选框失去焦点了";
                  issreen=false;
              }
          });


      if(radio1->isChecked()){
         issreen=false;
      }else if (radio2->isChecked()){
          issreen=true;
      }
    form.addRow( qm4,qhbl);



    //选择保存水印文件夹

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
            Qt::Horizontal, &dialog);

        buttonBox.button(QDialogButtonBox::Ok)->setText(tr("添加水印"));
        buttonBox.button(QDialogButtonBox::Ok)->setFont(qf);
         buttonBox.button(QDialogButtonBox::Ok)->setMinimumWidth(100);
          buttonBox.button(QDialogButtonBox::Ok)->setMinimumHeight(50);
         buttonBox.button(QDialogButtonBox::Ok)->setStyleSheet("background-color:#91C8E4;color:rgba(0,0,0,1);");

         buttonBox.button(QDialogButtonBox::Cancel)->setText(tr("取消"));
           buttonBox.button(QDialogButtonBox::Cancel)->setFont(qf);
          buttonBox.button(QDialogButtonBox::Cancel)->setMinimumWidth(100);
           buttonBox.button(QDialogButtonBox::Cancel)->setMinimumHeight(50);
           buttonBox.button(QDialogButtonBox::Cancel)->setStyleSheet("background-color:rgba(0,0,0,0.8);color:rgba(255,255,255,1);");

    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
           if(spinbox1->text()==""){
               QMessageBox msgBox(QMessageBox::Warning, tr("水印内容为空"), tr("水印内容不能为空"), QMessageBox::Ok, this);
               msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
                int result = msgBox.exec();
           }else{
                watermarkImage(spinbox1->text(),spinBox->value(),spinBox2->value(),issreen,waterfolderpath);
           }

    }
    }else{

        changecolor(5,"/icon/shuiyin");
//        QMessageBox msgBox(QMessageBox::Warning, tr("该目录内容为空"), tr("该目录内容为空,请重新选择目录"), QMessageBox::Ok, this);
//        msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
//        int result = msgBox.exec();

    }
}

void imagewidget::backhome()
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

                QString choosetext=tr("当前页: ")+QString::number(1)+QString("/")+QString::number(string_list.size())+"\n";
                label->setText(choosetext);

            }else{
                QString choosetext=tr("当前页: ")+QString::number(1)+QString("/")+QString::number(string_list.size())+"\n";
                label->setText(choosetext);
                page=0;
                oldallsize=0;
                allsize=2;
                readfloadimage();

                 translationwidget->setCurrentPage(0,true);
            }
    }

   this->close();

}

void imagewidget::on_pushButton_clicked()
{
//    qDebug()<<page%2<<"qiehuangyema";
     translationwidget->setCurrentPage(page-oldallsize,page,false);
}

//读取文件夹
void imagewidget::readfolder()
{



}

void imagewidget::checksize()
{
    if(string_list.size()!=readfloadimage2()){
        readfloadimage();
        QString tempcurrpage;
        if(page<=9){
            tempcurrpage=" "+QString::number(page+1);
        }else{
            tempcurrpage=QString::number(page+1);
        }
        QString choosetext=tr("当前页:")+tempcurrpage+QString("/")+QString::number(string_list.size())+" ";
        label->setText(choosetext);
    }
}




//读取图片
void imagewidget::readImagepage(QString folderpath)
{
        if(string_list.size()>0){
            string_list.clear();
        }
        if(imagelistwidget.size()>0){
             imagelistwidget.clear();
        }
        if(imagepaths.size()>0){
           imagepaths.clear();
        }

        imagesize=string_list.size();
        if(translationwidget!=nullptr){
            qvbox->removeWidget(translationwidget);

            delete(translationwidget);
            translationwidget=nullptr;
        }
         QDir dir(folderpath);
        if(dir.exists()){
            //设置过滤器
               QStringList filters;
               filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.png")<<QString("*.tif")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp");
               dir.setFilter (QDir::Files | QDir::NoSymLinks); // 设置类型过滤器，只为文件格式
               dir.setNameFilters (filters);  // 设置文件名称过滤器，只为 filters 格式（后缀为.jpeg 等图片格式）
               uint dir_count = dir.count();
                if(dir_count<=0){
                    return ;
                }else{
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

                    for(int i=0; i<dir_count; i++)
                    {
                        QString file_name = dir [i];  // 文件名称
                        QString file_path = folderpath + separator + file_name;   // 文件全路径
                        QChar h0 = 0x00;
                        if ( file_path.contains(h0))
                        {
                             file_path=file_path.replace(h0, "");
                        }
                        string_list.append(file_path);
                    }

                }

          }
        if(string_list.size()>0){


            for(int i=oldallsize;i<allsize;i++){

                 initwidgetpage(string_list.at(i));

            }
            qDebug()<<string_list.size();
            imagesize=string_list.size();

            translationwidget=new TranslationWidget(imagelistwidget);
            translationwidget->setMoveEnabled(false);
            qvbox->addWidget(translationwidget);


        }


}

void imagewidget::openfolder2()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    Qt::WindowFlags m_flags=fileDialog->windowFlags();
    fileDialog ->setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
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
void imagewidget::openfolder3()
{
    QFileDialog *fileDialog = new QFileDialog(temp);
    Qt::WindowFlags m_flags=fileDialog->windowFlags();
      fileDialog->setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
    fileDialog->setWindowTitle(tr("选择要保存扫描结果的路径"));
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->exec();
    auto selectDir = fileDialog->selectedFiles();
    if (selectDir.size()>0)
    {
        quheibianfolderpath=selectDir.at(0);
        quheibianbutton->setText(quheibianfolderpath);
    }

}
//还原图片
void imagewidget::openhuanyaun()
{
    if(string_list.size()>0){
    imagesonwidget *imsw=qobject_cast<imagesonwidget *>(imagelistwidget.at(page%2));
     QString imagepath=imsw->m_imagepath;
    QString waterinitimagepath=QCoreApplication::applicationDirPath()+readjsonreturnpath("yuanpath");
    QStringList qltemp=imagepath.split("/");
    QString waterinitimagename=qltemp.at(qltemp.size()-1);
    qDebug()<<waterinitimagename;
    QString filename=waterinitimagepath+"/"+waterinitimagename;
    QFile qfile(filename);
    qDebug()<<filename;
    if(qfile.exists()){
          QFile::remove(imagepath);
          QFile::copy(filename,imagepath);
           QFile::remove(filename);
          imsw->doLoadImage(imagepath);
         huifu();
    }else{
        huanyaun->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/icon/huanyuan2.png"));
       huanyaun->setIconSize(QSize(30,30));
//        QMessageBox msgBox(QMessageBox::Warning, tr("该图片无需恢复"), tr("该图片无需恢复"), QMessageBox::Ok, this);
//        msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
//        int result = msgBox.exec();

    }

    }else{

        QMessageBox msgBox(QMessageBox::Warning, tr("该目录内容为空"), tr("该目录内容为空,请重新选择目录"), QMessageBox::Ok, this);
        msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
        int result = msgBox.exec();
    }
}
/**
  读取文件夹中的内容，并将文件夹中的图片进行遍历初始化
 * @brief imagewidget::readfloadimage
 */
void imagewidget::readfloadimage()
{
    QString inipath=QCoreApplication::applicationDirPath()+readjsonreturnpath("inipath");
    QSettings *settings;//申明一个QSetting类函数
    settings = new QSettings (inipath, QSettings::IniFormat);//构建函数
    QString iniupdateitem=readjsonreturnpath("iniupdateitem2");

    //修改原始文件所在位置
      folderstring=settings->value(iniupdateitem).toString();
      QStringList qst=folderstring.split("/");
      folderstring=QCoreApplication::applicationDirPath()+readjsonreturnpath("parentpath")+qst.at(qst.size()-1);
      readImagepage(folderstring);

}

QString imagewidget::readjsonreturnpath(QString key)
{
    //读取json文件
    QString filepath=QCoreApplication::applicationDirPath()+"/json/pathjson.json";
    qDebug()<<filepath;
    QFile loadFile(filepath);
    qDebug()<<loadFile.exists();
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

void imagewidget::checkscannfinsh(){
         QFile qfile(QCoreApplication::applicationDirPath()+"/KST-903/KSTSMStart.yy");
         if(!qfile.exists()){
             QMessageBox *msgBox=new QMessageBox(QMessageBox::Information, tr("扫描成功"), "", QMessageBox::Ok|QMessageBox::Cancel, this);
                msgBox->setText("扫描"+QString::number(readfloadimageint())+"页");
                msgBox->setButtonText(QMessageBox::Ok,"确认");
                msgBox->setButtonText(QMessageBox::Cancel,"取消");
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
int imagewidget::readfloadimageint()
{
    QString inipath=QCoreApplication::applicationDirPath()+readjsonreturnpath("inipath");

//    //读取到ini位置
    QSettings *settings;//申明一个QSetting类函数

    settings = new QSettings (inipath, QSettings::IniFormat);//构建函数


    QString iniupdateitem=readjsonreturnpath("iniupdateitem");
//    //修改原始文件所在位置
//    folderpath=folderpath+"/";
    folderpath=settings->value(iniupdateitem).toString();
    QStringList qlist=folderpath.split("/");
    QString foldername=qlist.at(qlist.size()-1);
    folderpath=QCoreApplication::applicationDirPath()+readjsonreturnpath("parentpath")+foldername;

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
//校正
void imagewidget::dojiaozheng(QString imagename, QString outimagepath)
{
    //Read a single-channel image

      QByteArray cdata = imagename.toLocal8Bit();
      std::string imagenamestd=  string(cdata);
      // CV_LOAD_IMAGE_GRAYSCALE 0
      Mat srcImg = imread(imagenamestd,0);
      if (srcImg.empty())
          return ;
//      imshow("source", srcImg);

      Point center(srcImg.cols / 2, srcImg.rows / 2);

      //Expand image to an optimal size, for faster processing speed
      //Set widths of borders in four directions
      //If borderType==BORDER_CONSTANT, fill the borders with (0,0,0)
      Mat padded;
      int opWidth = getOptimalDFTSize(srcImg.rows);
      int opHeight = getOptimalDFTSize(srcImg.cols);
      copyMakeBorder(srcImg, padded, 0, opWidth - srcImg.rows, 0, opHeight - srcImg.cols, BORDER_CONSTANT, Scalar::all(0));

      Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
      Mat comImg;
      //Merge into a double-channel image
      merge(planes, 2, comImg);

      //Use the same image as input and output,
      //so that the results can fit in Mat well
      dft(comImg, comImg);

      //Compute the magnitude
      //planes[0]=Re(DFT(I)), planes[1]=Im(DFT(I))
      //magnitude=sqrt(Re^2+Im^2)
      split(comImg, planes);
      magnitude(planes[0], planes[1], planes[0]);

      //Switch to logarithmic scale, for better visual results
      //M2=log(1+M1)
      Mat magMat = planes[0];
      magMat += Scalar::all(1);
      log(magMat, magMat);

      //Crop the spectrum
      //Width and height of magMat should be even, so that they can be divided by 2
      //-2 is 11111110 in binary system, operator & make sure width and height are always even
      magMat = magMat(Rect(0, 0, magMat.cols & -2, magMat.rows & -2));

      //Rearrange the quadrants of Fourier image,
      //so that the origin is at the center of image,
      //and move the high frequency to the corners
      int cx = magMat.cols / 2;
      int cy = magMat.rows / 2;

      Mat q0(magMat, Rect(0, 0, cx, cy));
      Mat q1(magMat, Rect(0, cy, cx, cy));
      Mat q2(magMat, Rect(cx, cy, cx, cy));
      Mat q3(magMat, Rect(cx, 0, cx, cy));

      Mat tmp;
      q0.copyTo(tmp);
      q2.copyTo(q0);
      tmp.copyTo(q2);

      q1.copyTo(tmp);
      q3.copyTo(q1);
      tmp.copyTo(q3);

      //Normalize the magnitude to [0,1], then to[0,255]
      // CV_MINMAX 32
      normalize(magMat, magMat, 0, 1, 32);
      Mat magImg(magMat.size(), CV_8UC1);
      magMat.convertTo(magImg, CV_8UC1, 255, 0);
//      imshow("magnitude", magImg);
      //imwrite("imageText_mag.jpg",magImg);

      //Turn into binary image
      // CV_THRESH_BINARY      =0
      threshold(magImg, magImg, GRAY_THRESH, 255, 0);
//      imshow("mag_binary", magImg);
      //imwrite("imageText_bin.jpg",magImg);

      //Find lines with Hough Transformation
      vector<Vec2f> lines;
      float pi180 = (float)CV_PI / 180;
      Mat linImg(magImg.size(), CV_8UC3);
      HoughLines(magImg, lines, 1, pi180, HOUGH_VOTE, 0, 0);
      int numLines = lines.size();
      for (int l = 0; l<numLines; l++)
      {
          float rho = lines[l][0], theta = lines[l][1];
          Point pt1, pt2;
          double a = cos(theta), b = sin(theta);
          double x0 = a*rho, y0 = b*rho;
          pt1.x = cvRound(x0 + 1000 * (-b));
          pt1.y = cvRound(y0 + 1000 * (a));
          pt2.x = cvRound(x0 - 1000 * (-b));
          pt2.y = cvRound(y0 - 1000 * (a));
          line(linImg, pt1, pt2, Scalar(255, 0, 0), 3, 8, 0);
      }
//      imshow("lines", linImg);
      //imwrite("imageText_line.jpg",linImg);
      if (lines.size() == 3){
          cout << "found three angels:" << endl;
          cout << lines[0][1] * 180 / CV_PI << endl << lines[1][1] * 180 / CV_PI << endl << lines[2][1] * 180 / CV_PI << endl << endl;
      }

      //Find the proper angel from the three found angels
      float angel = 0;
      float piThresh = (float)CV_PI / 90;
      float pi2 = CV_PI / 2;
      for (int l = 0; l<numLines; l++)
      {
          float theta = lines[l][1];
          if (abs(theta) < piThresh || abs(theta - pi2) < piThresh)
              continue;
          else{
              angel = theta;
              break;
          }
      }

      //Calculate the rotation angel
      //The image has to be square,
      //so that the rotation angel can be calculate right
      angel = angel<pi2 ? angel : angel - CV_PI;
      if (angel != pi2){
          float angelT = srcImg.rows*tan(angel) / srcImg.cols;
          angel = atan(angelT);
      }
      float angelD = angel * 180 / (float)CV_PI;
      cout << "the rotation angel to be applied:" << endl << angelD << endl << endl;

      //Rotate the image to recover
      Mat rotMat = getRotationMatrix2D(center, angelD, 1.0);
      Mat dstImg = Mat::ones(srcImg.size(), CV_8UC3);
      warpAffine(srcImg, dstImg, rotMat, srcImg.size(), 1, 0, Scalar(255, 255, 255));
//      imshow("result", dstImg);
      QByteArray cdata2 = outimagepath.toLocal8Bit();
      std::string imagenamestd2=  string(cdata2);
      imwrite(imagenamestd2,dstImg);
}

