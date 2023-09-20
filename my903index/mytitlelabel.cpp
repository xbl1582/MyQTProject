#include "mytitlelabel.h"

mytitlelabel::mytitlelabel(QObject *parent) : QObject(parent)
{

}

QString mytitlelabel::gettitle()
{
    return this->title;
}

QString mytitlelabel::getstyle()
{
    return this->style;
}

int mytitlelabel::getfontsize()
{
    return this->fontsize;
}

QString mytitlelabel::gettyeface()
{
    return this->typeface;
}

int mytitlelabel::getlabelpositionx()
{
    return this->labelpositionx;
}

int mytitlelabel::getlabelpositiony()
{
    return this->labelpositiony;
}

void mytitlelabel::settitle(QString title)
{
    this->title=title;
}

void mytitlelabel::setstyle(QString style)
{
    this->style=style;
}

void mytitlelabel::setfontsize(int fontsize)
{
       this->fontsize=fontsize;
}

void mytitlelabel::settyeface(QString typeface)
{
    this->typeface=typeface;
}

void mytitlelabel::setpositionx(int positionx)
{
    this->labelpositionx=positionx;
}

void mytitlelabel::setpositiony(int positiony)
{
    this->labelpositiony=positiony;
}

mytitlelabel::mytitlelabel(QString title, int fontsize, QString typeface,QString style, int labelpositionx, int lablepositiony)
{
     this->title=title;
      this->fontsize=fontsize;
    this->style=style;
    this->typeface=typeface;
     this->labelpositionx=labelpositionx;
    this->labelpositiony=lablepositiony;
}
