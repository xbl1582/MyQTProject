#ifndef SCANNER_H
#define SCANNER_H
//#define MYJSONPATH ":/json/pathjson.json"
#include <QWidget>
#include<mylabel.h>
#include<mywidget.h>
#include<QTime>
#include<QTimer>
#include<imagewidget.h>
#include<scannerwidget.h>
#include <QApplication>
#include<QScreen>
#include<QFontDatabase>
#include<QPixmap>
#include<QDebug>
#include<imagewidget.h>
#include<QDir>
#include<QMovie>
#include<QFileDialog>
#include<QSettings>
#include<QThread>
#include<QTranslator>
QT_BEGIN_NAMESPACE
namespace Ui { class scanner; }
QT_END_NAMESPACE

class scanner : public QWidget
{
    Q_OBJECT

public:
    scanner(QWidget *parent = nullptr);
    ~scanner();
    QLabel *timelabel;
    Mylabel *numlabel;
    MyWidget *numWidget;

    Mylabel *title2;
     Mylabel *title;
     Mylabel *title3;
     Mylabel *title4;

   MyWidget *scannerWidget;
    Mylabel *scannerlabel;

    MyWidget *imagelWidget;
    Mylabel *imagelabel;

    MyWidget *settingWidget;
    Mylabel *settinglabel;

   MyWidget *outWidget;
    Mylabel *outlabel;

    //子页面
    imagewidget *sunimagewidget;
    scannerwidget *sunscannerwidget;


    int screenwidth;
    int screenheight;
    QPixmap *qpixmap;
    //计时器
    QTimer *timer;
    QTimer *timer2;
     QTimer *timer3;
      QTimer *timer4;
    QTime *time;
    bool m_showColon;       //显示 " : " 的闪动
    int m_nBeginCount;
    QWidget *loading;
    QMovie *movie;
    //完成扫描
    bool isfished;
    //jiazhai
    bool falg=true;
    //文件夹路径
    QString folderpath;
    QThread *thread;
    QString memory="0G";
    QMessageBox * msgBox;
    QTranslator *m_translator;
      void checkscannfinsh();
    //初始化页面
    void initScannerIndexPage();
    //封装方法
    void intilabel(QWidget *q,Mylabel *label,QString path,int imagewith,int imageheight,int maginw,int maginh,int minwidth,int maxwidth,int minheight,int maxheight);
    //封装widget
    void initwidget(QWidget *q,int minwidth,int maxwidth,int minheight,int maxheight,QString style);
    //点击事件函数
    //图片窗点击
    void imagewidgeclick();
    //扫描框点击
    void scannerwidgeclick();

    //设置框点击
    void settingwidgetclick();
    //退出框点击
    void outwidgetclick();
    //显示时间
    void showTime();
    //读取文件夹
    int readfloadimage();
      int readfloadimage2();
    //将文件进行拷贝
    void movefiletoresult();
    QString readJsonfloadname(QString key);
    void loadLanguage(QString type);


    int System_Check(int result);
private:
    Ui::scanner *ui;

};
#endif // SCANNER_H
