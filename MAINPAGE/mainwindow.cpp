#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPlainTextEdit>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //1.获取屏幕大小
    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
    this->screenwidth=screenRect.width();
    this->screenhight=screenRect.height();
    //2.获取程序当前位置
    appcurrentpath=QCoreApplication::applicationDirPath();
    basepath=appcurrentpath.mid(0,appcurrentpath.lastIndexOf("KSTMAIN"));
    //3.初始化子窗口
    qwidget=new QWidget();
    ui->setupUi(this);


    //4. 获取操作系统类型
    QSysInfo::productType();
    //5.修改ipservicesetting.ini配置文件中的系统类型
    ipservicesetting = new QSettings (appcurrentpath+"/config/ipserviceconfig.ini", QSettings::IniFormat);//构建函数
    if(QSysInfo::productType() == "windows")
    {
        qDebug() << "windows system";
        ipservicesetting->setValue("/ipserviceconfig/systemtype","win");
        ipservicesetting->sync();

    }else{
        qDebug()<<QSysInfo::productType();
        ipservicesetting->setValue("/ipserviceconfig/systemtype","arm");
        ipservicesetting->sync();

    }
    delete (ipservicesetting);
    //6.读取systemsetting.ini配置文件
    readconfigforsystemsetting();
    //7. 读取ipservicesetting.ini配置文件
    readconfigforIPService();
    //8.判断系统类型，3为后台，2为903扫描仪，1为票箱
    if(device==3){
        //9.当显示的是3，后台软件不显示点击启动应用的页面
        this->resize(1100,800);
        //10.初始化更多页
        more=new More(this);
        ui->pushButton->setVisible(false);
        this->setWindowIcon(QIcon(appcurrentpath+"/image/logo/KSTMAIN.ico"));

        connect(more, SIGNAL(sendhavenewtomainwindow(int)), this, SLOT(receivehavenew(int)));
        connect(more, SIGNAL(sendlange(QString,int)), this, SLOT(receivelange(QString,int)));
        //         connect(more, SIGNAL(successtomainwindow()), this, SLOT(recivesuccessformore()));
        more->showButton(false, false);
        more->onResize(this->size());
    }else{
        //10.初始化更多页
        more=new More();
         more->setWindowIcon(QIcon(appcurrentpath+"/image/logo/KSTMAIN.ico"));
        ui->pushButton->setVisible(true);
        connect(more, SIGNAL(sendhavenewtomainwindow(int)), this, SLOT(receivehavenew(int)));
        connect(more, SIGNAL(sendlange(QString,int)), this, SLOT(receivelange(QString,int)));
        connect(more, SIGNAL(successtomainwindow()), this, SLOT(recivesuccessformore()));
        more->onResize(this->size());
        this->setMinimumWidth(this->screenwidth);
        this->setMinimumHeight(this->screenhight);
        //11.创建一个定时器用于实时监控硬盘存储大小
        tr2 = new QTimer(this);//创建一个定时器
        connect(tr2,&QTimer::timeout,this,&MainWindow::OnTimerCountdown);
        BeginCountdown();

        //12.设置窗口的背景图片初始化图片
        setbackground(appcurrentpath+"/image/firstmain.png");
        //13.扫描未被使用的ip地址,子进程进行
        checkIpUse=new CheckIpUse();
        connect(checkIpUse, &CheckIpUse::reportData,this,[=](QString data){
            ipnousedata=data;
        });
        checkIpUse->start();
        //14.初始化窗口
        initMainWindow();
        //15.绑定窗口变化事件
        connect(this, &MainWindow::resized, this, &MainWindow::onResize);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initMainWindow()
{
    //1. 设置更多按钮图标
    ui->pushButton->setIcon(QIcon(appcurrentpath+"/image/more_nointer.png"));
    ui->pushButton->setIconSize(QSize(50,50));
    //2.获取程序当前目录
    appcurrentpath=QCoreApplication::applicationDirPath();
    //3.获取程序上层目录
    basepath=appcurrentpath.split("/KSTMAIN")[0];


    //4.创建数据库
    creatdatabase();
    //5.创建数据库执行对象
    sql_query=new QSqlQuery(database);
    //6.调用数据库执行函数，进行表创建
    todosql(QString("CREATE TABLE  If Not Exists appsversion("
                    "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                    "uuid text,"
                    "appname  TEXT,"
                    "apppath TEXT,"
                    "apprtmversion integer,"
                    "apprtmversionchar TEXT,"
                    "apprtmupdatetime integer,"
                    "appupdatedversion integer,"
                    "appupdatedversionchar TEXT,"
                    "appupdatedtime integer,"
                    "appupdateddesc TEXT,"
                    "appsize text,"
                    "appico TEXT,"
                    "appsystem TEXT,"
                    "appdevice TEXT,"
                    "priority integer DEFAULT 0"
                    " );"));

    //7.绑定IP按钮事件
    connect(ui->pushButton_2,&QPushButton::clicked,this,[=](){
        //打开应用服务
        //8.显示ip设置弹窗
        ShowInputBox();
    });



    //9.根据设备配置的类型设置不同的背景图片
    if(ipservicepojo->getDevicetype()==1){
        if(lange=="cn"){
            setbackground(appcurrentpath+"/image/bg.png");
        }else if(lange=="en"){
            setbackground(appcurrentpath+"/image/bgen.png");
        }

    }else if(ipservicepojo->getDevicetype()==2){
        if(lange=="cn"){
            setbackground(appcurrentpath+"/image/bg2.png");
        }else if(lange=="en"){
            setbackground(appcurrentpath+"/image/bg2en.png");
        }

    }else  if(ipservicepojo->getDevicetype()==3){
        //后台
        qDebug()<<"后台";
    }
    //10.生成软件(app)按钮
    qflist=generateWindow();
    applist=new APPList(qflist,qwidget);
    ui->verticalLayout_2->addWidget(qwidget);


    //修改更多按钮样式

    //11.绑定更多按钮
    connect( ui->pushButton,&QPushButton::clicked,this,[=](){
        //12.打开设置页
        showMoreFun();
    });


    //定义计时器start-------------------------------------

    //定义信号检测计时器：每5秒/次
    //12.检查硬盘存储容量,与网络通信
    bool internetflag=chackInternet(ipservicepojo->getIp(),ipservicepojo->getPort(),1000);
    if(internetflag){
        if(active==1){
            ui->pushButton->setIcon(QIcon(appcurrentpath+"/image/more_inter_active.png"));
        }else{
            ui->pushButton->setIcon(QIcon(appcurrentpath+"/image/more_inter_noactive.png"));
        }

        ui->pushButton->setIconSize(QSize(50,50));
    }
    checkmemory();

    signaltimer=new QTimer(this);
    //13.当网络断开，修改更多图标颜色
    connect(signaltimer, &QTimer::timeout,
            this,[=]{
        if(ipservicepojo->getIp()!=nullptr &&ipservicepojo->getPort()!=0){
            bool internetflag=chackInternet(ipservicepojo->getIp(),ipservicepojo->getPort(),1000);
            if(internetflag){
                ui->pushButton->setIcon(QIcon(appcurrentpath+"/image/more_inter_noactive.png"));
                ui->pushButton->setIconSize(QSize(50,50));
            }else{
                ui->pushButton->setIcon(QIcon(appcurrentpath+"/image/more_nointer.png"));
            }
            if(active==1){

                ui->pushButton->setIcon(QIcon(appcurrentpath+"/image/more_inter_active.png"));
                ui->pushButton->setIconSize(QSize(50,50));
                ui->pushButton->repaint();

            }
        }
        //        creatdatabase();
        //        sql_query=new QSqlQuery(database);
        //        int count=0;
        //        if(database.isOpen()){
        //            count=selectAppsversion().size();
        //        }else{
        //            database.open();
        //            count=selectAppsversion().size();
        //        }
        //        if(appcount!=count){
        //            //            reloadMainWindow();
        //            appcount=count;
        //            closedatabase();
        //        }
        checkmemory();
    });
    signaltimer->start(5000);
    //14.启动程序执行ip设置
    QString str="sh "+QCoreApplication::applicationDirPath()+"/shellcode/changIP.sh";
    const char *code2 =(str).toLocal8Bit();
    int result=system(code2);
    if(!(System_Check(result))){
    }else{
        showmessage("IP设置失败",nullptr);
    }
    //15.设置屏幕旋转角度
    QString rotate=" echo \"output:all:rotate"+QString::number(rote)+"\" >> /tmp/.weston_drm.conf";
    const char* commondrotate=rotate.toLocal8Bit().constData();
    int result2=system(commondrotate);

    if(!(System_Check(result2))){
    }else{
        showmessage("屏幕旋转设置失败",nullptr);
    }

    //定义计时器end-----------------------------------
    //16.绑定信号与槽
    //16.1有更新信号与槽处理
    connect(more, SIGNAL(sendhavenewtomainwindow(int)), this, SLOT(receivehavenew(int)));
    //16.2更换语言信号与槽处理
    connect(more, SIGNAL(sendlange(QString,int)), this, SLOT(receivelange(QString,int)));
    //16.3软件安装成功的信号与槽处理
    connect(more, SIGNAL(successtomainwindow()), this, SLOT(recivesuccessformore()));

}
//1.窗口重新刷新
void MainWindow::reloadMainWindow()
{

    if(applist!=nullptr){
        delete(applist);
        applist=nullptr;
        ui->verticalLayout_2->removeWidget(qwidget);
        delete(qwidget);
        qwidget=nullptr;

    }
    for(int i=0;i<qflist.size();i++){
        qflist.removeAt(i);
    }
    qflist=generateWindow();
    qwidget=new QWidget();
    applist=new APPList(qflist,qwidget);
    ui->verticalLayout_2->addWidget(qwidget);

    this->setMinimumWidth(this->screenwidth);
    this->setMinimumHeight(this->screenhight);
    this->onResize();
    more->onResize(this->size());
    closedatabase();

}

void MainWindow::changeDrivertype()
{
    if(device==1){


        if(lange=="cn"){
            setbackground(appcurrentpath+"/image/bg.png");
        }else{
            setbackground(appcurrentpath+"/image/bgen.png");
        }

    }else if(device==2)  {
        if(lange=="cn"){

            setbackground(appcurrentpath+"/image/bg2.png");
        }else{
            setbackground(appcurrentpath+"/image/bgen2.png");
        }


    }else if(device==3){

    }
}

void MainWindow::readconfigforIPService()
{
    //读取的时候创建
    QFile qf(appcurrentpath+"/config/ipserviceconfig.ini");
    if(qf.exists()){

        ipservicesetting = new QSettings (appcurrentpath+"/config/ipserviceconfig.ini", QSettings::IniFormat);//构建函数
        ipservicepojo=new IPServicePoJo();
        //读取配置文件内容
        ipservicepojo->setIp(ipservicesetting->value("/ipserviceconfig/ip").toString());
        ipservicepojo->setPort(ipservicesetting->value("/ipserviceconfig/port").toInt());
        ipservicepojo->setOuturl(ipservicesetting->value("/ipserviceconfig/outurl").toString());
        ipservicepojo->setUrltype(ipservicesetting->value("/ipserviceconfig/urltype").toString());
        ipservicepojo->setDevicetype(ipservicesetting->value("/ipserviceconfig/devicetype").toInt());
        ipservicepojo->setSystemtype(ipservicesetting->value("/ipserviceconfig/systemtype").toString());
        delete(ipservicesetting);
    }

}

void MainWindow::readconfigforsystemsetting()
{
    systemsettings=new QSettings (appcurrentpath+"/config/systemsetting.ini", QSettings::IniFormat);
    rote=systemsettings->value("systemconfig/rote").toInt();
    device= systemsettings->value("systemconfig/device").toInt();
    lange=systemsettings->value("systemconfig/lange").toString();
}
//创建数据库
void MainWindow::creatdatabase()
{

    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(appcurrentpath+"/db/KSTMAIN.db");
        database.setUserName("gdwt");
        database.setPassword("gdwt");
    }
    database.open();

}
//数据库操作
void MainWindow::todosql(QString sql)
{

    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        // do something创建数据表
        QString create_sql = sql;
        sql_query->prepare(create_sql);
        if(!sql_query->exec())
        {
            qDebug() << "Error: Fail to create table." << sql_query->lastError();
        }
        else
        {
            qDebug() << "Table created!";
        }

    }

}
//数据库操作完成后，最好关闭。
void MainWindow::closedatabase()
{
    database.close();
}

