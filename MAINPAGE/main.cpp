#include "mainwindow.h"

#include <QApplication>
#include <QSharedMemory>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //1.获取程序当前目录
    QString appcurrentpath=QCoreApplication::applicationDirPath();
    MainWindow w;
    //2.程序窗口居中
    QDesktopWidget *desktop = QApplication::desktop();
    w.move((desktop->width() - w.width())/ 2, (desktop->height() - w.height()) /2);
    w.setWindowTitle("快选通应用管理");
    QApplication::setWindowIcon(QIcon(appcurrentpath+"/image/logo/KSTMAIN.ico"));
    w.setWindowIcon(QIcon(":/icon/KSTMAIN.ico"));
    w.show();

    //    QSharedMemory shared("KSTMAIN");
    //       if(shared.attach())//共享内存被占用则直接返回
    //       {
    ////           QMessageBox::information(NULL,QStringLiteral("Warning"),QStringLiteral("程序已经在运行!"));
    //           return 0;
    //       }
    //       shared.create(1);//共享内存没有被占用则创建U

    return a.exec();
}

