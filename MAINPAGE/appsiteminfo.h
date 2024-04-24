#ifndef APPSITEMINFO_H
#define APPSITEMINFO_H

#include <QWidget>
#include "appsversion.h"
#include "QSettings"
#include <QApplication>
#include<QScreen>
#include <QFile>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "QDebug"
#include "QFileInfoList"
#include "dialog.h"
#include <QApplication>
namespace Ui {
class AppsItemInfo;
}

class AppsItemInfo : public QWidget
{
    Q_OBJECT

public:
    explicit AppsItemInfo(QWidget *parent = nullptr);
    ~AppsItemInfo();

    void initAppsItemInfo(AppsVersion *appsversion,QStringList fPath={},QStringList f={});

    void readconfigforsystemsetting();
    void creatdatabase();
    QString size_human(const QString filePath);
    quint64 getflodersize(const QString filePath);
    void todosql(QString sql);
    void closedatabase();
    void checkmemory();
    void loadFile();
    void changelabel(QString label);
    QString changeappname(QString appname);
private:
    Ui::AppsItemInfo *ui;
     QString appcurrentpath,basepath;
     QSettings *systemsettings;
     int rote,device;
     QString lange;
     int screenwidth;

     int screenhight;
     //数据库对象
     QSqlDatabase database;
     //sql对象
      QSqlQuery *sql_query;
      QString memory;

      QList<QString> lj,lt,lz,lb,lo;

      QString titleString;
      QStringList filePath,file;
      QString dialogtitle;
;
      QFileInfoList GetFileList(QString path);
      void on_pushButton_delTif_clicked();

      void on_pushButton_deljpg_clicked();

      void on_pushButton_delBMP_clicked();

      void on_pushButton_delZIP_clicked();

      void on_pushButton_delother_clicked();

      void on_pushButton_Del_clicked();
};

#endif // APPSITEMINFO_H
