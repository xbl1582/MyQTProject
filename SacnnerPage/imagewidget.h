#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H
//#define MYJSONPATH ":/json/pathjson.json"
#include <QWidget>
#include<mylabel.h>
#include<mywidget.h>
#include<QVBoxLayout>
#include<translationwidget.h>
#include<imagesonwidget.h>
#include<QJsonParseError>
#include<QJsonObject>
#include<QJsonArray>
#include<folderwidget.h>
#include<capturescreen.h>
#include<scannerwidget.h>
#include <QApplication>
#include<QScreen>
#include<QPixmap>
#include<QDebug>
#include<mywidget.h>
#include<QPushButton>
#include<mylabel.h>
#include<QPalette>
#include<QColor>
#include<translationwidget.h>
#include<imagesonwidget.h>
#include<folderwidget.h>
#include<QFileDialog>
#include <QInputDialog>
#include<QDateTime>
#include<QFormLayout>
#include<QSpinBox>
#include<QDialogButtonBox>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include "capturescreen.h"
#include<QMessageBox>
#include<QSettings>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "QTimer"
#include "QRandomGenerator"
namespace Ui {
class imagewidget;
}

class imagewidget : public QWidget
{
    Q_OBJECT

public:
    int page=0;
    int imagesize=0;
    int screenwidth;
    int screenheight;
    QPixmap *qpixmap;
    bool isblack;

    //空框
    //工具框
     MyWidget *toolbarwidget;
    //图篇展示框
     QWidget *imageshowwidget;
    //目录显示框
     FolderWidget *folderwidget;
    //图片展示窗口
     QList<QWidget*> imagelistwidget;
     //图片展示窗口
      QList<QString> imagepaths;
     //窗口页面
      imagesonwidget *isw;
      //文件
      QPushButton *qpb;
      //水印
      QPushButton *waterqpb;
      //扫描
      QPushButton *shaomiao;
      //属性
      QPushButton *shuxing;
      //shangyicheng
      QPushButton *shangyicheng;
      //目录
      QPushButton *mulu;
      //属性显示
      Mylabel *shuxingtext;
      //显示页码
      Mylabel *label;
      //文件夹清空
      QPushButton *deletefilebutton;
      //截取框
      capturescreen *cs=nullptr;
       QString folderstring;
       QStringList string_list;
        QVBoxLayout *qvbox;
        Mylabel *temlabel2;
       scannerwidget *sunscannerwidget;

        QWidget *temp;
        bool  open=true;
        int oldallsize=0;
        int allsize=2;

        QTimer *timer2;
        QTimer *timer3;
        QTimer *timer4;

        QList<QPushButton*> *buttonlsit;
    explicit imagewidget(QWidget *parent = nullptr);
    ~imagewidget();
    void initimagewidget();
    void checkfolderisempty();
    void initwidget(QWidget *q,int minwidth,int maxwidth,int minheight,int maxheight,QString style);
    void initlefttool(QString text,QString imagepath,QString style,  QVBoxLayout *qvbl,Mylabel *temlabel,int index);
    void changelabevalue();
    void initwidgetpage(QString imagepath);
      void jumpPage();
      int readfloadimage2();
      void checkscannfinsh();
      int readfloadimageint();
    //选择图片
    void chooseImage();
    //上一页
    void changeUpPage();
    //下一页
    void changeNextPage();
    //放大
    void  beBigImage();
    //缩小
    void beSmallImage();
    //旋转
    void rotateImage();
    //水印
    void watermarkImage(QString watercontent,int fontsize,int angle,bool allscreen,QString waterimagepath);
    //黑边
    void  blackEdgeImage();
    //校正
    void checkImage();
    //扫描
    void scannerImage();
    //目录
    void  openfolder();
    //属性
    void openshuxing();
    //水印文件
    void openwaterqpb();
    //读取图片按钮
    void readImagepage(QString folderpath);
    //打开文件夹框
   void  openfolder2();
   void openfolder3();
   //还原
   void openhuanyaun();
    //回主页
    //读取文件夹
    void readfloadimage();
    //读取json，返回地址
    QString readjsonreturnpath(QString key);
    //校正
       void dojiaozheng(QString imagename,QString outimagepath);
    //
       void huifu();

      void changecolor(int i,QString iconname);
            void changecolor2();

   void backhome();
   void readfolder();
   void checksize();
private slots:
       void on_pushButton_clicked();
private:
    Ui::imagewidget *ui;
    //动画窗口
    TranslationWidget *translationwidget;
    QString  folderpath;
    QString waterfolderpath=nullptr;
    QPushButton *qpbopenfload;
    //存储去黑边文件目录
    QString quheibianfolderpath=nullptr;
    QPushButton *quheibianbutton;
    QPushButton *huanyaun;
    bool issreen=false;



};

#endif // IMAGEWIDGET_H
