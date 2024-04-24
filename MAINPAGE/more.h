#ifndef MORE_H
#define MORE_H

#include <QWidget>
#include "systemsetting.h"
#include "apps.h"
#include "shop.h"
#include <QApplication>
#include<QScreen>
#include <sys/sysinfo.h>
#include<QBrush>
#include<QResizeEvent>
#include<empowerpojo.h>
#include <QUuid>
#include<QSettings>
#include<QString>
#include<QHttpMultiPart>
#include<QHttpPart>
#include<QDesktopWidget>
#include<QApplication>
#include<QFileSystemWatcher>
#include <QProcess>
#include "driverpojo.h"
//#include <JlCompress.h>
#include "apppathspojo.h"
namespace Ui {
class More;
}

class More : public QWidget
{
    Q_OBJECT

public:
    explicit More(QWidget *parent = nullptr);
    ~More();
    //根据选择的tab切换显示的widget
    void changewidget(int i);
    //添加事件
    //返回首页
    void reback();
    //显示IP配置
    void showIpchange();
    void initmore();


    //显示系统设置
    void  showsetting();
    //显示应用管理
    void  showinstallservice();
    //显示账号
    void showUser();
    //显示终端
    void ShowTreaim();
    //窗口变化
    void onResize(QSize size);
    //显示按钮
    void showButton(bool returnbutton,bool terbutton);
    //检查授权
    void  checkauthorization();
    //通过version.txt文件安装软件(手动安装)
    void retrievalapps();

    //读取软件目录大小
    QString size_human(const QString filePath);
    //转化目录大小
    quint64 getflodersize(const QString filePath);

    //读取ip配置信息
    void readconfigforIPService();
    //读取基础配置信息
    void readconfigforsystemsetting();
    //上传文件任务YYSN_XXXXXX_.txt
    void postFileTask(QString filepath, QString filename);
    //服务请求授权信息
    QList<EmpowerPoJo *>  getempower(QString filename);
    //请求服务器
    QJsonObject getfun(QString fullRequest);
    //json数据转QString
    QString getJsonobjQString(QString key, QJsonObject qjo);
    //json数据转Array
    QJsonArray getJsonobjArray(QString key, QJsonObject qjo);
    //json数据转int
    int getJsonObjInt(QString key, QJsonObject qjo);
    //检查ip是否在线
    bool IPLive(QString ip, int port, int timeout);
    //窗口内容刷新
    void reloadinstallservice();
    //下载文件
    bool getdownfun(QString fullrequest, QString fileName);
    //解压文件
    bool unzip(QString fileName);
    //拷贝文件
    int copyfile(QString orginfile, QString pathfile);
    //检查system是否执行成功
    int System_Check(int result);
    //显示提示窗口
    void showmessage(QString txt, QString shellname);
    //请求驱动，获取YYSN执行文件信息
    QList<Driverpojo *> getdriver(QString filename);
    //下载YYSN执行文件
    void downloadYYSN();

    //授权信息对象
    EmpowerPoJo *empowerpojo;
     QList<apppathspojo *>apppaths;
    QList<apppathspojo *> getapppaths();
    void selectappPaths();
private:
    Ui::More *ui;
    //安装页面
    InstallService *installService=nullptr;
    //基础配置
    SystemSetting *systemsetting=nullptr;
    //ip配置对象
    IPServicePoJo *ipservicepojo;
    //屏幕宽度
    int screenwidth;
    //屏幕高度
    int screenhight;
    //画布
    QPixmap pixmap;
    //当前语言
    QString lange;
    //设备类型
    int devicetype;
    //
    int active=0;
    //当前程序目录
    QString  appcurrentpath;
    //当前程序目录上层地址
    QString basepath;
    //父窗口大小
    QSize supersize;
    //目录列表
    QStringList folderlist;
    //版本列表
    QStringList versionpathlist;
    //数据库
    QSqlDatabase database;
    //sql对象
    QSqlQuery *sql_query;
    //ip配置文件
    QSettings *ipservicesetting;
    //基础配置文件
    QSettings *systemsettings;
    //旋转角度
    int rote;
    //设备类型
    int device;
    //创建数据库
    void  creatdatabase();
    //查询app列表
    QList<AppsVersion *> selectAppsversion(QString sql);
    //关闭数据库
    void closedatabase();
    //执行sql语句
    void todosql(QString sql);

signals:
    //语言切换信号
    void sendlange(QString,int);   //用来传递数据的信号
    //最新完整包信号
    void sendgetbestrmtapps(int);   //用来传递数据的信号
    //有最新包信号
    void sendhavenewtomainwindow(int active);
    //成功重新加载信号
    void successtomainwindow();
    //子类型重加载信号
    void commondappsreload();

private slots:
    void receivelange(QString lange,int type);   //接收传递过来的数据的槽
    void receivehavenew(int active);
    void recivesuccessforapps();
};

#endif // MORE_H
