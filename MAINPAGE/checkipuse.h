#ifndef CHECKIPUSE_H
#define CHECKIPUSE_H
#include <QThread>
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>
class CheckIpUse : public QThread
{
 Q_OBJECT
public:

     explicit CheckIpUse();
    ~CheckIpUse();

protected:
    void run() override; //重写QThread类的虚函数，也是线程子类的入口函数
private:
    QString appcurrentpath;
    QString basepath;

    int System_Check(int result);
signals:
    void done(); //完成信号
    void reportData(QString data); //报告完成进度
};

#endif // CHECKIPUSE_H
