#ifndef FORM_H
#define FORM_H
//#define MYJSONPATH ":/json/mybuttons.json"
#include <QWidget>
#include<QLabel>
#include<QPushButton>
#include "translationwidget.h"
#include <QPushButton>
#include <QPalette>
#include<QPainter>
#include <QLabel>
#include<QtSvg/QSvgWidget>
#include<QtSvg/QSvgRenderer>
#include<QToolButton>
#include<mylabel.h>
#include <qloadingwidget.h>
#include<QTime>
#include<QTimer>
#include<QMessageBox>
#include<QComboBox>
#include<QTranslator>
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    int x=0;
    int y=0;
    int timer=0;
    int timel=0;
    int jsonbuttonsize=0;
    int jsonlabelsize=0;
    int nNewLoaction=0;
    int allpage=0;
    int currentpage=0;
    int screenwidth=0;
    int screenhight=0;
      QTranslator *m_translator;
    QString langue="cn";
    QPushButton *qpb2;
    int open=0;
    QLoadingWidget *qw;
    QTime *time;
    QTimer *timer2;
    QTimer *tr2;
    QString ipv4;
    QString ipv42;
    QPushButton *q3;
    QMessageBox *msgBox;
    bool isshowmessage=true;
    QString memory="0";
    bool  isstart=false;
    Mylabel *txt;
     QPixmap pixmap;
    void initTranslationWidget();//初始化平移容器
    void BeginCountdown();
    void OnTimerCountdown();
    void  checkmemory();
    void memorybutton(QDialog *dialog);
    void memorybutton();
    QString readJsonfloadname(QString key);
    void changelanguage();
protected:
    //初始化页面
    void createinit(QString path);
    //按钮格式生成函数
    void setbuttonPicturestyle(QLabel  *label1, QString strpath,int svgw,int svgh, QString labelstyle,int labelpositionw,int labelpositionh );
    //左右按钮格式函数
    void setleftrightbutton(QString svgpath ,QToolButton *button,int x,int y,int buttonsizex,int buttonsizey);
    //左右两边按钮点击事件函数
    //左边换页点击触发函数
    void  leftbuttronFun();
    //右边换页按钮点击触发函数
    void rightbuttonFun();

   //生成按钮
    //按钮生成函数
  QList<QWidget*>  greantbutton(QString jsonpath);
        //弹出框
   void messageBox();
    //每个按钮点击事件
    //按钮点击函数
//   void onclickbutton(QString directory,QString sheelcode);
    void onclickbutton(QString directory,QString sheelcode,QString bgpath);

    //按钮点击函数
    void changestyle(Mylabel &mybale);
    //读取json文件函数
     QJsonArray readjsonFile(QString filePath,QString youwant);
     void ShowTreaim();

     void ShowInputBox();


private slots:
    void on_pushButton_clicked(int page);


private:
    Ui::Form *ui;
    TranslationWidget *translationWidget;
};

#endif // FORM_H
