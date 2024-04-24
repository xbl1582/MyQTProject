#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{

}

void MyWidget::mouseReleaseEvent(QMouseEvent *ev)
{
     emit clicked();
}