void MainWindow::BeginCountdown()
{

    if (tr2->isActive() == false)
    {
        //当前定时器未激活时，激活定时器
        //qDebug()<<"激活定时器tr2";
        tr2->start(500);
    }

}

void MainWindow::OnTimerCountdown()
{
    if(ipservicepojo->getDevicetype()==1){
        if (tr2->isActive() == true)
        {
            tr2->stop();
        }
        delete tr2;
        tr2= nullptr;
    }else if(ipservicepojo->getDevicetype()==2)  {

        QProcess process;
        process.start("lsusb");
        process.waitForFinished();
        QByteArray output = process.readAllStandardOutput();
        QString str_output = output;
        //qDebug()<<"检测是否启动扫描仪";
        //

        if(str_output.indexOf("3216:1042") >= 0)
        {
            if (tr2->isActive() == true)
            {
                tr2->stop();
            }
            delete tr2;
            tr2= nullptr;
            changeDrivertype();
            ui->verticalWidget->setVisible(true);
        }else{
            setbackground(appcurrentpath+"/image/firstmain.png");
            ui->verticalWidget->setVisible(false);
        }
    }

}



QList<QWidget *> MainWindow::generateWindow()
{
    QList<AppsVersion *>list=selectAppsversion();
    QList<AppsVersion *>listnokstmain=list;

    for(int i=0;i<listnokstmain.size();i++){

        if(listnokstmain.at(i)->getAppname()=="KSTMAINX"){
            listnokstmain.removeAt(i);
        }else
            if(listnokstmain.at(i)->getAppname()=="KSTMAINQ"){
                listnokstmain.removeAt(i);
            }else
                if(listnokstmain.at(i)->getAppname()=="KSTMAIN"){
                    listnokstmain.removeAt(i);
                }
    }

    int allapps=listnokstmain.size();
    int tempone=allapps%3;
    int page=allapps/3;
    if(tempone>0){
        page+=1;
    }
    QList<QWidget *> qlist;

    for(int i=0;i<page;i++){
        QWidget *qw=new QWidget();
        //        QHBoxLayout *qhb=new QHBoxLayout(qw);
        qgr=new QGridLayout(qw);
        //----------------------------------
        qw->setMinimumWidth(this->size().rwidth());
        //----------------------------------
        qw->setMinimumHeight(350);
        //        qw->setStyleSheet("QWidget{background:rgba(0,0,0,0.3);}");
        for(int j=i*3;j<(i*3+3)&j<allapps;j++){

            QPushButton *qp=new QPushButton();
            //            qp->setMaximumWidth(200);
            qp->setFlat(true);
            qp->setStyleSheet("QPushButton:hover{background-color:rgba(0,0,0,0);}"
                              "QPushButton:pressed{background-color: rgba(0,0,0,0);"
                              "border-style: inset;}");
            qgr->setAlignment(Qt::AlignCenter);
            if(j==i*3){
                //                qp->move(400,50);
                qgr->addWidget(qp,1,2);
                //                qhb->addWidget(qp,1,Qt::AlignCenter);
            }else if(j==(i*3+1)){
                //                qp->move(100,50);
                qgr->addWidget(qp,1,3);
                //                qhb->addWidget(qp,1,Qt::AlignCenter);
            }else if(j==(i*3+2)){
                qgr->addWidget(qp,1,1);
                //                qp->move(700,50);
                //                qhb->addWidget(qp,1,Qt::AlignCenter);
            }
            qp->resize(100,100);
            qp->setFlat(true);

            QFile qfile(appcurrentpath+"/image/logo/"+listnokstmain.at(j)->getAppico());
            if(qfile.exists()){
                if(lange=="en"){
                    qp->setIcon(QIcon(appcurrentpath+"/image/logo/"+listnokstmain.at(j)->getAppico().split(".png")[0]+"en.png"));
                }else{
                    qp->setIcon(QIcon(appcurrentpath+"/image/logo/"+listnokstmain.at(j)->getAppico()));
                }

            }else{
                qp->setIcon(QIcon(appcurrentpath+"/image/logo/Default.png"));
            }

            qp->setIconSize(QSize(200,200));
            //            qp->setText("第"+QString::number(j)+"按钮");
            qp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            connect(qp,&QPushButton::clicked,this,[=]{
                bool falg= generateStartupScript(listnokstmain.at(j));
                if(falg){
                    if(lange=="en"){
                        QString shellname="run"+listnokstmain.at(j)->getAppname().toLower()+"en.sh";
                        todoshell(shellname,listnokstmain.at(j));
                    }else{
                        QString shellname="run"+listnokstmain.at(j)->getAppname().toLower()+".sh";
                        todoshell(shellname,listnokstmain.at(j));
                    }
                }else{
                    msgBox=new QMessageBox(QMessageBox::Warning, tr("警告"),tr( "启动程序失败"), QMessageBox::Ok, this);
                    msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
                    QFont qf;
                    qf.setPixelSize(20);
                    msgBox->setFont(qf);
                    msgBox->setMinimumWidth(200);
                    msgBox->setMinimumHeight(200);
                    msgBox->setBaseSize(200,200);

                    msgBox->setFixedSize(200,200);
                    msgBox->setStyleSheet("QLabel{"

                                          "}");
                    int result2 = msgBox->exec();
                }
            });


        }
        qlist.append(qw);
    }


    return qlist;
}

