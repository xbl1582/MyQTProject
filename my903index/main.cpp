#include "form.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Form form;

    form.resize(form.screenwidth,form.screenhight);
        form.setFixedSize(form.screenwidth,form.screenhight);
        form.setMinimumSize(form.screenwidth,form.screenhight);
        form.setMaximumSize(form.screenwidth,form.screenhight);
    form.show();

    return a.exec();
}
