#include "dialog.h"
#include "ui_dialog.h"
#include <QRect>
#include <QDesktopWidget>

Dialog::Dialog(QWidget *parent,QString title) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    Qt::WindowFlags m_flags = windowFlags();
    setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
    ui->plainTextEdit->setPlainText(QString(title));
    QDesktopWidget* desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry();
    int x = (screen.width() - this->width()) / 2;
    int y = (screen.height() - this->height()) / 2;
    setGeometry(x, y, this->width(), this->height()); // 让窗口位于屏幕中心
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButtonOK_clicked()
{
    accept();
}

void Dialog::on_pushButton_Cancel_clicked()
{
    reject();
}
