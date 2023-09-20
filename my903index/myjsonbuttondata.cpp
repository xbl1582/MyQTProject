#include "myjsonbuttondata.h"

myjsonbuttonData::myjsonbuttonData(QObject *parent) : QObject(parent)
{

}

int myjsonbuttonData::getFontSize()
{
    return this->fontsize;
}

void myjsonbuttonData::setFontSize(int fontsize)
{
    this->fontsize=fontsize;
}

QString myjsonbuttonData::gettypeface()
{
    return this->typeface;
}

void myjsonbuttonData::settypeface(QString typeface)
{
   this->typeface=typeface;
}





QString myjsonbuttonData::getDirectorypath()
{
         return this->directorypath;
}

QString myjsonbuttonData::setDirectorypath(QString directorypath)
{
      this->directorypath=directorypath;
}

QString myjsonbuttonData::getName()
{
        return this->name;
    }
void myjsonbuttonData::setName(QString name)
{
        this->name=name;
    }

QString myjsonbuttonData::getFuncode()
{
        return this->funcode;
    }

void myjsonbuttonData::setFuncode(QString funcode)
{
        this->funcode=funcode;
    }

QString myjsonbuttonData::getSvg()
{
        return this->svg;
    }
void myjsonbuttonData::setSvg(QString svg)
{
        this->svg=svg;
    }
int myjsonbuttonData::getSvgwidth()
{
        return this->svgwidth;
    }
void myjsonbuttonData::setSvgwidth(int svgwidth)
{
        this->svgwidth=svgwidth;
    }

int myjsonbuttonData::getSvgheight()
{
    return this->svgheight;
}
void myjsonbuttonData::setSvgheight(int svgheight)
{
        this->svgheight=svgheight;
    }
void myjsonbuttonData::setX(int X)
{
    this->x=X;
}

int myjsonbuttonData::getX()
{
    return this->x;
}

void myjsonbuttonData::setY(int Y)
{
    this->y=Y;
}

int myjsonbuttonData::getY()
{
    return this->y;
}

void myjsonbuttonData::setBgpath(QString bgpath)
{
    this->bgpath=bgpath;
}
QString myjsonbuttonData::getBgpath()
{
   return this->bgpath;
}

void myjsonbuttonData::setLabelpositionx(int X)
{
    this->labelpositionx=X;
}

int myjsonbuttonData::getLabelpositionx()
{
    return this->labelpositionx;
}

void myjsonbuttonData::setLabelpositionY(int Y)
{
    this->labelpositiony=Y;
}

int myjsonbuttonData::getLabelpositionY()
{
    return this->labelpositiony;
}


QString myjsonbuttonData::getButtonstyle()
{
        return this->buttonstyle;
 }

void myjsonbuttonData::setButtonstyle(QString buttonstyle)
{
        this->buttonstyle=buttonstyle;
    }

QString myjsonbuttonData::getLabelstyle()
{
        return this->labelstyle;
    }
void myjsonbuttonData::setLabelstyle(QString labelstyle)
{
        this->labelstyle= labelstyle;
}
myjsonbuttonData::myjsonbuttonData(QString typeface,int fontsize,  QString name,QString directorypath, QString funcode, QString svg,int svgwidth,int  svgheight,  int x,
                                                      int y,int labelpositionx,int labelpositiony,QString buttonstyle,QString  labelstyle,QString bgpath)
{
        this->directorypath=directorypath;
        this->typeface=typeface;
        this->fontsize=fontsize;
         this->name=name;
         this->funcode=funcode;
         this->svg=svg;
         this->svgwidth=svgwidth;
         this->svgheight=svgheight;
         this->x=x;
         this->y=y;
        this->labelpositionx=labelpositionx;
     this->labelpositiony=labelpositiony;
         this->buttonstyle=buttonstyle;
         this->labelstyle= labelstyle;
    this->bgpath=bgpath;
}