void MainWindow::changelange(QString lange2)
{
    lange=lange2;
    ui->retranslateUi(this);

}

void MainWindow::showMoreFun()
{
    if(more==nullptr){
        more=new More();
    }

    more->onResize(this->size());
    if(active==0){
        more->show();
    }
    active=0;

}

bool MainWindow::chackInternet(QString ip, int port, int timeout)
{
    return IPLive(ip, port,timeout)==true?true:(IsWebOk()==true?true:false);
}
bool MainWindow::IPLive(QString ip, int port, int timeout) {
    QTcpSocket tcpClient;
    tcpClient.abort();
    tcpClient.connectToHost(ip, port);
    //100毫秒没有连接上则判断不在线
    return tcpClient.waitForConnected(timeout);
}
//判断是否通外网
bool MainWindow::IsWebOk() {
    //能接通百度IP说明可以通外网
    return IPLive("121.201.19.27", 80);
}

//检测内存
void MainWindow::checkmemory()
{


    //memory

    //设置内存大小数值
    QString shcode="sh GetDisk.sh";
    //qDebug()<<"执行脚本:sh GetDisk.sh";
    QFile f2(QCoreApplication::applicationDirPath()+"/shellcode/GetDisk.sh");
    if(f2.exists()){
        QString commond="sh "+QCoreApplication::applicationDirPath()+"/shellcode/GetDisk.sh";
        const char* commond2=commond.toLocal8Bit().constData();
        system(commond2);

        QFile f(QCoreApplication::applicationDirPath()+"/shellcode/GetDisk_1.txt");
        //qDebug()<<"读取磁盘返回内容"+QCoreApplication::applicationDirPath()+"/shellcode/GetDisk_1.txt";
        if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << ("打开文件失败");

        }
        QTextStream txtInput(&f);
        QString lineStr;
        while(!txtInput.atEnd())
        {
            lineStr = txtInput.readLine();
        }
        f.close();
        memory=lineStr;

        QRegExp rx("\\d+");
        rx.indexIn(memory,0);
        QString numsize=rx.cap(0);


        QString numsizedangwei=memory.at(memory.size()-1);
        //        qDebug()<<"设置硬盘容量:"+memory;
        //       ui->pushButton_2->setText(memory);
        ui->pushButton_2->setIcon(QIcon(appcurrentpath+"/image/ip.png"));
        ui->pushButton_2->setIconSize(QSize(30,30));
        if(numsizedangwei=="G"){
            //
            if(numsize.toInt()<=1){
                //                ui->pushButton_2->setIcon(QIcon(appcurrentpath+"/image/ip.png"));
                //                ui->pushButton_2->setIconSize(QSize(30,30));
                if(isshowmessage){
                    QFont qf;
                    qf.setPixelSize(30);
                    msgBox=new QMessageBox(QMessageBox::Warning, tr("警告"),tr( "存储空间过低请，即时清除空间"), QMessageBox::Ok, this);
                    ////qDebug()<<"存储空间过低请，即时清除空间";

                    msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
                    msgBox->setFont(qf);
                    msgBox->setMinimumWidth(200);
                    msgBox->setMinimumHeight(200);
                    msgBox->setBaseSize(200,200);

                    msgBox->setFixedSize(200,200);
                    msgBox->setStyleSheet("QLabel{"

                                          "}");
                    int result2 = msgBox->exec();
                }
            }
        }else{
            if(isshowmessage){
                QFont qf;
                qf.setPixelSize(30);
                msgBox=new QMessageBox(QMessageBox::Warning, tr("警告"), tr("存储空间过低请，即时清除空间"), QMessageBox::Ok, this);
                // //qDebug()<<"存储空间过低请，即时清除空间";
                msgBox->setFont(qf);
                msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
                msgBox->setMinimumWidth(200);
                msgBox->setMinimumHeight(200);
                msgBox->setBaseSize(200,200);

                msgBox->setFixedSize(200,200);
                msgBox->setStyleSheet("QLabel{"
                                      "min-width: 200px;"
                                      "min-height: 100px; "
                                      "}");
                int result2 = msgBox->exec();
            }
        }
        isshowmessage=false;
    }


}

