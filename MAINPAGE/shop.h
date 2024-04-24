#ifndef SHOP_H
#define SHOP_H

#include <QWidget>
#include <QApplication>
#include<QScreen>
#include <sys/sysinfo.h>
#include<QBrush>
#include "shopitem.h"
#include"QLabel"
#include "QVBoxLayout"
#include "httpclient.h"
#include "QDebug"
#include "ipservicepojo.h"
#include "QFile"
#include "QSettings"
#include "QMovie"
#include <QTimer>
#include "QJsonArray"
#include "zipobj.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "appsversion.h"
namespace Ui {
class Shop;
}

class Shop : public QWidget
{
    Q_OBJECT

public:
    explicit Shop(QWidget *parent = nullptr);
    ~Shop();
    //初始化商城界面
    void  initshop();
    //获取安装列项
    void getshopitem();
    //请求服务器最新的完整包
    void  getbestnewapprtm();
    //读取Ip配置文件内容
    void  readconfigforIPService();
    //读取基础设置配置内容
    void readconfigforSystem();
    //将json数据转为int
    int getJsonObjInt(QString key, QJsonObject qjo);
    //将json数据转为array
    QJsonArray getJsonobjArray(QString key, QJsonObject qjo);
    //将json数据转为QString
    QString getJsonobjQString(QString key, QJsonObject qjo);
    //向服务器请求数据
    QJsonObject getfun(QString fullRequest);
    //测试ip是否连通
    bool IPLive(QString ip, int port, int timeout);
    //窗口大小修改事件
    void onResize(QSize size);
private:
    Ui::Shop *ui;
    //屏幕宽度
    int screenwidth;
    //屏幕高度
    int screenhight;
    //ip设置对象
    IPServicePoJo *ipservicepojo;
    //ip配置文件
    QSettings *ipservicesetting;
    //基础配置文件
    QSettings *systemsettings;
    //当前程序位置
    QString appcurrentpath;
    //api
    //下载api
    QString dowonloadpath;
    //下载api 类型
    QString dowonloadpathtype;
    //请求最新完整包api
    QString getbeastnewrtmapp;
    //请求最新完整包api 请求类型
    QString getbeastnewrtmapptype;
    //请求最新更新包api
    QString getbestnewupdatedapp;
    //请求最新更新包api 请求类型
    QString getbestnewupdatedapptype;
    QString updatederrorpath;
    QString updatederrorpathtype;
    QString gettododatapath;
    QString gettododatapathtype;
    //加载计时器
    QTimer *timer;
    //加载计时器倒数1秒
    int timecount=1;
    //加载动画
    QMovie *movie ;
    //下载安装包列项
    QList<Zipobj *> list;
    //数据库对象
    QSqlDatabase database;
    //sql对象
    QSqlQuery sql_query;

    //系统基础设置
    //语言
    QString lange="cn";
    //类型
    int device=1;
    //旋转角度
    int rote=0;
    //读取json数据，通过key
    QString readJsonfloadname(QString key, QString jsonpath);
    //读取系统安装有哪些程序--数据库操作
    void  creatdatabase();
    //sql执行
    void todosql(QString sql);
    //select 根据path查询本地数据库存储
    AppsVersion* selectAppsversion(QString path);
    //安装列项
    QList<QWidget *>qlist;
signals:
    //安装成功，发送信号给apps
    void successtoapps();
private slots:
    void onTimeout();
    //接收安装列项发送来的信号
    void reciveinsertsuccessforshopitem();
};

#endif // SHOP_H
