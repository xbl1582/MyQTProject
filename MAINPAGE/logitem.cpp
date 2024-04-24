#include "logitem.h"
#include "ui_logitem.h"

LogItem::LogItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogItem)
{
    ui->setupUi(this);
}

LogItem::~LogItem()
{
    delete ui;
}
