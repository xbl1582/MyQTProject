#ifndef UPDATEWIDGET_H
#define UPDATEWIDGET_H

#include <QWidget>
#include <QApplication>
#include<QScreen>
namespace Ui {
class UpdateWidget;
}

class UpdateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UpdateWidget(QWidget *parent = nullptr);
    ~UpdateWidget();
    bool changeprogressbarvalue(int value);
    bool changeisok();
    bool setbarmaxnum(int value);

private:
    Ui::UpdateWidget *ui;
    int screenwidth;
    int screenhight;
    bool isok;
};

#endif // UPDATEWIDGET_H
