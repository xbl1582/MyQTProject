#include "mylabel.h"

Mylabel::Mylabel(QLabel *parent) : QLabel(parent)
{

}
void Mylabel::mousePressEvent(QMouseEvent *ev)
{
    if (ev != nullptr && ev->button() == Qt::LeftButton)
    {
        emit changestyle();    //emit signal
    }
}

void Mylabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev != nullptr && ev->button() == Qt::LeftButton)
    {
        emit clicked();    //emit signal
    }
}

void Mylabel::enterEvent(QEvent *)
{
    setCursor(QCursor(Qt::PointingHandCursor));
}

void Mylabel::leaveEvent(QEvent *)
{
    setCursor(QCursor(Qt::PointingHandCursor));
}
