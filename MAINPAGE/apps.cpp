#include "apps.h"
#include "ui_apps.h"

InstallService::InstallService(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstallService)
{
    ui->setupUi(this);
    initinstallservice();
}

InstallService::~InstallService()
{
    delete ui;
}
void InstallService::initinstallservice()
{
    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
    this->screenwidth=screenRect.width();
    this->screenhight=screenRect.height();

    QScroller::grabGesture(ui->scrollArea,QScroller::LeftMouseButtonGesture);//TouchGesture


    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(TabWidgetWhichTabIsDown(int )));
    app=new App();
    ui->verticalLayout_4->addWidget(app);
    connect(app, SIGNAL(sendhavenew(int)), this, SLOT(receivehavenew(int)));
    connect(app, SIGNAL(sendToappsAndreload()), this, SLOT(recivereappsreload()));
    app->initapp();

    ui->tabWidget->setCurrentIndex(0);
}

void InstallService::onResize(QSize size)
{
    this->resize(size);
    supersize=size;
    ui->verticalWidget->resize((this->size().width()*5/6)-10,this->size().height()-20);
    ui->tabWidget->resize(QSize((this->size().width()*5/6)-20,this->size().height()-20));
    ui->horizontalWidget->resize(QSize((this->size().width()*5/6)-25,this->size().height()-50));
    ui->verticalWidget_2->resize(QSize(((this->size().width()*5/6)),this->size().height()));
    ui->verticalWidget_3->resize(QSize(((this->size().width()*5/6)),this->size().height()));
    ui->scrollArea->resize(QSize(((this->size().width()*5/6)),this->size().height()));

    if(app!=nullptr){
        app->onResize(QSize((this->size().width()*5/6),this->size().height()));
    }
    if(shop!=nullptr){
        shop->onResize(QSize((this->size().width()*5/6),this->size().height()));
    }
}

void InstallService::TabWidgetWhichTabIsDown(int i)
{
    //    qDebug()<<QString::number(i);
    //app
    if(i==0){

        if(shop!=nullptr){
            ui->verticalLayout_5->removeWidget(shop);
            delete(shop);
            shop=nullptr;

        }
        if(app!=nullptr){
            ui->verticalLayout_4->removeWidget(app);

            app=nullptr;
            delete(app);
        }

        app=new App();

        ui->verticalLayout_4->addWidget(app);
        app->initapp();
        connect(app, SIGNAL(sendhavenewtoapps(int)), this, SLOT(receivehavenew(int)));
        connect(app, SIGNAL(sendToappsAndreload()), this, SLOT(recivereappsreload()));
        //        connect(app, SIGNAL(sendToappsAndreload()), this, SLOT(recivereappsreload()));
        this->resize(supersize);
        ui->verticalWidget->resize((this->size().width()*5/6)-10,this->size().height()-20);

        ui->tabWidget->resize(QSize((this->size().width()*5/6)-20,this->size().height()-20));
        ui->horizontalWidget->resize(QSize((this->size().width()*5/6)-25,this->size().height()-50));
        ui->verticalWidget_2->resize(QSize(((this->size().width()*5/6)),this->size().height()));
        ui->verticalWidget_3->resize(QSize(((this->size().width()*5/6)),this->size().height()));
        ui->scrollArea->resize(QSize(((this->size().width()*5/6)),this->size().height()));
        app->onResize(QSize((this->size().width()*5/6),this->size().height()));

    }else if(i==1){
        //shop

        if(shop!=nullptr){
            ui->verticalLayout_5->removeWidget(shop);
            delete(shop);
            shop=nullptr;
        }
        if(app!=nullptr){
            ui->verticalLayout_4->removeWidget(app);
            delete(app);
            app=nullptr;
        }
        shop=new Shop();

        ui->verticalLayout_5->addWidget(shop);
        connect(shop, SIGNAL(successtoapps()), this, SLOT(recivesuccessforshops()));
        shop->initshop();
        this->resize(supersize);
        ui->verticalWidget->resize((this->size().width()*5/6)-10,this->size().height()-20);
        ui->tabWidget->resize(QSize((this->size().width()*5/6)-20,this->size().height()-20));
        ui->horizontalWidget->resize(QSize((this->size().width()*5/6)-25,this->size().height()-50));
        ui->verticalWidget_2->resize(QSize(((this->size().width()*5/6)),this->size().height()));
        ui->verticalWidget_3->resize(QSize(((this->size().width()*5/6)),this->size().height()));
        ui->scrollArea->resize(QSize(((this->size().width()*5/6)),this->size().height()));
        shop->onResize(QSize((this->size().width()*5/6),this->size().height()));
    }else{
        if(shop!=nullptr){
            delete(shop);
            shop=nullptr;
        }
        if(app!=nullptr){

            app=nullptr;
            delete(app);
        }
    }
}

void InstallService::receivehavenew(int i){
    qDebug()<<"apps--receivehavenew";
    qDebug()<<QString::number(i);
    emit sendhavenewtomore(i);
}

void InstallService::recivesuccessforshops()
{
    qDebug()<<"--------------------recivesuccessforshops-----------------------";
    emit successtomore();
    TabWidgetWhichTabIsDown(1);

}

void InstallService::recivereappsreload()
{
    qDebug()<<"--------------------recivereappsreload-----------------------";

    emit successtomore();

}

