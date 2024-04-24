#ifndef DIALOG_H
#define DIALOG_H
#include "QTimer"
#include <QDialog>
#include"QDebug"
#include"QFile"
#include"QDir"
#include"updatewidget.h"
#include "QtGui/private/qzipreader_p.h"
#include "QtGui/private/qzipwriter_p.h"
#include "QMessageBox"
#include "QProcess"
#include"QJsonParseError"
#include"QJsonObject"
#include "QClipboard"
#include"QThread"
//#include "JlCompress.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    QTimer *timer;
     QTimer *timer2;
    /**
      * this widget is show update schedule
      *
     */
    UpdateWidget *updatewidget;
    int probarvalue=0;
    bool isok=false;
    int filenums=0;
    int folderindex=0;
    QStringList folderpath;
    bool isactive=true;
    QString apppath;
    QMessageBox *msgBox;
    QString updatepath;
    int filecount=0;
      bool myresult=false;
      bool showmessagefalg=true;
    /*create a function that it is use to init the procedure function.
     *this function name is call initupdateproject .
     * this function has a timer ,this timer function is monitor an folder that it is
     * store zip ,and then we are acorring the filename to update in APP folder project.
     *
     */
    bool initupdateproject();
    bool checkupdatefolder();
    bool unzip(QString zipPath, QString zipDir,int todo);
    bool copyfile(QString src, QString dst);
    void writelog(QString logpath,QString content);
    void zipnum();
    void checkfolde();
    void filenum(QString src);
    QString  readJsonfloadname(QString key);
    void showmessage(QStringList labels);
    void appfoldpath(QFileInfo file_info,QString result,int todo);
    QString  changename(QString foldname);
    int diguitime=0;

private:
    Ui::Dialog *ui;
};
#endif // DIALOG_H
