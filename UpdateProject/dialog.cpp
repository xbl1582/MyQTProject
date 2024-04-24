#include "dialog.h"
#include "ui_dialog.h"
#include "QTextCodec"
#include "dialog.h"
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    Qt::WindowFlags m_flags = windowFlags();
    this->setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
    system("chmod -Rf 777 /mnt/lib/APP");
 msgBox=new QMessageBox(QMessageBox::Information, tr("更新设备"), "", QMessageBox::Ok, this);
      updatewidget=new UpdateWidget();
       apppath=readJsonfloadname("apppath");
       updatepath=readJsonfloadname("updatepath");

updatewidget->show();
    timer =new QTimer();
    connect(timer, &QTimer::timeout,
            this,[=]{
        initupdateproject();
    });
    timer->start(1000);

    timer2 =new QTimer();
    connect(timer2, &QTimer::timeout,
            this,[=]{
        checkfolde();
    });
    timer2->start(2000);
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}
// update project
/**
 * @brief Dialog::initupdateproject
 * @return
 * thinking:
 *      step1: want to get a foldername
 *      step2: let this function to listen in the foldername what is update peroject .
 *      step3: this folder has some different zip, and we will Update specific programs based on  zip name.
 *             if has a update.sh file ,we will run it at the same time.
 *      step4: message the user ,this boot will be reboot.
 * todo:
 *      step1: create a timer ,it be used to listen a folder that name is "update"
 *
 *
 *
 */
bool Dialog::initupdateproject()
{
    bool falg=checkupdatefolder();
    if(falg){
         if(myresult){
            QThread::msleep(10000);
              QString str="rm -rf /mnt/lib/APP/update";
               const char* commond=str.toLocal8Bit().constData();
              system(commond);
              myresult=false;
            system("reboot");
         }

    }else{
        /*
         * open the KSTMAIN perject
         */
          timer->stop();
          delete(timer);
          timer=nullptr;
        if(isactive){
             updatewidget->hide();
            QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
            QString str = dateTime .toString("yyyy_MM_dd_hh_mm_ss");//格式化时间
            QString logtext=QCoreApplication::applicationDirPath()+"/log/start_"+str+".txt";
             str = dateTime .toString("yyyy-MM-dd hh:mm:ss");//格式化时间
             QString logcontent=str+" Open KSTMAIN--";
             writelog(logtext,logcontent);
              QString thisprojectpath =QCoreApplication::applicationDirPath()+"/shellcode/";
              qDebug()<<thisprojectpath;
              QProcess *process=new QProcess();
               QString workspace=thisprojectpath;
               qDebug()<<workspace;
              QString shellname="sh openkstmain.sh";
               process->setWorkingDirectory(workspace);
               process->start(shellname);
               if(process->waitForFinished()){
                   process->close();

               }
            isactive=false;

        }
    }
     myresult=false;
}
/**
 * @brief Dialog::checkupdatefolder
 * @return
 * introdution: this function function is used to check folder whether has zip ,if it has zip todo update ,it hasn't zip not to do anything.
 *
 *
 */
bool Dialog::checkupdatefolder()
{

            zipnum();
            updatewidget->setbarmaxnum(filenums);
//            QString thisprojectpath =QCoreApplication::applicationDirPath();
               // hendle the folder path ,we need a folder is '/update',it is one layer above the current position
//            QStringList tempstringlist=thisprojectpath.split("/");
            QString result="";
            QString result2="";

            result2=apppath;
              result=updatepath;
            bool falg=false;

            for(int i=0;i<folderpath.size();i++){

               QString folderpath2=folderpath.at(i);
               QStringList folderlist=folderpath2.split("/");
               if(folderlist.at(folderlist.size()-1)==folderlist.at(folderlist.size()-2)){

                 falg=copyfile(folderpath.at(i), result2);
                }else{
                   QString foldername=folderlist.at(folderlist.size()-1).split("_").at(0);
                   qDebug()<<foldername;
                   QDir temdir(result2+"/"+foldername);
                   if(!temdir.exists()){
                       temdir.mkdir(result2+"/"+foldername);


                   }
                    falg=copyfile(folderpath.at(i),  result2+"/"+foldername);

               }
                 QString str="rm -rf "+folderpath.at(i);
                 const char* commond=str.toLocal8Bit().constData();
                 system(commond);
                 falg=true;
                folderindex++;
            }
            myresult=falg;

        return  falg;






}

