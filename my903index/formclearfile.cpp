#include "formclearfile.h"
#include "ui_formclearfile.h"

FormClearFile::FormClearFile(QWidget *parent,QString t1,QStringList p1,QStringList f1,QString t2,QStringList p2,QStringList f2,QString t3,QStringList p3,QStringList f3) :
    QWidget(parent),
    ui(new Ui::FormClearFile)
{
    ui->setupUi(this);
    this->title1=t1;
    this->title2=t2;
    this->title3=t3;
    this->path1=p1;
    this->path2=p2;
    this->path3=p3;
    this->f1=f1;
    this->f2=f2;
    this->f3=f3;
    process=new QProcess();
   l1=new ListFileControl(nullptr,title1,path1,this->f1);
    ui->verticalLayout_t1->addWidget(l1);

    l2=new ListFileControl(nullptr,title2,path2,this->f2);
    ui->verticalLayout_t2->addWidget(l2);
    l3=new ListFileControl(nullptr,title3,path3,this->f3);
    ui->verticalLayout_t3->addWidget(l3);
    timer=new QTimer(this);//创建一个定时器
    checkfoldermemory();
    connect(timer, &QTimer::timeout,
            this,[=]{
        checkfoldermemory();
    });
        timer->start(3000);

    Qt::WindowFlags m_flags = windowFlags();
    setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);

}

FormClearFile::~FormClearFile()
{
    delete ui;
}

void FormClearFile::on_pushButton_clicked()
{
    QWidget::close();
}
QString FormClearFile::checkmemory(QString shellpath,QString shellname,QString filepath)
{
    //设置内存大小数值


    //QCoreApplication::applicationDirPath()+"/shellcode";
     QString workspace=shellpath;
     QString shcode="sh "+shellname;
     process->setWorkingDirectory(workspace);
     process->start(shcode);
     if(process->waitForFinished()){
         process->close();
     }
     QFile f(QCoreApplication::applicationDirPath()+"/shellcode/"+filepath);
     if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
     {

     }
     QTextStream txtInput(&f);
     QString lineStr;
     while(!txtInput.atEnd())
     {
         lineStr = txtInput.readLine();

     }
     f.close();
    QString memory=lineStr.split(" ").at(0);
    return memory;


}
void FormClearFile::checkfoldermemory(){
        QString memory=checkmemory(QCoreApplication::applicationDirPath()+"/shellcode","GetScanFolderSize.sh","ScanDisk.txt");
        l1->changelabel(tr("占用:")+memory);//
        QString memory2=checkmemory(QCoreApplication::applicationDirPath()+"/shellcode","GetVoteFolderSize.sh","VoteDisk.txt");
        l2->changelabel(tr("占用:")+memory2);//
        QString memory3=checkmemory(QCoreApplication::applicationDirPath()+"/shellcode","GetWtKpFolderSize.sh","WtkpDisk.txt");
        l3->changelabel(tr("占用:")+memory3);

        QProcess *process=new QProcess();
         QString workspace=QCoreApplication::applicationDirPath()+"/shellcode";
         QString shcode="sh GetDisk.sh";
         process->setWorkingDirectory(workspace);
         process->start(shcode);
         if(process->waitForFinished()){
             process->close();
         }
         QFile f(QCoreApplication::applicationDirPath()+"/shellcode/GetDisk_1.txt");
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
        QString memory4=lineStr;

//        ui->pushButton_2

        ui->pushButton_2->setText(tr("剩余存储容量:")+memory4);
        ui->pushButton_2->resize(200,150);
         ui->pushButton_2->setStyleSheet("background-color:#FFFFFF;color:black;");
        QFont qf3;
        qf3.setPointSize(10);
        ui->pushButton_2->setFont(qf3);



}

