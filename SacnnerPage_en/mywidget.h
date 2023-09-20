#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr);

signals:
    void clicked();
protected:
    virtual void mouseReleaseEvent(QMouseEvent * ev);

};

#endif // MYWIDGET_H
