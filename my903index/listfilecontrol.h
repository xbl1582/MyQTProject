#ifndef LISTFILECONTROL_H
#define LISTFILECONTROL_H

#include <QWidget>
#include <QList>
#include <QFileInfo>
#include <QFileInfoList>
#include <QStringList>
#include<QTimer>
#include<QProcess>
#include<QTextStream>
namespace Ui {
class ListFileControl;
}

class ListFileControl : public QWidget
{
    Q_OBJECT

public:
    explicit ListFileControl(QWidget *parent = nullptr,QString title="", QStringList fPath={},QStringList f={});
    ~ListFileControl();
     void changelabel(QString label);

private slots:
    void on_pushButton_delTif_clicked();

    void on_pushButton_deljpg_clicked();

    void on_pushButton_delBMP_clicked();

    void on_pushButton_delZIP_clicked();

    void on_pushButton_delother_clicked();

    void on_pushButton_Del_clicked();


private:
    Ui::ListFileControl *ui;
    QList<QString> lj,lt,lz,lb,lo;

    QString titleString;
    QStringList filePath,file;

    void loadFile();
    QFileInfoList GetFileList(QString path);

};

#endif // LISTFILECONTROL_H
