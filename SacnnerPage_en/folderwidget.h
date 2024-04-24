#ifndef FOLDERWIDGET_H
#define FOLDERWIDGET_H
//#define MYJSONPATH ":/json/pathjson.json"
#include <QWidget>
#include<mylabel.h>
#include<QPushButton>
#include<mywidget.h>
#include<QGridLayout>
#include<QScrollArea>
#include<QListWidget>
#include<translationwidget.h>
#include<QJsonParseError>
#include<QJsonObject>
#include<QJsonArray>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include<QMessageBox>
#include<capturescreen.h>
#include "QTimer"
#include<QPalette>
#include<QPixmap>
#include <QApplication>
#include<QScreen>
#include<mylabel.h>
#include<QFileDialog>
#include<QDebug>
#include<QPushButton>
#include<mywidget.h>
#include<QFile>
#include<QDir>
#include<QScrollArea>
#include<QScrollBar>
#include<QTimer>
#include<translationwidget.h>
#include<QListWidget>
#include<QFormLayout>
#include<QDialogButtonBox>
#include<QLineEdit>
#include<math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include<QDateTime>
#include<QByteArray>
#include<QSettings>
#include"QDirIterator"
#include "QRegExp"
namespace Ui {
class FolderWidget;
}

class FolderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FolderWidget(QWidget *parent = nullptr);
    ~FolderWidget();
    int screenwidth;
    int screenheight;
    Mylabel *folderlabel;
    QString folderpath;
    QString oldfolderpath;
    int curreindex=0;
    int m_sollarsider=0;
    QScrollArea *qsa;
    QString folderpath2;
    int page=0;
    int pagesize=0;
    QStringList imagelist;
    capturescreen *cs;
    QPushButton *qpbopenfload;
    QTimer *timer2;
    Mylabel *filename;
    QList<QPushButton *>buttons;
    //初始化文件夹
    void initfolder();
    QString readjsonreturnpath(QString key);
     void suaxing();



protected:
    //初始化目录页面
    void initFolderWidget();

    //读取文件夹
    void readfolder();
    //递归图片文件
    void readImageFile();
    //懒加载预览
    void loadingimage( QScrollBar *qsb);


    void initwidget(QWidget *q,int minwidth,int maxwidth,int minheight,int maxheight,QString style);

    //封装方法
    void intilabel(QWidget *q,Mylabel *label,QString path,int imagewith,int imageheight,int maginw,int maginh,int minwidth,int maxwidth,int minheight,int maxheight);

    //上一页
    void changeUpPage();

    //下一页
    void changeNextPage();
    //添加水印
    void addwaterimage();
    //生成水印图
    void  watermarkImage(QString watercontent,QString imagepath);
    //点击列项
   //返回上一层
    void closefloader();
    //去黑边
    void  quheibian();
    //全选
    void quanxuan();
    //取消选择
    void quxiaoxuanze();
    //删除
    void deleteimage();
     void deleteimage2();
     void quxiaoxuanze2();
    //生成按钮
    void shengchengbutton(QStringList  buttontitlearrays,QStringList buttoniconarrays);
    void openfolder2();
     void openfolder3();
     void checkisemty();
     void daochu();
     void changeChooseColor();

private:
    Ui::FolderWidget *ui;
     QPushButton *qpb;
     QStringList string_list;
     QStringList choose_list;
     QPixmap *qpixmap;
     Mylabel *sonlabel;
     MyWidget *filewidget;
     MyWidget *showimagewidget;
     Mylabel *label;
     QString waterpathtemp;
      QString  waterfolderpath;

     bool flag=false;
     bool flag2=true;
     int allpage=0;
     int  currentpage=0;

     int filelistwidgetheight=490;
     int allimage;
     int choose;
     QGridLayout *filegridlayout;
     TranslationWidget *trw;
     QList<QWidget *>qlist;
    QListWidget *temlistwidget;

    QPushButton *quheibianbutton;
    QString quheibianfolderpath=nullptr;



       QIcon icon;
private slots:
       void on_pushButton_clicked();
       void clickImage(QListWidgetItem *item);
       void clickImage(QListWidgetItem *item,int allchoose);


};

#endif // FOLDERWIDGET_H
