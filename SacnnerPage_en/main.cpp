#include "scanner.h"
#include <QApplication>
#include<QScreen>
#include<QRect>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    scanner *w =new scanner();

//    w.showFullScreen();
    w->show();
    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
    w->screenwidth=screenRect.width();
    w->screenheight=screenRect.height();
    w->resize(w->screenwidth,w->screenheight);





    return a.exec();
}

