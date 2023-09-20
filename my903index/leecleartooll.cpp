#include "leecleartooll.h"
#include <formclearfile.h>

leeCleartooll::leeCleartooll(QString t1,QStringList p1,QStringList f1,QString t2,QStringList p2,QStringList f2,QString t3,QStringList p3,QStringList f3)
{
    this->title1=t1;
    this->title2=t2;
    this->title3=t3;
    this->path1=p1;
    this->path2=p2;
    this->path3=p3;

    this->f1=f1;
    this->f2=f2;
    this->f3=f3;
}
void leeCleartooll::openWindow()
{
    FormClearFile * lxwindow=new FormClearFile(nullptr,this->title1,this->path1,this->f1,this->title2,this->path2,this->f2,this->title3,this->path3,this->f3);
    lxwindow->showFullScreen();
//    lxwindow->show();
    return;
}
