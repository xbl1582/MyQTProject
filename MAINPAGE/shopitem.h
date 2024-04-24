#ifndef SHOPITEM_H
#define SHOPITEM_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include<mythread.h>
#include<QMessageBox>
#include<QFont>
#include"QDesktopWidget"
#include"QApplication"
namespace Ui {
class ShopItem;
}

class ShopItem : public QWidget
{
    Q_OBJECT

public:
    explicit ShopItem(QWidget *parent = nullptr);
    ~ShopItem();
    //初始化商店子项
    void iniShopItem();
    //初始化商店子项请求路径
    void iniShopItempath(QString requestur,Zipobj *zipobjt);
    //安装应用
    void installItem(QString requestur,Zipobj *zipobj);
    //修改安装列项logo图片
    void changelogo(QString path);
    //修改安装列项应用名称
    void changeappname(QString name);
    //修改安装列项的版本号
    void changeappversion(QString name);
    //修改安装列项的当前版本时间
    void changeappupdatedtime(QString name);
    //修改安装列项的更新时间
    QString changeupdatetime2(QString time);
    //修改安装列项的描述
    void changeappdesc();
    //修改安装列项安装包大小
    void changeappsize(QString size);
    //修改安装按钮的状态
    void changepushbuttonstate(bool visiable,bool buttondisable,QString txt);
    //启动下载进程
    void runThread(QString requestur,Zipobj *zipobj);
    //关闭数据库连接
    void closedatabase();
    //修改应用名称
    QString changeappname2(QString appname);
private:
    //下载线程
    MyThread *thread;
    //请求路径
    QString requestur;
    //下载包对象
    Zipobj *zipobj;
    //数据库对象
    QSqlDatabase database;
    //sql对象
    QSqlQuery *sql_query;
    //当前应用所处位置
    QString appcurrentpath;
    //当前应用上层所处位置
    QString basepath;
    //路径配置文件
    QSettings *savepathsetting;
    //窗口宽度
    int screenwidth;
    //读取系统安装有哪些程序--数据库操作
    void  creatdatabase();
    //sql执行
    void todosql(QString sql);

private:
    Ui::ShopItem *ui;
signals:
    //安装成功发送信号给shop页
    void insertsuccesstoshop();
};

#endif // SHOPITEM_H
