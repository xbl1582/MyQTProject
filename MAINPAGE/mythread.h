#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QWidget>
#include <QApplication>
#include<QScreen>
#include <QWidget>
#include <QSettings>
#include "QTcpSocket"
#include<QJsonParseError>
#include<QJsonObject>
#include<QJsonArray>
#include<QFile>
#include"QNetworkRequest"
#include"QNetworkAccessManager"
#include"QTimer"
#include "QObject"
#include"QNetworkReply"
#include "QDir"
#include "zipobj.h"
#include "QThread"
#include "QtConcurrent"
#include <QApplication>
#include"QSqlDatabase"
#include "QSqlQuery"
class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QString requesturl,Zipobj *zipobj);
    explicit MyThread();

    ~MyThread();
protected:
    void run() override; //重写QThread类的虚函数，也是线程子类的入口函数
private:
    //屏幕宽度
    int screenwidth;
    //屏幕高度
    int screenhight;
    QString label;
    QSettings *settings;//申明一个QSetting类函数
    QSettings *apps;
    //当前程序上一层目录地址
    QString basepath;
    //当前程序目录地址
    QString apppath;
    //进度条值
    int processvalue=0;
    //请求路径
    QString requesturl2;
    //压缩包对象
    Zipobj *zipobj2;
    //基础配置文件
    QSettings *systemsettings;//申明一个QSetting类函数
    //请求数据
    QJsonObject getfun(QString fullrequest);
    //Json数据转QString
    QString UtilFromJsonArrayToString(const QJsonArray &data);
    //下载
    bool download(QString request,Zipobj *zipobj);
    //解压包
    void  unzipackge(QString urltype ,QString ip,int port ,int devicetype,QString systemtype,QString path);
    //修改权限
    QByteArray  getFileMd5(QString filePath);
    //检查下载的压缩包是否完整
    bool checkzip(QString md5code,QString filepath);
    //下载压缩包
    bool getdownfun(QString fullrequest,QString fileName);
    //解压压缩包
    void myunzip(Zipobj *zipobj);



signals:
    void done(); //完成信号
    void reportProgress(int precent); //报告完成进度

};

#endif // MYTHREAD_H
