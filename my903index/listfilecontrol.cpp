#include "listfilecontrol.h"
#include "ui_listfilecontrol.h"
#include <QDir>
#include <dialog.h>

ListFileControl::ListFileControl(QWidget *parent,QString title,QStringList fPath,QStringList f) :
    QWidget(parent),
    ui(new Ui::ListFileControl)
{

    ui->setupUi(this);
    this->titleString=title;
    this->filePath=fPath;
    this->file=f;
    ui->label_TItle->setText(this->titleString);

    loadFile();
}

void ListFileControl::loadFile()
{
//    if(this->filePath.isEmpty())
//        return;
//     QDir dir(this->filePath);
//    if(!dir.exists())
//        return;
    int t=0;
    int j=0;
    int b=0;
    int z=0;
    int o=0;

    for (int i =0;i<this->file.count();++i) {
        if(!QFile::exists(this->file.at(i)))
            continue;
        if((QFileInfo(this->file.at(i)).suffix()).toUpper()=="TIF")
        {
            t++;
            lt.append(this->file.at(i));
        }
        else if((QFileInfo(this->file.at(i)).suffix()).toUpper()=="JPEG")
        {
            j++;
            lj.append(this->file.at(i));
        }
       else  if((QFileInfo(this->file.at(i)).suffix()).toUpper()=="BMP")
        {
            b++;
            lb.append(this->file.at(i));
        }
        else if((QFileInfo(this->file.at(i)).suffix()).toUpper()=="ZIP")
        {
            z++;
            lz.append(this->file.at(i));
        }
        else
        {
            o++;
            lo.append(this->file.at(i));
        }
    }

    for (int var = 0; var < this->filePath.count(); ++var) {
        if(this->filePath.at(var).isEmpty())
            continue;
        QDir dir(this->filePath.at(var));
        if(!dir.exists())
                continue;
        QFileInfoList file_info_list = GetFileList(this->filePath.at(var));
         if(file_info_list.count()>0)
         {

             QString filter;
             foreach(QFileInfo fileinfo, file_info_list)
             {
                 //_filter = _fileinfo.completeSuffix();
                 filter = fileinfo.suffix();    //后缀名
                 if(filter.toUpper()=="TIF")
                 {
                     t++;
                     lt.append(fileinfo.filePath());
                 }
                 else if(filter.toUpper()=="JPG"||filter.toUpper()=="JPEG")
                 {
                     j++;
                     lj.append(fileinfo.filePath());
                 }
                else  if(filter.toUpper()=="BMP")
                 {
                     b++;
                     lb.append(fileinfo.filePath());
                 }
                 else if(filter.toUpper()=="ZIP")
                 {
                     z++;
                     lz.append(fileinfo.filePath());
                 }
                 else
                 {
                     o++;
                     lo.append(fileinfo.filePath());
                 }
             }


         }
    }

    ui->label_tifCount->setText(QString::number(t));
    ui->label_jpgCount->setText(QString::number(j));
    ui->label_bmpCount->setText(QString::number(b));
    ui->label_zipCount->setText(QString::number(z));
    ui->label_otherCount->setText(QString::number(o));
}

//递归遍历文件夹
QFileInfoList ListFileControl::GetFileList(QString path)
{
    QDir dir(path);
    //列出dir(path)目录文件下所有文件和目录信息，存储到file_list容器
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    //列出dir(path)目录下所有子文件夹
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    //进行子文件夹folder_list递归遍历，将内容存入file_list容器
    for(int i= 0; i != folder_list.size(); i++)
    {
        QString name = folder_list.at(i).absoluteFilePath();
        QFileInfoList child_file_list = GetFileList(name);
        file_list.append(child_file_list);
    }
    return file_list;
}



ListFileControl::~ListFileControl()
{
    delete ui;
}



void ListFileControl::on_pushButton_delTif_clicked()
{
    if(lt.count()<1)
        return;
    Dialog *d=new Dialog(nullptr,QString(tr("是否清除《")+ui->label_TItle->text()+tr("》TIF文件？")));
    if(d->exec()==QDialog::Rejected)
        return;
    for (int var = 0; var < lt.count(); ++var) {
        QFile f(lt.at(var));
        if(f.exists())
            f.remove();
    }
    loadFile();
}

void ListFileControl::on_pushButton_deljpg_clicked()
{
    if(lj.count()<1)
        return;
    Dialog *d=new Dialog(nullptr,QString(tr("是否清除《")+ui->label_TItle->text()+tr("》JPG文件？")));
    if(d->exec()==QDialog::Rejected)
        return;
    for (int var = 0; var < lj.count(); ++var) {
        QFile f(lj.at(var));
        if(f.exists())
            f.remove();
    }
     loadFile();
}

void ListFileControl::on_pushButton_delBMP_clicked()
{
    if(lb.count()<1)
        return;
    Dialog *d=new Dialog(nullptr,QString(tr("是否清除《")+ui->label_TItle->text()+tr("》BMP文件？")));
    if(d->exec()==QDialog::Rejected)
        return;
    for (int var = 0; var < lb.count(); ++var) {
        QFile f(lb.at(var));
        if(f.exists())
            f.remove();
    }
     loadFile();
}

void ListFileControl::on_pushButton_delZIP_clicked()
{
    if(lz.count()<1)
        return;
    Dialog *d=new Dialog(nullptr,QString("是否清除《"+ui->label_TItle->text()+"》ZIP文件？"));
    if(d->exec()==QDialog::Rejected)
        return;
    for (int var = 0; var < lz.count(); ++var) {
        QFile f(lz.at(var));
        if(f.exists())
            f.remove();
    }
     loadFile();
}

void ListFileControl::on_pushButton_delother_clicked()
{
    if(lo.count()<1)
        return;
    Dialog *d=new Dialog(nullptr,QString("是否清除《"+ui->label_TItle->text()+"》其它文件？"));
    if(d->exec()==QDialog::Rejected)
        return;
    for (int var = 0; var < lo.count(); ++var) {
        QFile f(lo.at(var));
        if(f.exists())
            f.remove();
    }
     loadFile();
}

void ListFileControl::on_pushButton_Del_clicked()
{
    Dialog *d=new Dialog(nullptr,QString("是否清除《"+ui->label_TItle->text()+"》所有文件？"));
    if(d->exec()==QDialog::Rejected)
        return;
    for (int var = 0; var < lt.count(); ++var) {
        QFile f(lt.at(var));
        if(f.exists())
            f.remove();
    }
    for (int var = 0; var < lj.count(); ++var) {
        QFile f(lj.at(var));
        if(f.exists())
            f.remove();
    }
    for (int var = 0; var < lb.count(); ++var) {
        QFile f(lb.at(var));
        if(f.exists())
            f.remove();
    }
    for (int var = 0; var < lz.count(); ++var) {
        QFile f(lz.at(var));
        if(f.exists())
            f.remove();
    }
    for (int var = 0; var < lo.count(); ++var) {
        QFile f(lo.at(var));
        if(f.exists())
            f.remove();
    }
    loadFile();
}

void ListFileControl::changelabel(QString label)
{
    ui->label->setText(label);
}