void MainWindow::setbackground(QString path)
{
    pixmap = QPixmap(path).scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    //qDebug()<<"设置背景图片";
    QPalette pal=this->palette();
    pal.setBrush(QPalette::Background,QBrush(pixmap));
    this->setAutoFillBackground(true);
    setPalette(pal);
}

void MainWindow::ShowInputBox()
{
    readeth();
    QHostAddress address = QHostAddress(QHostAddress::LocalHost);
    QString localHostName = QHostInfo::localHostName();
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    QString ipv6;
    int howmanyeth=0;
    QMap<QString, QString> ipv4str;


    bool haseth0=false,haseth1=false,haseth2=false,haseth3=false,haseth4=false;


    for (const QNetworkInterface& interface : list) {

        if(interface.name()=="eth0"){
            ////qDebug()<<"存在网卡1,启动网卡1可编辑";
            haseth0=true;

        }
        //ipv4 = entry.ip().toString();
        else if (interface.name()=="eth1"){
            ////qDebug()<<"存在网卡2,启动网卡2可编辑";
            haseth1=true;

        }
        else if(interface.name()=="eth2"){
            ////qDebug()<<"存在网卡3,启动网卡3可编辑";
            haseth2=true;

        }
        else if(interface.name()=="eth3"){
            ////qDebug()<<"存在网卡4,启动网卡4可编辑";
            haseth3=true;

        }
        else if(interface.name()=="eth4"){
            ////qDebug()<<"存在网卡4,启动网卡4可编辑";
            haseth4=true;

        }

        //            ////qDebug()<<interface;

        if (interface.type() == QNetworkInterface::Loopback) {
            continue;
        }
        QList<QNetworkAddressEntry> entryList = interface.addressEntries();
        for (const QNetworkAddressEntry& entry : entryList) {

            if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol) {
                ipv6 = entry.ip().toString();
                ////qDebug()<< ipv6;
            } else if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                //                      ////qDebug()<<entry.ip().toString();

                if(interface.name()=="eth0"){

                    haseth0=true;
                    ipv4str.insert("eth0",entry.ip().toString());

                }
                //ipv4 = entry.ip().toString();
                else if (interface.name()=="eth1"){

                    haseth1=true;
                    ipv4str.insert("eth1",entry.ip().toString());

                }
                else if(interface.name()=="eth2"){
                    haseth2=true;
                    ipv4str.insert("eth2",entry.ip().toString());

                }
                else if(interface.name()=="eth3"){
                    haseth3=true;
                    ipv4str.insert("eth3",entry.ip().toString());

                }
                else if(interface.name()=="eth4"){
                    haseth4=true;
                    ipv4str.insert("eth4",entry.ip().toString());

                }
                //                              ipv42= entry.ip().toString();
            }
        }
    }

    QDialog dialog(this);
    dialog.setWindowTitle(tr("修改ip地址"));
    dialog.resize(QSize(600,400));
    QGridLayout form(&dialog);

    QFont qf;
    // Value1
    QLabel *val1=new QLabel("ip1: ");
    qf.setPixelSize(30);
    val1->setFont(qf);
    QLineEdit *qle1=new QLineEdit(&dialog);
    qle1->setMinimumHeight(60);
    qle1->setReadOnly(!haseth0);
    //    if(ipv4!=nullptr &&ipv4==""){
    //        ipv4=ipv4str.value("eth0");
    //    }

    qle1->setText(ipv4);

    qle1->setFont(qf);
    form.addWidget(val1,1,1,1,1);
    form.addWidget(qle1,1,2,1,6);
    // Value2
    QLabel *val2=new QLabel("ip2:");
    val2->setFont(qf);
    QLineEdit *qle2=new QLineEdit(&dialog);
    qle2->setMinimumHeight(60);
    qle2->setReadOnly(!haseth1);
    //    qle2->setReadOnly(true);
    qle2->setFont(qf);
    //     qle2->setText(ipv42);
    //    if(ipv42==nullptr &&ipv42==""){
    //        ipv42=ipv4str.value("eth1");
    //    }


    qle2->setText(ipv42);
    form.addWidget(val2,2,1,1,1);
    form.addWidget(qle2,2,2,1,6);

    //value3
    QLabel *val3=new QLabel("ip3:");
    val3->setFont(qf);
    QLineEdit *qle3=new QLineEdit(&dialog);
    qle3->setMinimumHeight(60);
    qle3->setReadOnly(!haseth2);
    qle3->setFont(qf);

    //    if(ipv43==nullptr &&ipv43==""){
    //        ipv43=ipv4str.value("eth2");
    //    }
    qle3->setText(ipv43);
    form.addWidget(val3,3,1,1,1);
    form.addWidget(qle3,3,2,1,6);

    //value4
    QLabel *val4=new QLabel("ip3:");
    val4->setFont(qf);
    QLineEdit *qle4=new QLineEdit(&dialog);
    qle4->setMinimumHeight(60);
    qle4->setReadOnly(!haseth3);
    qle4->setFont(qf);
    //    ipv44=ipv4str.value("eth3");
    if(ipv44==nullptr &&ipv44==""){
        ipv44=ipv4str.value("eth3");
    }
    qle4->setText(ipv44);
    form.addWidget(val4,3,1,1,1);
    form.addWidget(qle4,3,2,1,6);
    bool iseth2=false;
    if(ipv4str.value("eth2")!=nullptr&&ipv4str.value("eth2")!=""){
        iseth2=true;
        val3->setVisible(true);
        qle3->setVisible(true);
        val4->setVisible(false);
        qle4->setVisible(false);
    }else
        if(ipv4str.value("eth3")!=nullptr&&ipv4str.value("eth3")!=""){
            iseth2=false;
            val4->setVisible(true);
            qle4->setVisible(true);
            val3->setVisible(false);
            qle3->setVisible(false);
        }else{
            iseth2=true;
            val3->setVisible(true);
            qle3->setVisible(true);
            val4->setVisible(false);
            qle4->setVisible(false);
        }

    //显示按钮键盘


    for(int i=0;i<12;i++){
        QPushButton *qtb=new QPushButton(&dialog);
        qtb->setFocusPolicy(Qt::NoFocus);
        qtb->resize(50,60);
        QFont qf2;
        qf2.setPointSize(18);
        qtb->setFont(qf2);
        qtb->setMinimumHeight(60);
        if(i==0){
            qtb->setText(QString::number(i));
            connect(qtb,&QPushButton::clicked,qtb,[=](){

                if(qle1->hasFocus()){
                    ipv4+=QString::number(i);
                    //qDebug()<< "ip1输入数字"+QString::number(i);
                    qle1->setText(ipv4);
                }
                if(qle2->hasFocus()){
                    ipv42+=QString::number(i);
                    //qDebug()<< "ip2输入数字"+QString::number(i);
                    qle2->setText(ipv42);
                }
                if(iseth2){
                    if(qle3->hasFocus()){
                        //qDebug()<< "ip3输入数字"+QString::number(i);
                        ipv43+=QString::number(i);
                        qle3->setText(ipv43);
                    }
                }else{
                    if(qle4->hasFocus()){
                        //qDebug()<< "ip3输入数字"+QString::number(i);
                        ipv44+=QString::number(i);
                        qle4->setText(ipv44);
                    }
                }


            });
            form.addWidget(qtb,10,2,2,2);
        }
        else if(i==10){
            form.addWidget(qtb,10,4,2,2);
            qtb->setText(".");
            connect(qtb,&QPushButton::clicked,qtb,[=](){


                if(qle1->hasFocus()){
                    ipv4+=".";
                    //qDebug()<< "ip1输入:.";
                    qle1->setText(ipv4);
                }
                if(qle2->hasFocus()){
                    ipv42+=".";
                    //qDebug()<< "ip2输入:.";
                    qle2->setText(ipv42);
                }
                if(iseth2){
                    if(qle3->hasFocus()){
                        //qDebug()<< "ip3输入:.";
                        ipv43+=".";
                        qle3->setText(ipv43);
                    }
                }else{
                    if(qle4->hasFocus()){
                        //qDebug()<< "ip3输入:.";
                        ipv44+=".";
                        qle4->setText(ipv44);
                    }
                }


            });

        }else if(i==11){
            qtb->setText("←");
            form.addWidget(qtb,10,6,2,2);
            connect(qtb,&QPushButton::clicked,qtb,[=](){


                if(qle1->hasFocus()){

                    int size=ipv4.size();
                    if(size>0){

                        ipv4=ipv4.left(size-1);

                    }
                    qle1->setText(ipv4);
                }
                if(qle2->hasFocus()){

                    int size=ipv42.size();
                    if(size>0){

                        ipv42=ipv42.left(size-1);

                    }
                    qle2->setText(ipv42);
                    //qDebug()<<ipv42;
                }
                if(iseth2){
                    if(qle3->hasFocus()){
                        int size=ipv43.size();
                        if(size>0){
                            ipv43=ipv43.left(size-1);

                        }

                        qle3->setText(ipv43);
                    }
                }else{
                    if(qle4->hasFocus()){
                        int size=ipv44.size();
                        if(size>0){
                            ipv44=ipv44.left(size-1);

                        }

                        qle4->setText(ipv44);
                    }

                }
            });

        }else{
            qtb->setText(QString::number(i));
            connect(qtb,&QPushButton::clicked,qtb,[=](){


                if(qle1->hasFocus()){
                    //qDebug()<< "ip1:获取焦点";
                    ipv4+=QString::number(i);
                    qle1->setText(ipv4);
                }
                if(qle2->hasFocus()){
                    //qDebug()<< "ip2:获取焦点";
                    ipv42+=QString::number(i);
                    qle2->setText(ipv42);
                }
                if(iseth2){
                    if(qle3->hasFocus()){
                        //qDebug()<< "ip3:获取焦点";
                        ipv43+=QString::number(i);
                        qle3->setText(ipv43);
                    }
                }else{
                    if(qle4->hasFocus()){
                        //qDebug()<< "ip3:获取焦点";
                        ipv44+=QString::number(i);
                        qle4->setText(ipv44);
                    }
                }
            });
            switch (i) {
            case 1:form.addWidget(qtb,8,2,2,2);break;
            case 2:form.addWidget(qtb,8,4,2,2);break;
            case 3: form.addWidget(qtb,8,6,2,2);break;
            case 4:form.addWidget(qtb,6,2,2,2);break;
            case 5:form.addWidget(qtb,6,4,2,2);break;
            case 6: form.addWidget(qtb,6,6,2,2);break;
            case 7:form.addWidget(qtb,4,2,2,2);break;
            case 8:form.addWidget(qtb,4,4,2,2);break;
            case 9:form.addWidget(qtb,4,6,2,2);break;

            }
        }






    }



    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    buttonBox.setStyleSheet("text-align:center");

    buttonBox.button(QDialogButtonBox::Ok)->setText(tr("保存"));
    QFont qf2;
    qf2.setPixelSize(25);
    buttonBox.button(QDialogButtonBox::Ok)->setMinimumHeight(50);
    buttonBox.button(QDialogButtonBox::Ok)->setFont(qf2);
    buttonBox.button(QDialogButtonBox::Ok)->setMinimumWidth(150);
    buttonBox.button(QDialogButtonBox::Ok)->setStyleSheet("background-color:rgb(120,120,255); border-radius:5px;color:rgb(255,255,255);margin-left:30px;");

    buttonBox.button(QDialogButtonBox::Cancel)->setText(tr("取消"));
    buttonBox.button(QDialogButtonBox::Cancel)->setFont(qf2);
    buttonBox.button(QDialogButtonBox::Cancel)->setMinimumHeight(50);
    buttonBox.button(QDialogButtonBox::Cancel)->setMinimumWidth(150);
    buttonBox.button(QDialogButtonBox::Cancel)->setStyleSheet("background-color:rgb(255,0,0); border-radius:5px;color:rgb(255,255,255);margin-right:30px;");
    //    QPushButton *q3=new QPushButton();
    //    q3->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/image/Drive.png"));
    //    q3->setIconSize(QSize(30,30));
    QFont qf3;
    qf.setPointSize(30);
    //    q3->setFont(qf3);
    //    q3->resize(150,50);
    //    q3->move(this->screenwidth-300,this->screenhight-100);
    //    q3->setFlat(true);
    //    q3->setText(memory);
    //    q3->setParent(this);
    //    form.addWidget(q3,12,1,1,1);
    form.addWidget(&buttonBox,13,3,1,3);



    QPushButton *q4=new QPushButton();
    q4->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/image/chontu.png"));
    q4->setIconSize(QSize(30,30));
    QFont qf4;
    qf.setPointSize(30);
    q4->setFont(qf3);
    q4->resize(150,50);
    q4->move(this->screenwidth-300,this->screenhight-100);
    q4->setFlat(true);
    q4->setText("未用IP");
    q4->setParent(this);
    connect(q4,&QPushButton::clicked,this,[=](){
        checkhavenotuseip();
    });

    form.addWidget(q4,12,7,1,1);










    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {

        //qDebug()<<"执行ip修改"<<endl;
        QString ip1=qle1->text();
        //qDebug()<< "保存ip1:"+ip1;
        QString ip2=qle2->text();
        //qDebug()<< "保存ip2:"+ip2;
        QString ip3=qle3->text();
        //qDebug()<< "保存ip3:"+ip3;
        QString ip4=qle4->text();
        //qDebug()<< "保存ip4:"+ip4;

        QFile qf("./shellcode/changIP.sh");
        if(qf.open(QFile::WriteOnly | QFile::Truncate)){        //文件流写文件
            QTextStream out(&qf);      //通过文本流控制文件写操作
            QString changIPcontent="#!/bin/bash\nifconfig eth0 "+ip1+" \n";
            //qDebug()<< "往脚本写命令:ifconfig eth0 "+ip1+" \n";
            if(ip2!=""){
                changIPcontent+=+"ifconfig eth1 "+ip2+" \n";
                //qDebug()<< "往脚本写命令:ifconfig eth1 "+ip2+" \n";
            }
            if(ip3!=""){
                changIPcontent+=+"ifconfig eth2 "+ip3+" \n";
                //qDebug()<< "往脚本写命令:ifconfig eth2 "+ip3+" \n";
            }
            if(ip4!=""){
                changIPcontent+=+"ifconfig eth3 "+ip4+" \n";
                //qDebug()<< "往脚本写命令:ifconfig eth3 "+ip4+" \n";
            }
            out<<changIPcontent<<endl;
        }
        qf.close();
        QString str="sh "+QCoreApplication::applicationDirPath()+"/shellcode/changIP.sh";
        //qDebug()<< "执行ip设置脚本"+str;
        const char *code2 =(str).toLocal8Bit();
        system(code2);

    }


}