bool Dialog::unzip(QString zipPath, QString zipDir,int todo)
{

    QDir tempDir;
      if(!tempDir.exists(zipDir)) tempDir.mkdir(zipDir);
      QZipReader reader(zipPath);
      QString commond;
      if(todo==1){
           commond="unzip "+zipPath+" -d "+zipDir;
      }else if(todo==2){
          commond="tar -xvf "+zipPath+" -C "+zipDir;
          qDebug()<<commond<<"--------------";
      }else if(todo==3){
          commond="tar -xzvf "+zipPath+" -C "+zipDir;
      }

      std::string str = commond.toStdString();
      const char* ch = str.c_str();
      myresult= QProcess::execute(ch)==0?true:false;
////      bool istrue=reader.extractAll(zipDir);
      QDateTime dateTime2= QDateTime::currentDateTime();//获取系统当前的时间
      QString str3 = dateTime2 .toString("yyyy_MM_dd_hh_mm_ss");//格式化时间
      QString logtext2=QCoreApplication::applicationDirPath()+"/log/unzip_"+str3+".txt";
       str3 = dateTime2 .toString("yyyy-MM-dd hh:mm:ss");//格式化时间
       QString r=myresult?"true":"false";
       QString logcontent2=str3+" update file--"+zipPath+"----"+r;
       writelog(logtext2,logcontent2);

      QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
      QString str2 = dateTime .toString("yyyy_MM_dd_hh_mm_ss");//格式化时间
      QString logtext=QCoreApplication::applicationDirPath()+"/log/delete_"+str2+".txt";
       str2 = dateTime .toString("yyyy-MM-dd hh:mm:ss");//格式化时间
       bool myresult2=QFile::remove(zipPath);
       QString logcontent=str2+" delete file--"+zipPath+"----"+myresult2;
       writelog(logtext,logcontent);


      return myresult &&myresult2;
}



void Dialog::writelog(QString logpath,QString content)
{
    //写日志
                    QFile file(logpath);  //创建文件对象


                   file.open(QIODevice::ReadWrite|QIODevice::Text);	//以读且写的方式打开

                   QTextCodec *codec=QTextCodec::codecForName("UTF-8");	 //防止乱码

                   QByteArray array=file.readAll();
                               //读取原文件内容（必须先读出来，才不会被新内容## 标题覆盖掉)

                   QString str=content;	//字符串，名字可变，这里只是举例

                   QTextStream out(&file);		//输入流

                   out<<str<<"\n";			//写入内容

                   file.close();              //关闭文件流

    //写日志
}




bool Dialog::copyfile(QString src, QString dst)
{

     bool falg;
        QDir srcDir(src);
        QDir dstDir(dst);

        if (!dstDir.exists()) {
            dstDir.mkdir(dst);
        }

        QFileInfoList list = srcDir.entryInfoList();

        foreach (QFileInfo info, list) {
            if (info.fileName() == "." || info.fileName()== "..") {
                continue;
            }
            if (info.isDir()) {
                // 创建文件夹，递归调用该函数
                QString srcfolder=src.split("/").last();
                if(info.fileName().split("_").at(0)==srcfolder&&diguitime==0){

                    copyfile(info.filePath(), dst);
                }else{
                     copyfile(info.filePath(), dst + "/" + info.fileName());
                }


                continue;
            }
            // 文件拷贝
            QFile file(info.filePath());
            QFile resultfile(dst + "/" + info.fileName());
            if(resultfile.exists()){
                QFile::remove(dst + "/" + info.fileName());
            }

           falg=file.copy(dst + "/" + info.fileName());

           QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
           QString str = dateTime .toString("yyyy_MM_dd_hh_mm_ss");//格式化时间
           QString logtext=QCoreApplication::applicationDirPath()+"/log/log_"+str+".txt";

            str = dateTime .toString("yyyy-MM-dd hh:mm:ss");//格式化时间
            QString results=falg?"success":"fail";
           QString logcontent=str+"-copy file -"+info.filePath()+"--to---"+dst + "/" + info.fileName()+"--result--"+results;
           writelog(logtext,logcontent);
           if(falg){

               if(probarvalue<=filenums){
                   probarvalue++;
               }
           }
           updatewidget-> changeprogressbarvalue(probarvalue);

           }


}

