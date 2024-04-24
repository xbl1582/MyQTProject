#ifndef INSTALLSERVICE_H
#define INSTALLSERVICE_H

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
#include "app.h"
#include "QScroller"
namespace Ui {
class InstallService;
}

class InstallService : public QWidget
{
    Q_OBJECT

public:
    explicit InstallService(QWidget *parent = nullptr);
    ~InstallService();
    void initinstallservice();
    void onResize(QSize size);



private:
    Ui::InstallService *ui;
    int screenwidth;
    int screenhight;
    Shop *shop=nullptr;
    App *app=nullptr;
    QPixmap pixmap;
    QSize supersize;

signals:
    void currentChanged(int);
    void sendhavenewtomore(int active);
    void successtomore();
private slots:
      void TabWidgetWhichTabIsDown(int );
      void receivehavenew(int active);
      void recivesuccessforshops();
      void recivereappsreload();




};

#endif // INSTALLSERVICE_H