void MainWindow::readeth(){
    //        //qDebug()<<"读取修改ip脚本:"+QCoreApplication::applicationDirPath()+"/shellcode/changIP.sh";
    QFile file(appcurrentpath+"/shellcode/changIP.sh");

    //    // 从文件读取数据
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString line;
        QTextStream in(&file);  //用文件构造流
        line = in.readLine();//读取一行放到字符串里
        //qDebug() << line;
        int i=0;
        while(!line.isNull())//字符串有内容
        {

            line=in.readLine();//循环读取下行
            if(line!="#!/bin/bash"){
                if(line!=nullptr){
                    QStringList list=line.split(" ");
                    for(int i=0;i<list.length();i++){
                        QString temp=list.at(i);
                        if(temp=="eth0"){
                            ipv4=list.at(i+1);
                            //qDebug()<<ipv4;
                        }else if(temp=="eth1"){
                            ipv42=list.at(i+1);
                            //qDebug()<<ipv42;
                        }else if(temp=="eth2"){
                            ipv43=list.at(i+1);
                            //qDebug()<<ipv43;
                        }else if(temp=="eth3"){
                            ipv44=list.at(i+1);
                            //qDebug()<<ipv44;
                        }

                    }
                    //qDebug() << line;
                }
            }




        }
    }
    file.close();





}

