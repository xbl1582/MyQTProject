#ifndef FORMCLEARFILE_H
#define FORMCLEARFILE_H

#include <QWidget>
#include <QList>
#include <QStringList>
#include  <listfilecontrol.h>
#include<QDebug>
namespace Ui {
class FormClearFile;
}

class FormClearFile : public QWidget
{
    Q_OBJECT

public:
    explicit FormClearFile(QWidget *parent = nullptr,QString t1="",QStringList p1={},QStringList f1={},QString t2="",QStringList p2={},QStringList f2={},QString t3="",QStringList p3={},QStringList f3={});
    ~FormClearFile();

private slots:
    void on_pushButton_clicked();

private:
    Ui::FormClearFile *ui;
    QString title1,title2,title3;
    QStringList path1,path2,path3,f1,f2,f3;
    ListFileControl *l1, *l2,*l3;

    QTimer *timer;
    QProcess *process;
    void checkfoldermemory();
    QString checkmemory(QString shellpath,QString shellname,QString filepath);

};

#endif // FORMCLEARFILE_H