void Dialog::zipnum()
{
    //setp1:get the uodate folder path

        //get the poject path
     QString result="";
     QString result2="";
     result2=apppath;
       result=updatepath;
        //is the update is not exist ,we will create it before we use it.
     QDir qdir(result);
    if(!qdir.exists()){
       qdir.mkdir(result);
       // open the KSTMAIN project

    }else{
        // is the update folder is exist ,we will read this folder and check it whether has zip file. if hasn't noting ,
        //start the KSTMAIN project. else  it will do the copy operations and message user reboot the machine.
        QStringList dir_list = qdir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

        int dir_count=dir_list.size();
        for(int j=0;j<dir_count;j++){
            folderpath.append(result+"/"+dir_list.at(j));
        }
        qdir.setFilter(QDir::Files | QDir::NoSymLinks);
           QFileInfoList list = qdir.entryInfoList();

           int file_count = list.count();
           if(file_count <= 0&&dir_count<=0)
           {
               filenums=0;
               folderpath.clear();
               return ;
           }

           QStringList string_list;
           for(int i=0; i<file_count; i++)
           {
               QFileInfo file_info = list.at(i);
               QString suffix = file_info.suffix();
               if(QString::compare(suffix, QString("zip"), Qt::CaseInsensitive) == 0)
               {
               appfoldpath(file_info,result,1);
               }
               if(QString::compare(suffix, QString("tar"), Qt::CaseInsensitive) == 0)
               {
                    appfoldpath(file_info,result,2);
               }
               if(QString::compare(suffix, QString("gz"), Qt::CaseInsensitive) == 0)
               {
                 appfoldpath(file_info,result,3);
               }
           }
    }
}

void Dialog::checkfolde()
{

    QString result="";
    QString result2="";
    result2=apppath;
      result=updatepath;
       //is the update is not exist ,we will create it before we use it.
    QDir qdir(result);
   if(!qdir.exists()){
      qdir.mkdir(result);
      // open the KSTMAIN project

   }else{

       // is the update folder is exist ,we will read this folder and check it whether has zip file. if hasn't noting ,
       //start the KSTMAIN project. else  it will do the copy operations and message user reboot the machine.
//       QStringList dir_list = qdir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

//       int dir_count=dir_list.size();
       QTime _Timer = QTime::currentTime().addMSecs(5000);
       while( QTime::currentTime() < _Timer )
              QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
       QFile qf(result+"/update.sh");
       if(qf.exists()){
               QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
               QString str = dateTime .toString("yyyy_MM_dd_hh_mm_ss");//格式化时间
               QString logtext=QCoreApplication::applicationDirPath()+"/log/dosh_"+str+".txt";
                str = dateTime .toString("yyyy-MM-dd hh:mm:ss");//格式化时间
                QString logcontent=str+" do update.sh--"+result+"/update.sh";
                writelog(logtext,logcontent);
               QProcess *process=new QProcess();
                QString workspace=result+"/";
                qDebug()<<workspace;
                QString shcode="sh update.sh";
                process->setWorkingDirectory(workspace);
                process->start(shcode);
                if(process->waitForFinished()){
                    process->close();
                }
                QFile::remove(result+"/update.sh");

       }

       qdir.setFilter(QDir::Files | QDir::NoSymLinks|QDir::Dirs | QDir::NoDotAndDotDot);

          QFileInfoList list = qdir.entryInfoList();

          int file_count = list.count();
          if(file_count <= 0)
          {
              filenums=0;
              folderpath.clear();
              return ;
          }
          QStringList templabl;
//          for(int j=0;j<dir_count;j++){
//              templabl.append(changename(dir_list.at(j)));
//          }
          for(int o=0;o<file_count;o++){
              templabl.append(changename(list.at(o).fileName()));
          }

           bool falg=false;
          filecount=file_count;
          for(int i=0; i<file_count; i++)
          {
              timer2->stop();
              delete(timer2);
              timer2=nullptr;

              QFileInfo file_info = list.at(i);

              QString suffix = file_info.suffix();

              if(file_info.isDir()||QString::compare(suffix, QString("zip"), Qt::CaseInsensitive) == 0
                      ||QString::compare(suffix, QString("tar"), Qt::CaseInsensitive) == 0
                    ||QString::compare(suffix, QString("gz"), Qt::CaseInsensitive) == 0)
              {

                  showmessage(templabl);
                  break;
                  return;




              }

        }


   }



}




