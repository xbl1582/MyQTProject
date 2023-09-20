#ifndef MYLABEL_H
#define MYLABEL_H
#include <QLabel>
#include<QMouseEvent>


class Mylabel :public QLabel
{
    Q_OBJECT
public:
    explicit Mylabel(QLabel *parent = nullptr);
    void mousePressEvent(QMouseEvent* ev);
     void mouseReleaseEvent(QMouseEvent* ev);
     void enterEvent(QEvent*);
     void leaveEvent(QEvent*);

signals:
      void clicked();
      void changestyle();

};

#endif // MYLABEL_H
