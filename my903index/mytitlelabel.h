#ifndef MYTITLELABEL_H
#define MYTITLELABEL_H

#include <QObject>

class mytitlelabel : public QObject
{
    Q_OBJECT
public:
    explicit mytitlelabel(QObject *parent = nullptr);

    QString gettitle();
     QString getstyle();
    int getfontsize();
    QString gettyeface();
    int getlabelpositionx();
    int getlabelpositiony();

    void settitle(QString title);
    void setstyle(QString style);
    void setfontsize(int fontsize);
    void settyeface(QString typeface );
    void setpositionx(int positionx);
     void setpositiony(int positiony);
     mytitlelabel(QString  title,int fontsize,QString typeface,QString style,int labelpositionx,int lablepositiony);


private:
   QString  title;
    int  fontsize;
    QString style;
    QString typeface;
    int labelpositionx;
     int labelpositiony;




signals:

};

#endif // MYTITLELABEL_H