void Dialog::filenum(QString src)
{


        QDir srcDir(src);

        QFileInfoList list = srcDir.entryInfoList();

        foreach (QFileInfo info, list) {

            if (info.fileName() == "." || info.fileName()== "..") {

                continue;
            }
            if (info.isDir()) {
                // 创建文件夹，递归调用该函数
                filenum(info.filePath());
                continue;
            }
            // 文件拷贝
            QFile file(info.filePath());
              filenums++;

            if(info.fileName()=="update.sh"){
                QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
                QString str = dateTime .toString("yyyy_MM_dd_hh_mm_ss");//格式化时间
                QString logtext=QCoreApplication::applicationDirPath()+"/log/dosh_"+str+".txt";
                 str = dateTime .toString("yyyy-MM-dd hh:mm:ss");//格式化时间
                 QString logcontent=str+" do update.sh--"+src+"/update.sh";
                 writelog(logtext,logcontent);
                QProcess *process=new QProcess();
                 QString workspace=src+"/";
                 qDebug()<<workspace;
                 QString shcode="sh update.sh";
                 process->setWorkingDirectory(workspace);
                 process->start(shcode);
                 if(process->waitForFinished()){
                     process->close();
                 }

            }



        }
}
void Dialog:: appfoldpath(QFileInfo file_info,QString result,int todo){
    QString absolute_file_path = file_info.absoluteFilePath();
//                   string_list.append(absolute_file_path);
    QStringList templist=absolute_file_path.split("/");
    QString dirnametemp=templist.at(templist.size()-1);
    QStringList dirnametemplist=dirnametemp.split(".");
    QString dirname=dirnametemplist.at(0);
    QStringList dirnamenamelist=dirname.split("_");
    QString dirname2=dirnamenamelist.at(0);
    bool istrue=unzip(absolute_file_path, result+"/"+dirname2,todo);
    folderpath.append(result+"/"+dirname2);

    if(istrue){
         filenum(result+"/"+dirname2);
    }
}

void Dialog::  showmessage(QStringList labels){
    QString labl="请确保拷贝完成后，点击确认更新[";
   for(int i=0;i<labels.size();i++){
       if(i==labels.size()-1){
           labl+=labels.at(i);
       }else
       {
       labl+=labels.at(i)+",";
       }
   }
   labl+="]";
   QFont qf;
   qf.setPixelSize(25);

  msgBox->setText(labl);
  msgBox->setFont(qf);
  msgBox->setStyleSheet(""
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
    msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
    if (msgBox->exec() == QMessageBox::Ok){


        updatewidget->close();
        system("reboot");
    }
}
QString Dialog:: readJsonfloadname(QString key){
    //读取json文件
    QString filepath=QCoreApplication::applicationDirPath()+"/json/pathjson.json";
    QFile loadFile(filepath);
          if(!loadFile.open(QIODevice::ReadOnly| QIODevice::Text))
          {

                 return 0;
          }
    QByteArray array = loadFile.readAll(); //将数据读到array中
    loadFile.close();
    //json错误抛出
    QJsonParseError json_error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(array,&json_error);
    if(json_error.error != QJsonParseError::NoError)//判断是否可以解析
     {
           qDebug()<<"json error!";
           return 0;
      }
     QJsonObject rootObj = jsonDoc.object();
     QJsonValue foldepath = rootObj.value(key);
     QString folderstring=foldepath.toString();
     return folderstring;
}
QString Dialog::changename(QString foldname){
    QString folderpath2=foldname;
    QStringList folderlist=folderpath2.split("/");
    QString foldername2=folderlist.at(folderlist.size()-1).split("_").at(0);
    if(foldername2== "KSTMAIN"){
       return "主页面";
    }else if(foldername2=="KSTPXQ"){
        return "计票";
    }else if(foldername2=="WTKP"){
        return"测评";
    }else if(foldername2=="KSTSCAN"){
     return"扫描";
    }else{
      return foldername2;
    }


}
