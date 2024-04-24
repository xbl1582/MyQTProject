#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "QTcpSocket"
#include <QMainWindow>
#include "applist.h"
#include "QDebug"
//
#include"QNetworkRequest"
#include"QNetworkAccessManager"
#include "QTimer"
#include "QProcess"
#include "QMessageBox"
#include "QSettings"
#include "ipservicepojo.h"
#include <QApplication>
#include<QScreen>
#include <sys/sysinfo.h>
#include<QBrush>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include<QJsonParseError>
#include<QJsonObject>
#include<QJsonArray>
#include<QDialogButtonBox>
#include"QLineEdit"
#include "more.h"
#include "QNetworkAddressEntry"
#include "QHostInfo"
#include "QTableWidget"
#include "QPushButton"
#include "appsversion.h"
#include "loadingwidget.h"
#include "checkipuse.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //1.初始化界初始化界面
    void  initMainWindow();
    //2.窗口重新加载
    void reloadMainWindow();
    //3.修改设备类型
    void changeDrivertype();
    //根据下载购买设备的编号码适配功能项-激活码
    QString readActivationCode();
    //4.读取ip配置文件
    void  readconfigforIPService();
    //5.读取设备基础设置配置文件
    void  readconfigforsystemsetting();
    //读取系统安装有哪些程序--数据库操作
    //6.数据库操作
    void  creatdatabase();
    //7.sql执行
    void todosql(QString sql);
    //8.创建表格
    void creattable(QString sql);
    //9.关闭数据库
    void closedatabase();
    //10.开始计时器
    void BeginCountdown();
    //11.计时器到时退出执行
    void OnTimerCountdown();
    //12.根据安装的软件生成窗口
    QList<QWidget *> generateWindow();
    //13.修改语言
    void changelange(QString lange);
    //日志
    //检查容量大小
    //14.显示更多功能
    void showMoreFun();
    //15.计时器检查网络
    bool chackInternet(QString ip, int port, int timeout);
    bool IPLive(QString ip, int port,  int timeout = 1000);
    bool IsWebOk();
    //16.检查硬盘存储容量
    void checkmemory();
    //17.设置背景图片
    void setbackground(QString path);
    //18.存储容量按钮点击事件
    void memorybutton(QDialog *d);
    //19.读取json数据封装
    QString readJsonfloadname(QString key);
    //20.显示ip设置窗口
    void ShowInputBox();
    //21.读取ip设置脚本里的ip地址
    void  readeth();
    //22.显示被未使用的ip地址
    void  showuseip();
    //23.查询app列表
    QList<AppsVersion *>selectAppsversion();
    //24.生成软件启动脚本
    bool generateStartupScript(AppsVersion *apps);
    //25.执行软件启动脚本
    void todoshell(QString shellname,AppsVersion *appsversion);
    //26.检查system是否执行成功
    int  System_Check(int result);
    //27.显示警告弹窗
    void showmessage(QString txt,QString shellname);
    //28.警告弹窗
    void showmessage(QString txt,AppsVersion *appsversion);

    //29.解压上传
    void unzipAndUploadBackground(QDir qdir, QString floderpath, QString resultpath,QString shellname);
    void unzipAndUploadBackground(QDir qdir, QString floderpath, QString resultpath,AppsVersion *appsversion);

    //30.判断是否开启扫描
    bool isopenscann();
    //31.检查被未使用的ip地址
    void checkhavenotuseip();
    QString getIpnousedata();
    void setIpnousedata(const QString &value);

private:
    Ui::MainWindow *ui;
    APPList *applist=nullptr;

    //程序当前位置
    QString appcurrentpath;
    //存储容量
    QString  memory;
    //IP地址+
    QString ipv4;
    QString ipv42;
    QString ipv43;
    QString ipv44;
    //语言
    QString lange="cn";
    //程序上一层路径
    QString basepath;
    //未被使用的ip地址
    QString ipnousedata="";

    //弹窗
    QWidget *qwidget;
    //读取软件安装内容
    //显示更多功能
    More *more;
    //信号计时器
    QTimer *signaltimer;
    //存储容量定时器
    QTimer *tr2;
    //是否显示容量警告弹窗
    bool isshowmsg=false;
    //显示警告框
    bool isshowmessage=true;
    QMessageBox *msgBox;
    //读取ip配置文件
    QSettings *ipservicesetting;
    IPServicePoJo *ipservicepojo;
    //设备类型
    int drivertype=1;
    //屏幕宽度
    int screenwidth;
    //屏幕高度
    int screenhight;
    //设备类型
    int devicetype=1;
    //app数量
    int appcount=0;
    //屏幕旋转角度
    int rote=0;
    //设置类型
    int device=0;
    //标识
    int active=0;
    int showmsgtime=0;
    //画布，设置背景图片
    QPixmap pixmap;
    //数据库对象
    QSqlDatabase database;
    //sql对象
    QSqlQuery *sql_query;
    QSettings *systemsettings;//申明一个QSetting类函数
    //存储app项数组
    QList<QWidget *> qflist;


     //加载页
    loadingwidget *myloadingwidget=nullptr;
    //检查ip线程
    CheckIpUse *checkIpUse;
    //网格布局
    QGridLayout *qgr;


    //
private slots:
    void receivelange(QString lange,int type);   //接收传递过来的数据的槽
    void receivehavenew(int active);
    void recivesuccessformore();
    void onResize();
protected:
    void resizeEvent(QResizeEvent *event);
signals:
    void resized();
};
#endif // MAINWINDOW_H