void MainWindow::showuseip()
{

}

void MainWindow::receivelange(QString mylange,int type)
{
    device=systemsettings->value("/systemconfig/device").toInt();
    lange=systemsettings->value("/systemconfig/lange").toString();
    ui->retranslateUi(this);
    changeDrivertype();
    //    reloadMainWindow();
}

void MainWindow::receivehavenew(int myactive)
{
    if(myactive==1){
        active=myactive;
        ui->pushButton->setIcon(QIcon(appcurrentpath+"/image/more_inter_active.png"));
        ui->pushButton->setIconSize(QSize(50,50));

    }
    qDebug()<<"mainwidow:receivehavenew";
    qDebug()<<"more_inter_active.png";
}

void MainWindow::recivesuccessformore()
{
    this->reloadMainWindow();
}

void MainWindow::onResize()
{


    setbackground(appcurrentpath+"/image/bg.png");
    applist->resizewindow(this->size());
    ui->centralwidget->resize(this->size());
    ui->verticalWidget->resize(this->size());
    ui->verticalLayout_2->setSizeConstraint(QLayout::SetFixedSize);
    ui->verticalWidget2->resize(this->size());

    if(more!=nullptr){
        connect(more, SIGNAL(sendhavenewtomainwindow(int)), this, SLOT(receivehavenew(int)));
        connect(more, SIGNAL(sendlange(QString,int)), this, SLOT(receivelange(QString,int)));
        connect(more, SIGNAL(successtomainwindow()), this, SLOT(recivesuccessformore()));
        more->onResize(this->size());
    }
    for(int i=0;i<qflist.size();i++){
        qflist.at(i)->setMinimumWidth(this->size().width());

    }
    //           qDebug() <<this->size();
    // 在这里重新调整控件布局和大小的逻辑代码

}

void MainWindow::resizeEvent(QResizeEvent *event)
{

    QMainWindow::resizeEvent(event);

    // 发送自定义的resized信号
    emit resized();

}


QList<AppsVersion *>  MainWindow::selectAppsversion()
{
    QList<AppsVersion *> list;

    if(database.open()){
        sql_query->exec("select * from appsversion order by priority desc");
    }else{
        database.open();
        sql_query->exec("select * from appsversion order by priority desc");
    }

    if(!sql_query->exec())
    {
        qDebug()<<sql_query->lastError();

    }
    else
    {

        while(sql_query->next())
        {
            AppsVersion *appsversion=new AppsVersion();
            appsversion->setId(sql_query->value(0).toInt());
            appsversion->setUuid(sql_query->value(1).toString());
            appsversion->setAppname(sql_query->value(2).toString());
            appsversion->setApppath(sql_query->value(3).toString());
            appsversion->setApprtmversion(sql_query->value(4).toInt());
            appsversion->setApprtmversionchar(sql_query->value(5).toString());

            appsversion->setApprtmupdatetime(sql_query->value(6).toInt());
            appsversion->setAppupdatedversion(sql_query->value(7).toInt());
            appsversion->setAppupdatedversionchar(sql_query->value(8).toString());

            appsversion->setAppupdatedtime(sql_query->value(9).toInt());
            appsversion->setAppupdateddesc(sql_query->value(10).toString());
            appsversion->setAppsize(sql_query->value(11).toString());
            appsversion->setAppico(sql_query->value(12).toString());
            appsversion->setAppsystem(sql_query->value(13).toString());
            appsversion->setAppdevice(sql_query->value(14).toInt());
            list.append(appsversion);
        }
    }
    return list;

}

