#ifndef LEECLEARTOOLL_H
#define LEECLEARTOOLL_H

#include <QString>
#include <QList>
#include <QStringList>


class  leeCleartooll
{
public:
    leeCleartooll(QString t1="",QStringList p1={},QStringList f1={},QString t2="",QStringList p2={},QStringList f2={},QString t3="",QStringList p3={},QStringList f3={});
     void openWindow();
private:
     QString title1,title2,title3;
     QString disk1,disk2,disk3;
     QStringList path1,path2,path3,f1,f2,f3;


};

#endif // LEECLEARTOOLL_H
