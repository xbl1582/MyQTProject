#ifndef LOGITEM_H
#define LOGITEM_H

#include <QWidget>

namespace Ui {
class LogItem;
}

class LogItem : public QWidget
{
    Q_OBJECT

public:
    explicit LogItem(QWidget *parent = nullptr);
    ~LogItem();

private:
    Ui::LogItem *ui;
};

#endif // LOGITEM_H
