#include "mythread.h"
#include "mythread.h"

MyThread::MyThread(QString requesturl,Zipobj *zipobj): QThread()
{
    requesturl2=requesturl;
    zipobj2=zipobj;
}

MyThread::MyThread() : QThread()
{

}

MyThread::~MyThread()
{

}

void MyThread::run(){
    //获取程序位置
    basepath=QCoreApplication::applicationDirPath();
    settings = new QSettings (basepath+"/config/ipserviceconfig.ini", QSettings::IniFormat);//构建函数
    //    apps = new QSettings (basepath+"/config/apps.ini", QSettings::IniFormat);//构建函数

    QDir dir(basepath+"/zip");
    if(!dir.exists()){
        dir.mkdir(basepath+"/zip");
    }
    processvalue+=10;
    emit reportProgress(processvalue);
    //获取app根目录
    QStringList qlist=basepath.split("/");
    for (int i=0;i<qlist.count()-1;i++) {
        apppath+=qlist.at(i)+"/";
    }
    processvalue+=10;
    emit reportProgress(processvalue);
    QString downloadpath=settings->value("/router/downloadpath").toString();
    QString ip=settings->value("/ipserviceconfig/ip").toString();
    int port=settings->value("/ipserviceconfig/port").toInt();
    QString urltype=settings->value("/ipserviceconfig/urltype").toString();

    for(;;){

        QString request=urltype+"://"+ip+":"+QString::number(port)+downloadpath+"?path="+zipobj2->getPath()+"/"+zipobj2->getZipname();
        //         qDebug()<<"循环";
        bool flag=download(request,zipobj2);
        if(flag){
            break;
        }else{

            QFile qfile(basepath+"/zip/"+zipobj2->getZipname());
            if(qfile.exists()){
                qfile.remove();
            }else{
                continue;
            }
        }
    }
    myunzip(zipobj2);
}

bool MyThread::download(QString request,Zipobj *zipobj){
    bool flag=getdownfun(request,zipobj->getZipname());

    if(flag){

        QString zippath=basepath+"/zip/"+zipobj->getZipname();
        QFile qfile(zippath);
        if(qfile.exists()){
            //判断md5
            processvalue+=1;
            emit reportProgress(processvalue);
            if(checkzip(zipobj->getMd5code(),zippath)){
                qDebug()<<"18.下载包:"+zipobj->getZipname()+"成功;";
                processvalue+=30;
                emit reportProgress(processvalue);
                return true;
            }

        }else{
            qDebug()<<"18.下载包:"+zipobj->getZipname()+"失败;";
            return false;
        }
    }

}

