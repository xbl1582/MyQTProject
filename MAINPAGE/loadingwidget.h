#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include <QWidget>
#include "QLabel"
#include "appsversion.h"
#include"QSettings"
#include "QScreen"
#include "QTime"
#include "QProcess"
#include "QTimer"
#include "QDebug"
#include "QFile"
#include "QMessageBox"
namespace Ui {
class loadingwidget;
}

class loadingwidget : public QWidget
{
    Q_OBJECT

public:
    explicit loadingwidget(QWidget *parent = nullptr);
//    explicit loadingwidget(QWidget *parent = nullptr, );
    ~loadingwidget();
    void setappversion(AppsVersion *appversion);
    QString bgimagepath;
    int screenwidth=0;
    int screenhight=0;
    QString doworkspace;
    QString doshellcode;
    QLabel *ql;
    QTimer *timer;
    QTime *time;
    QString lange="cn";
    bool open=true;
    QPixmap pixmap;
    //time
    int m_nBeginCount;
    QString imagepath2;
    QSettings *systemsettings;//申明一个QSetting类函数
    QString  appcurrentpath;
    QString basepath;

    AppsVersion *myappversion;
     QMessageBox *msgBox;
    int rote;
    int device;
    void  initWidge(QString imagepath);
    void mouseReleaseEvent(QMouseEvent * ev);
    void timeoutShow();
    void BeginCountdown();
    void OnTimerCountdown();
    void changelanguage(QString lange);
    void changebg(QString imagepath);
    void generscript();

    void readconfigforsystemsetting();
    int System_Check(int result);
    void errormassage(QString txt);
signals:
    void clicked();

public slots:
    void mouseClicked();
private:
    Ui::loadingwidget *ui;
};

#endif // LOADINGWIDGET_H
