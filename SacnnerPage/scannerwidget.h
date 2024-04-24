#ifndef SCANNERWIDGET_H
#define SCANNERWIDGET_H
//#define MYJSONPATH ":/json/pathjson.json"
#include <QWidget>
#include<QMovie>
#include<QDebug>
#include<QScreen>
#include<QJsonParseError>
#include<QFile>
#include<QDir>
#include<QDateTime>
#include<QLabel>
#include<QProcess>
#include<QPainter>
namespace Ui {
class scannerwidget;
}

class scannerwidget : public QWidget
{
    Q_OBJECT

public:
    explicit scannerwidget(QWidget *parent = nullptr);
    ~scannerwidget();
    void initscannerwidget();
    int readfloadimage();
    void movefiletoresult();
    QString readpath(QString key);
    void paintEvent(QPaintEvent *);

    int screenwidth;
    int screenheight;
private:
    Ui::scannerwidget *ui;
};

#endif // SCANNERWIDGET_H