bool MainWindow::generateStartupScript(AppsVersion *apps)
{
    //生成启动脚本到shellcode中
    try {

        QFile qfile(appcurrentpath+"/shellcode/run"+apps->getAppname().toLower()+".sh");
        QFile qfile2(appcurrentpath+"/shellcode/run"+apps->getAppname().toLower()+"en.sh");
        if(!qfile2.exists()){
            //生成脚本
            if(qfile2.open(QFile::WriteOnly | QFile::Truncate)){        //文件流写文件
                QTextStream out2(&qfile2);      //通过文本流控制文件写操作
                QString runapp;
                if(lange=="en"){
                    QFile qfile3(basepath+"/"+apps->getApppath()+"/"+apps->getAppname()+"_EN");
                    if(qfile3.exists()){
                        QString appname=apps->getAppname()=="KSTPXX"?"KSTPXQ":(apps->getAppname()=="WTKPX"?"WTKP":apps->getAppname());
                        runapp="#!/bin/bash\n COUNT=$(ps -ef |grep  "+appname+"|grep -v \"grep\" |wc -l)\n"
                                                                              "if [ $COUNT -eq 0 ]; then \n"
                                                                              "cd "+basepath+"/"+apps->getApppath()+"\n"
                                                                                                                    "./"+appname+"_EN &\n"
                                                                                                                                 "else \n"
                                                                                                                                 "echo is RUN \n"
                                                                                                                                 "fi";
                    }else{
                        QString appname=apps->getAppname()=="KSTPXX"?"KSTPXQ":(apps->getAppname()=="WTKPX"?"WTKP":apps->getAppname());
                        runapp="#!/bin/bash\n COUNT=$(ps -ef |grep  "+appname+"|grep -v \"grep\" |wc -l)\n"
                                                                              "if [ $COUNT -eq 0 ]; then \n"
                                                                              "cd "+basepath+"/"+apps->getApppath()+"\n"
                                                                                                                    "./"+appname+" &\n"
                                                                                                                                 "else \n"
                                                                                                                                 "echo is RUN \n"
                                                                                                                                 "fi";
                    }

                }else{
                    QString appname=apps->getAppname()=="KSTPXX"?"KSTPXQ":(apps->getAppname()=="WTKPX"?"WTKP":apps->getAppname());
                    runapp="#!/bin/bash\n COUNT=$(ps -ef |grep  "+appname+"|grep -v \"grep\" |wc -l)\n"
                                                                          "if [ $COUNT -eq 0 ]; then \n"
                                                                          "cd "+basepath+"/"+apps->getApppath()+"\n"
                                                                                                                "./"+appname+" &\n"
                                                                                                                             "else \n"
                                                                                                                             "echo is RUN \n"
                                                                                                                             "fi";
                }

                out2<<runapp<<endl;
            }
            qfile2.close();
        }
        if(!qfile.exists()){
            if(qfile.open(QFile::WriteOnly | QFile::Truncate)){        //文件流写文件
                QTextStream out(&qfile);      //通过文本流控制文件写操作
                QString appname=apps->getAppname()=="KSTPXX"?"KSTPXQ":(apps->getAppname()=="WTKPX"?"WTKP":apps->getAppname());
                QString runapp="#!/bin/bash\n COUNT=$(ps -ef |grep  "+appname+"|grep -v \"grep\" |wc -l)\n"
                                                                              "if [ $COUNT -eq 0 ]; then \n"
                                                                              "cd "+basepath+"/"+apps->getApppath()+"\n"
                                                                                                                    "./"+appname+" &\n"
                                                                                                                                 "else \n"
                                                                                                                                 "echo is RUN \n"
                                                                                                                                 "fi";




                out<<runapp<<endl;
            }
            qfile.close();
        }
        QString commond="chmod 777 -R  "+basepath;
        system(commond.toLocal8Bit());


        return true;
    } catch (QException e) {
        qDebug()<<e.what();
        return false;
    }


}

void MainWindow::todoshell(QString shellname,AppsVersion *appsversion)
{
    if(isshowmessage){
        QFont qf;
        qf.setPixelSize(30);
        msgBox=new QMessageBox(QMessageBox::Warning, tr("警告"), tr("存储空间过低请，即时清除空间"), QMessageBox::Ok, this);
        // //qDebug()<<"存储空间过低请，即时清除空间";
        msgBox->setFont(qf);
        msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
        msgBox->setMinimumWidth(200);
        msgBox->setMinimumHeight(200);
        msgBox->setBaseSize(200,200);

        msgBox->setFixedSize(200,200);
        msgBox->setStyleSheet("QLabel{"
                              "min-width: 200px;"
                              "min-height: 100px; "
                              "}");
        int result2 = msgBox->exec();
    }else{
        if(device==1){
            QString foldername="Upgrade";
            QDir qdir(basepath+"/"+appsversion->getApppath()+"/Upgrade");
            QDir qdir2(basepath+"/"+appsversion->getApppath()+"/UpLoadDir");


            if(qdir.exists()){
                unzipAndUploadBackground(qdir,basepath+"/"+appsversion->getApppath()+"/Upgrade",basepath+"/"+appsversion->getApppath(),shellname);
            }else if(qdir2.exists()){
                unzipAndUploadBackground(qdir2,basepath+"/"+appsversion->getApppath()+"/UpLoadDir",basepath+"/"+appsversion->getApppath(),shellname);
            }

        }else if(device==2){
            QString foldername="Upgrade";
            QDir qdir(basepath+"/"+appsversion->getApppath()+"/Upgrade");
            QDir qdir2(basepath+"/"+appsversion->getApppath()+"/UpLoadDir");
            if(qdir.exists()){
                unzipAndUploadBackground(qdir,basepath+"/"+appsversion->getApppath()+"/Upgrade",basepath+"/"+appsversion->getApppath(),appsversion);
            }else if(qdir2.exists()){
                unzipAndUploadBackground(qdir2,basepath+"/"+appsversion->getApppath()+"/UpLoadDir",basepath+"/"+appsversion->getApppath(),appsversion);
            }else{
                if(myloadingwidget!=nullptr){
                    delete (myloadingwidget);
                    myloadingwidget=nullptr;
                }
                myloadingwidget=new loadingwidget();
                myloadingwidget->setappversion(appsversion);
                myloadingwidget->show();

            }
        }


    }
}

int MainWindow:: System_Check(int result)
{
    if((-1 != result) && (WIFEXITED(result)) && (!(WEXITSTATUS(result))))

        return 0;
    else

        return -1;
}

