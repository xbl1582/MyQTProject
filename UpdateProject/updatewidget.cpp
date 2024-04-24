#include "updatewidget.h"
#include "ui_updatewidget.h"
#include"QDebug"
UpdateWidget::UpdateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UpdateWidget)
{
    ui->setupUi(this);
    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
       this->screenwidth=screenRect.width();
        this->screenhight=screenRect.height();
        Qt::WindowFlags m_flags = windowFlags();

        this->setWindowFlags(m_flags &~Qt::WindowStaysOnTopHint);

        this->setMinimumWidth(screenwidth);
        this->setMinimumHeight(screenhight);
     ui->verticalWidget->resize(screenwidth,screenhight-100);
     ui->progressBar->setMinimumWidth(screenwidth-500);
    QString path=QCoreApplication::applicationDirPath()+"/image/bg.jpg";
    qDebug()<<path;
    QPixmap pixmap = QPixmap(path).scaled(this->screenwidth,this->screenhight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    QPalette pal=this->palette();
    pal.setBrush(QPalette::Background,QBrush(pixmap));
    this->setAutoFillBackground(true);
    setPalette(pal);




}

UpdateWidget::~UpdateWidget()
{
    delete ui;
}

bool UpdateWidget::changeprogressbarvalue(int value)
{  
            ui->progressBar->setValue(value);
}

bool UpdateWidget::changeisok()
{
    isok=true;
}

bool UpdateWidget::setbarmaxnum(int value)
{
    ui->progressBar->setMaximum(value);
}
