#ifndef APP_H
#define APP_H

#include <QWidget>
#include <QApplication>
#include<QScreen>
#include <sys/sysinfo.h>
#include<QBrush>
#include"QLabel"
#include "QVBoxLayout"
#include "shop.h"
#include "appsitem.h"
#include "logitem.h"
#include "QMovie"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
namespace Ui {
class App;
}

class App : public QWidget
{
    Q_OBJECT

public:
    explicit App(QWidget *parent = nullptr);
    ~App();
    void initapp();
    void getappitem();
    void  getbestnewappupdated();
    void  readconfigforIPService();
    void readconfigforSystem();


    int getJsonObjInt(QString key, QJsonObject qjo);
    QJsonArray getJsonobjArray(QString key, QJsonObject qjo);
       QString readJsonfloadname(QString key, QString jsonpath);
    QString getJsonobjQString(QString key, QJsonObject qjo);
    QJsonObject getfun(QString fullRequest);

    QString changedatelabe(int date);
    void onResize(QSize size);
private:
    int screenwidth;
    int screenhight;
    IPServicePoJo *ipservicepojo;
    QSettings *ipservicesetting;
    QList<Zipobj *> list;
    //数据库对象
    QSqlDatabase database;
    //sql对象
    QSqlQuery *sql_query;
    QString  appcurrentpath;
    QTimer *timer;
    int timecount=1;
    QMovie *movie ;
    //系统基础设置
    QString lange="cn";
    int device=1;
    int rote=0;

    QString dowonloadpath;
    QString dowonloadpathtype;
    QString getbeastnewrtmapp;
    QString getbeastnewrtmapptype;
    QString getbestnewupdatedapp;
    QString getbestnewupdatedapptype;
    QString updatederrorpath;
    QString updatederrorpathtype;
    QString gettododatapath;
    QString gettododatapathtype;
    QSettings *systemsettings;
    QPixmap pixmap ;
    QList<QWidget * >appsitemlist;
    bool issend=false;


    //读取系统安装有哪些程序--数据库操作
    void  creatdatabase();
    //sql执行
    void todosql(QString sql);
    void closedatabase();
    QList<AppsVersion *>  selectAppsversion();
    bool IPLive(QString ip, int port, int timeout) ;
    QString changeappname(QString appname);


private:
    Ui::App *ui;

signals:
   void sendhavenewtoapps(int active);   //用来传递数据的信号
   void sendToappsAndreload();

private slots:
     void onTimeout();
     void recivereAppitemInfo();



};

#endif // APP_H
