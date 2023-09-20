#ifndef MYJSONBUTTONDATA_H
#define MYJSONBUTTONDATA_H
/*
  按钮就对象
  */
#include <QObject>

class myjsonbuttonData : public QObject
{
    Q_OBJECT
public:
    explicit myjsonbuttonData(QObject *parent = nullptr);
     int getFontSize();
     void setFontSize(int size);
     QString gettypeface();
     void settypeface(QString typeface);



    QString getDirectorypath();
    QString setDirectorypath(QString directorypath);
   QString getName();
   void setName(QString name);
   QString getFuncode();
   void setFuncode(QString funcode);
   QString getSvg();
   void setSvg(QString svg);
  int getSvgwidth();
   void setSvgwidth(int svgwidth);
   int  getSvgheight();
   void setSvgheight(int svgheight);
   void setX(int X);
   int getX();
   void setY(int Y);
    int getY();

    void setBgpath(QString bgpath);
     QString getBgpath();

    //卡片位置
    void setLabelpositionx(int X);
    int getLabelpositionx();
    void setLabelpositionY(int Y);
     int getLabelpositionY();

   void setSvgheight(QString svgheight);
   QString getButtonstyle();
   void setButtonstyle(QString buttonstyle);
   QString getLabelstyle();
   void setLabelstyle(QString  labelstyle);


   myjsonbuttonData( QString typeface,int fontsize, QString name,QString directorypath, QString funcode, QString svg,int svgwidth,int  svgheight,  int x,
                     int y,int labelpositionx,int labelpositiony,QString buttonstyle,QString  labelstyle,QString bgpath);


private:
    //按钮名字
    QString name;
    //字体
     QString typeface;
     //大小
     int fontsize;
    //按钮执行的shell命令
    QString directorypath;
    QString funcode;
    //按钮图片地址
    QString svg;
    //按钮宽度
    int svgwidth;
    //按钮高度
   int  svgheight;
    //按钮位置
    int  x;
     int y;
     //卡片位置
     int labelpositionx;
     int labelpositiony;

    //按钮样式
    QString buttonstyle;
    //图片样式
    QString  labelstyle;
    QString bgpath;

signals:







};

#endif // MYJSONBUTTONDATA_H