void MyThread::myunzip(Zipobj *zipobj)
{
    QString commonde;
    qDebug()<<"19.解压安装包"+zipobj->getZipname();
    commonde="unzip -o "+basepath+"/zip/"+zipobj->getZipname()+" -d "+apppath;
    qDebug()<<"20."+commonde;

    if(zipobj->getVersiontype()=="updated"){
        QDir dirtemp(apppath+zipobj->getZipname().split("_")[0]);
        if(!dirtemp.exists()){
            dirtemp.mkdir(apppath+zipobj->getZipname().split("_")[0]);
        }
        commonde="unzip -o  "+basepath+"/zip/"+zipobj->getZipname()+" -d "+apppath+zipobj->getZipname().split("_")[0];
        qDebug()<<"21."+commonde;

    }
    const char* test=commonde.toLocal8Bit().constData();
    system(test);

    //完整包
    QDir dir3(apppath+zipobj->getZipname().split("\.zip")[0]);
    QString apppath2=apppath+zipobj->getZipname().split("_")[0];
    if(zipobj->getVersiontype()=="rtm"&&dir3.exists()){
        QDir tempdir(apppath2);
        tempdir.setFilter(QDir::Files);
        QDir tempdir2(apppath2+"/data");
        if(tempdir2.exists()){
            QString commondetemp="cp -rf "+apppath2+"/data"+" "+ apppath+"/KSTMAIN/lib/"+zipobj->getPath()+"/";
            qDebug()<<"24."+commondetemp;
            const char* testtemp=commondetemp.toLocal8Bit().constData();
            system(testtemp);
        }
        QFileInfoList list = tempdir.entryInfoList();
        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            if(fileInfo.fileName()=="libyy.so"||fileInfo.fileName()=="DB.db"){
                QFile q(fileInfo.path());
                QDir qd(apppath+"/KSTMAIN/lib/"+zipobj->getPath());
                if(!qd.exists()){
                    qd.mkdir(apppath+"/KSTMAIN/lib/"+zipobj->getPath());
                }
                QString commondetemp="cp -rf "+fileInfo.path()+"/"+fileInfo.fileName() +" "+ apppath+"/KSTMAIN/lib/"+zipobj->getPath()+"/";
                qDebug()<<"24."+commondetemp;
                const char* testtemp=commondetemp.toLocal8Bit().constData();
                system(testtemp);
            }
        }

        if(tempdir.exists()){
            tempdir.removeRecursively();
        }
        QString commonde2="mv "+apppath+zipobj->getZipname().split("\.zip")[0] +" "+ apppath2;
        qDebug()<<"24."+commonde2;
        const char* test2=commonde2.toLocal8Bit().constData();
        system(test2);
        QFile qfile(apppath+"/KSTMAIN/lib/"+zipobj->getPath()+"/libyy.so");
        QFile qfile2(apppath+"/KSTMAIN/lib/"+zipobj->getPath()+"/DB.db");
        QDir dir2(apppath+"/KSTMAIN/lib/"+zipobj->getPath()+"/data");
        if(qfile.exists()){
            QString commondetemp="cp -rf "+apppath+"/KSTMAIN/lib/"+zipobj->getPath()+"/libyy.so "+" "+ apppath+zipobj->getPath()+"/";
            qDebug()<<"24."+commondetemp;
            const char* testtemp=commondetemp.toLocal8Bit().constData();
            system(testtemp);

        }
        if(qfile2.exists()){
            QString commondetemp="cp -rf "+apppath+"/KSTMAIN/lib/"+zipobj->getPath()+"/DB.db "+" "+ apppath+zipobj->getPath()+"/";
            qDebug()<<"24."+commondetemp;
            const char* testtemp=commondetemp.toLocal8Bit().constData();
            system(testtemp);

        }
        if(dir2.exists()){
            QString commondetemp="cp -rf "+apppath+"/KSTMAIN/lib/"+zipobj->getPath()+"/data"+" "+ apppath+zipobj->getPath()+"/";
            qDebug()<<"24."+commondetemp;
            const char* testtemp=commondetemp.toLocal8Bit().constData();
            system(testtemp);
        }

        if(zipobj->getZipname().split("\.zip")[0]!=nullptr&&zipobj->getZipname().split("\.zip")[0]!=" "){
            QDir tempdir2(apppath+zipobj->getZipname().split("\.zip")[0] );
            if(tempdir2.exists()){
                tempdir2.removeRecursively();
            }
        }
        processvalue+=10;
        emit reportProgress(processvalue);
    }
    //更新包
    QString temppath=apppath+zipobj->getZipname().split("_")[0]+"/"+zipobj->getZipname().split("\.zip")[0];
    QDir dir2(temppath);
    if(zipobj->getVersiontype()=="updated"&&dir2.exists()){
        QString commonde2="cp -rf "+temppath+"/* "+ apppath2;
        qDebug()<<"23."+commonde2;
        const char* test2=commonde2.toLocal8Bit().constData();
        system(test2);
        processvalue+=5;
        emit reportProgress(processvalue);


        QString commonde3="rm -r "+temppath;
        qDebug()<<"23."+ commonde3;
        const char* test3=commonde3.toLocal8Bit().constData();
        system(test3);
        processvalue+=10;
        emit reportProgress(processvalue);
    }
    processvalue+=10;
    emit reportProgress(processvalue);

    QFile qf(basepath+"/zip/"+zipobj->getZipname());

    emit done();
}

QByteArray  MyThread::getFileMd5(QString filePath)
{
    QFile localFile(filePath);

    if (!localFile.open(QFile::ReadOnly))
    {
        qDebug() << "file open error.";
        return 0;
    }

    QCryptographicHash ch(QCryptographicHash::Md5);

    quint64 totalBytes = 0;
    quint64 bytesWritten = 0;
    quint64 bytesToWrite = 0;
    quint64 loadSize = 1024 * 4;
    QByteArray buf;

    totalBytes = localFile.size();
    bytesToWrite = totalBytes;

    while (1)
    {
        if(bytesToWrite > 0)
        {
            buf = localFile.read(qMin(bytesToWrite, loadSize));
            ch.addData(buf);
            bytesWritten += buf.length();
            bytesToWrite -= buf.length();
            buf.resize(0);
        }
        else
        {
            break;
        }

        if(bytesWritten == totalBytes)
        {
            break;
        }
    }

    localFile.close();
    QByteArray md5 = ch.result();
    return md5;
}

bool MyThread::checkzip(QString md5code, QString filepath)
{
    QByteArray qa=getFileMd5(filepath);
    QString md5code2=qa.toHex();
    md5code=md5code.toLower() ;
    qDebug()<<"Server MD5:"+md5code;
    qDebug()<<"Client MD5:"+md5code2;

    return  md5code.compare(md5code2)==0?true:false;
}

bool MyThread::getdownfun(QString fullrequest, QString fileName)
{
    QNetworkRequest request;

    request.setUrl(QUrl(fullrequest));
    //发送Get请求
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()),&eventLoop, SLOT(quit()));
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    QString folderpath=QApplication::applicationDirPath()+"/zip";
    QDir qdir(folderpath);
    if(!qdir.exists()){
        qdir.mkdir(folderpath);
    }
    fileName=folderpath+"/"+fileName;
    QFile qfile(fileName);
    if(!qfile.exists()){
        qfile.open(QIODevice::ReadWrite);
    }
    qfile.write(reply->readAll());
    qfile.close();
    return true;
}
//------------------------------------------ 复制文件夹 END------------


