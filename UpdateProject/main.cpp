#include "dialog.h"
/**

  */
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
//     QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    Dialog w;

    return a.exec();
}
