#ifndef APPSITEM_H
#define APPSITEM_H
#include "zipobj.h"
#include <QWidget>
#include "mythread.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include<mythread.h>
#include "appsversion.h"
#include "QMessageBox"
#include "appsiteminfo.h"
#include "ipservicepojo.h"
#include"QDesktopWidget"
#include"QApplication"
#include"apppathspojo.h"
namespace Ui {
class appsitem;
}

class appsitem : public QWidget
{
    Q_OBJECT

public:
    explicit appsitem(QWidget *parent = nullptr);
    ~appsitem();

    void iniAppsItem();
    void iniAPPsItempath(QString requestur,Zipobj *zipobjt);
    void installItem(QString requestur,Zipobj *zipobj);
    void changelogo(QString path);
    void changeappname(QString name);
    void changeappversionAndappupdatedtime(QString name);
    void changertmversionAndaddrtmtime(QString name);
    void changeappnewversionAndappnewupdatedtime(QString name);
    void changeappdesc( QString desc);
    void changeappsize(QString size);
    void changepushbuttonstate(bool visiable,bool buttondisable,QString txt);
    void runThread(QString requestur,Zipobj *zipobj);
    void closedatabase();
    bool uninstall(AppsVersion *appsversion);
    void getappversion(AppsVersion *myappsversion);
    void  showappsiteminfo(AppsVersion *appsversion);

    void changeuninstalldisable();
    void readconfigforIPService();
    QString readJsonfloadname(QString key, QString jsonpath);
    int getJsonObjInt(QString key, QJsonObject qjo);
    QJsonArray getJsonobjArray(QString key, QJsonObject qjo);
    QString getJsonobjQString(QString key, QJsonObject qjo);
    QJsonObject getfun(QString fullRequest);
    bool IPLive(QString ip, int port, int timeout);
    void getbestnewapprtm(QString path);
    AppsVersion* selectAppsversion(QString path);
    QString changedatelabe(int date);
    void getempower(QString filename);
    void resizeappitem(QSize size);
    QList<apppathspojo *> getapppaths();

    void selectappPaths();
private:
    Ui::appsitem *ui;
    MyThread *thread;
     MyThread *thread2;
    QString requestur;
    QString requestrtm;
    Zipobj *zipobj;
    AppsVersion *appsversion;
    //数据库对象
    QSqlDatabase database;
    //sql对象
    QSqlQuery *sql_query;
    QString appcurrentpath;
    QMessageBox *msgBox;

    AppsItemInfo *appsiteminfo=nullptr;

    QSettings *savepathsetting;

    QSettings *ipservicesetting ;

    IPServicePoJo *ipservicepojo;

    bool checkupdated=false;

    QString  getbestnewupdatedapp;
    QString   getbestnewupdatedapptype;

    int screenwidth;
    int screenhight;
     QString getbeastnewrtmapp;

     QList<Zipobj *> *rtmlist;
     QString basepath;
     QList<apppathspojo *>apppaths;
    //读取系统安装有哪些程序--数据库操作
    void  creatdatabase();
    //sql执行
    void  todosql(QString sql);
    bool deletesql(QString sql);
    //显示软件占用情况
    void showappsize();

    QString getappsize();

    quint64 getflodersize(const QString filePath);
    QString size_human(const QString filePath);

    void checkupdatedfun(AppsVersion *appversion);




signals:
    void sendToappAndloadapp();

};

#endif // APPSITEM_H
