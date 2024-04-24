#ifndef SYSTEMSETTING_H
#define SYSTEMSETTING_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QWidget>
#include <QApplication>
#include<QScreen>
#include <sys/sysinfo.h>
#include<QBrush>
#include "QDebug"
#include "QButtonGroup"
#include "QSettings"
#include "QMessageBox"
#include"QNetworkRequest"
#include"QNetworkAccessManager"
#include "QTcpSocket"
#include"QTranslator"
#include"QFile"
#include "QDir"
#include "empowerpojo.h"
#include "ipservicepojo.h"
#include"QJsonParseError"
#include"QJsonObject"
#include"QEventLoop"
#include"QNetworkReply"
#include"QJsonArray"
#include"QHttpMultiPart"
#include"appsversion.h"
#include"QTimer"
#include"QDesktopWidget"
#include"driverpojo.h"
namespace Ui {
class SystemSetting;
}

class SystemSetting : public QWidget
{
    Q_OBJECT

public:
    explicit SystemSetting(QWidget *parent = nullptr);
    ~SystemSetting();
    void  initsystemsetting();
    void creatdatabase();
    //sql执行
    void todosql(QString sql);
    //保存系统设置
    void  savesystemsetting();

    void changesetshow();
    void onResize(QSize size);




    void checkauthorization();
    QList<EmpowerPoJo *> getempower(QString filename);
    void readconfigforIPService();
    QJsonObject getfun(QString fullRequest);
    QString readJsonfloadname(QString key, QString jsonpath);
    int getJsonObjInt(QString key, QJsonObject qjo);
    QJsonArray getJsonobjArray(QString key, QJsonObject qjo);
    QString getJsonobjQString(QString key, QJsonObject qjo);
    void postFileTask(QString filepath, QString filename);
    int System_Check(int result);
    void showmessage(QString txt, QString shellname);
    bool getdownfun(QString fullrequest, QString fileName);
    QList<AppsVersion *> selectAppsversion(QString sql);
    bool unzip(QString fileName);
    int copyfile(QString orginfile, QString pathfile);
    QList<Driverpojo *> getdriver(QString filename);
    void downloadYYSN();
private:
    Ui::SystemSetting *ui;
    //屏幕宽度
    int screenwidth;
    // 屏幕高度
    int screenhight;
    //数据库对象
    QSqlDatabase database;
    //sql对象
    QSqlQuery *sql_query;

    //程序当前位置
    QString appcurrentpath;
    //程序上层位置
    QString  basepath;
    //按钮组
    QButtonGroup *bgGroup;

    //系统基础设置
    //语言设置
    QString lange="cn";
    //软件类型
    int device=1;
    //屏幕旋转角度
    int rote=0;
    //系统基本配置
    QSettings *systemsettings;//申明一个QSetting类函数

    //服务配置
    QSettings *servicesettings;//申明一个QSetting类函数

    //Ip配置文件
    QSettings *ipservicesetting;
    //IP配置对应对象
     IPServicePoJo *ipservicepojo;
    //设备类型
    int devicetype;
    //ip地址
    QString  ip;
    //url地址
    QString  outurl;
    //端口号
    int port;
    //系统类型
    QString systemtype;
    //请求协议类型
    QString  urltype;
    QTranslator *m_translator;
    //是否是管理员，1是，0否
    int admin;
    //对话弹窗
    QMessageBox   * msgBox;
    //下载api
    QString dowonloadpath;
    //下载api请求方式
    QString dowonloadpathtype;
     //获取最新完整包版本api
    QString getbeastnewrtmapp;
     //获取最新完整包版本api请求方式
    QString getbeastnewrtmapptype;
     //获取最新更新包版本api
    QString getbestnewupdatedapp;
    //获取最新更新包版本api请求方式
    QString getbestnewupdatedapptype;
    QString updatederrorpath;
    QString updatederrorpathtype;
    QString gettododatapath;
    QString gettododatapathtype;
    QString updatedpath;
    QString updatedpathtype;


private:
    //检查网络
    int chackInternet(QString ip, int port, int timeout);
    bool IPLive(QString ip, int port,  int timeout = 1000);
    bool IsWebOk();
    //切换语言
    void changelanguage();
    //显示终端
    void ShowTreaim();
    //测试网络连接
    void testInternet();
signals:
    //发送语言切换信号
    void sendlange(QString,int );   //用来传递数据的信号
private slots:
    void slot_RtnGroupChanged(int id);
    void on_comboBox_currentIndexChanged(const QString &arg1);
};

#endif // SYSTEMSETTING_H