void MainWindow::showmessage(QString txt,QString shellname){
    QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("通知"),txt);

    QFont qf;
    qf.setPixelSize(20);
    m_box->setFont(qf);
    m_box->setStyleSheet(""
                         "QPushButton {"
                         "background-color:#89AFDE;"
                         " border-style: outset;"
                         " border-width: 2px;"
                         " border-radius: 10px;"
                         " border-color: beige;"
                         " font: bold 24px;"
                         " min-width: 5em;"
                         " min-height: 2em;"
                         "}");
    QDesktopWidget *desktop=QApplication::desktop();
    int screenWidth = desktop->width();
    int screenHeight = desktop->height();

    // 计算弹窗的中心位置
    int x = (screenWidth - m_box->width()) / 2;
    int y = (screenHeight - m_box->height()) / 2;
    m_box->move(x,y);
    QTimer::singleShot(2000,m_box,[=]{
        m_box->close();
        QString commond="sh "+ appcurrentpath+"/shellcode/"+ shellname;
        const char*code=commond.toLocal8Bit();
        system(code);
    });
}


void MainWindow::showmessage(QString txt,AppsVersion *appsversion){
    QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("通知"),txt);

    QFont qf;
    qf.setPixelSize(20);
    m_box->setFont(qf);
    m_box->setStyleSheet(""
                         "QPushButton {"
                         "background-color:#89AFDE;"
                         " border-style: outset;"
                         " border-width: 2px;"
                         " border-radius: 10px;"
                         " border-color: beige;"
                         " font: bold 24px;"
                         " min-width: 5em;"
                         " min-height: 2em;"
                         "}");
    m_box->show();
    QTimer::singleShot(2000,m_box,[=]{
        m_box->close();
        if(appsversion!=nullptr){
            if(myloadingwidget!=nullptr){
                delete (myloadingwidget);
                myloadingwidget=nullptr;
            }
            myloadingwidget=new loadingwidget();

            myloadingwidget->setappversion(appsversion);
            myloadingwidget->show();
        }
    });
}

void MainWindow::checkhavenotuseip()
{
    QDialog dialog(this );
    dialog.setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    dialog.setWindowTitle(tr("未用ip"));
    dialog.resize(QSize(600,400));

    QGridLayout form(&dialog);
    QPlainTextEdit *qptedit= new QPlainTextEdit();
    ipnousedata.replace("\n","\t");
    qptedit->setPlainText(ipnousedata);
    form.addWidget(qptedit,1,1);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok ,Qt::Horizontal, &dialog);
    buttonBox.setStyleSheet("text-align:center");

    buttonBox.button(QDialogButtonBox::Ok)->setText(tr("关闭"));
    QFont qf2;
    qf2.setPixelSize(25);
    buttonBox.button(QDialogButtonBox::Ok)->setMinimumHeight(50);
    buttonBox.button(QDialogButtonBox::Ok)->setFont(qf2);
    buttonBox.button(QDialogButtonBox::Ok)->setMinimumWidth(150);
    buttonBox.button(QDialogButtonBox::Ok)->setStyleSheet("background-color:rgb(120,120,255); border-radius:5px;color:rgb(255,255,255);");
    form.addWidget(&buttonBox,2,1);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        dialog.close();
    }


}
void MainWindow::unzipAndUploadBackground(QDir qdir,QString floderpath,QString resultpath,QString shellname){


    QFileInfoList templist = qdir.entryInfoList();
    if (templist.count() >0&&!qdir.isEmpty())
    {
        QStringList qdirtemplist;
        QDirIterator it(floderpath, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::AllDirs, QDirIterator::Subdirectories);
        QDirIterator it2(floderpath, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::AllDirs, QDirIterator::Subdirectories);

        while(it.hasNext())
        {
            it.next();
            qdirtemplist.append(it.fileInfo().absoluteFilePath());
            //                        qDebug()<<it.fileInfo().absoluteFilePath();
        }
        if(qdirtemplist.size()>0){
            for(QString str:qdirtemplist){
                if(str.contains(".zip")){
                    //                                qDebug()<<str;
                    QString  code= "unzip -o "+str+" -d "+ floderpath;
                    const char* commond=code.toLocal8Bit().constData();
                    //                    qDebug()<<"解压:"+code;
                    system(commond);
                    QString  code2= "rm -rf "+str;
                    qDebug()<<"清空:"+code2;
                    const char* commond2=code2.toLocal8Bit().constData();
                    system(commond2);
                }
            }

            int result=-1;
            QDir qdir(floderpath);
            //            qDebug()<<floderpath+"--------------------";
            //            qDebug()<<QString::number(qdir.entryInfoList().count())+"----------------";
            if(qdir.entryInfoList().count()>0){
                QString movefile="mv -f "+floderpath+"/*"+" "+resultpath;
                const char* commond=movefile.toLocal8Bit().constData();
                result=system(commond);

                if(!(System_Check(result))){
                    showmessage("更新成功,启动程序",shellname);
                }else{
                    showmessage("更新失败,启动程序",shellname);
                }
            }else{
                QString commond="sh "+ appcurrentpath+"/shellcode/"+ shellname;
                const char*code=commond.toLocal8Bit();
                system(code);
            }

        }




    }
    else{
        QString commond="sh "+ appcurrentpath+"/shellcode/"+ shellname;
        const char*code=commond.toLocal8Bit();
        system(code);
    }



}

void MainWindow::unzipAndUploadBackground(QDir qdir,QString floderpath,QString resultpath,AppsVersion *appsversion){

    QFileInfoList templist = qdir.entryInfoList();
    if (templist.count() >0&&!qdir.isEmpty())
    {
        QStringList qdirtemplist;
        QDirIterator it(floderpath, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::AllDirs, QDirIterator::Subdirectories);
        while(it.hasNext())
        {
            it.next();
            qdirtemplist.append(it.fileInfo().absoluteFilePath());
        }
        if(qdirtemplist.size()>0){
            for(QString str:qdirtemplist){
                if(str.contains(".zip")){
                    QString  code= "unzip -o "+str+" -d "+floderpath;
                    const char* commond=code.toLocal8Bit().constData();
                    system(commond);
                    QString  code2= "rm -rf "+str;
                    const char* commond2=code2.toLocal8Bit().constData();
                    system(commond2);
                }
            }
        }
        QDir tempqd(floderpath);
        if(!tempqd.isEmpty()){
            int result=-1;
            QString movefile="mv -f "+floderpath+"/*"+" "+basepath+"/"+appsversion->getApppath();
            const char* commond=movefile.toLocal8Bit().constData();
            result=system(commond);

            if(!(System_Check(result))){
                showmessage("更新成功,启动程序",appsversion);
            }else{
                showmessage("更新失败,启动程序",appsversion);
            }
        }else{
            if(myloadingwidget!=nullptr){
                delete (myloadingwidget);
                myloadingwidget=nullptr;
            }
            myloadingwidget=new loadingwidget();
            myloadingwidget->setappversion(appsversion);
            myloadingwidget->show();

        }




    }else{
        if(myloadingwidget!=nullptr){
            delete (myloadingwidget);
            myloadingwidget=nullptr;
        }
        myloadingwidget=new loadingwidget();
        myloadingwidget->setappversion(appsversion);
        myloadingwidget->show();


    }
    //    else{
    //        QString commond="sh "+ appcurrentpath+"/shellcode/"+ shellname;
    //        const char*code=commond.toLocal8Bit();
    //        system(code);
    //    }



}
