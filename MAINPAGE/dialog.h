#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr,QString title="");
    ~Dialog();

private slots:
    void on_pushButtonOK_clicked();

    void on_pushButton_Cancel_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
