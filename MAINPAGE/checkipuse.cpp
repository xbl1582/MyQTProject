#include "checkipuse.h"

CheckIpUse::CheckIpUse(): QThread()
{

}

CheckIpUse::~CheckIpUse()
{

}
void CheckIpUse::run(){
    appcurrentpath=QCoreApplication::applicationDirPath();
    basepath=appcurrentpath.split("/KSTMAIN")[0];
    //第一次执行先将文件删除
    QFile file(appcurrentpath+"/shellcode/unreachable.txt");
    if(file.exists()){
        file.remove();
    }
    QFile file2(appcurrentpath+"/shellcode/reachable.txt");
    if(file2.exists()){
        file2.remove();
    }

    QString commond="sh "+appcurrentpath+"/shellcode/checkipall.sh";
    const char* code=commond.toLocal8Bit().data();
    int result=system(code);
    if(!System_Check(result)){
        QFile file(appcurrentpath+"/shellcode/unreachable.txt");
        if(file.exists()){

            //创建文件对象
            bool isok = file.open(QIODevice::ReadOnly); //只读模式打开
            if(isok == true)
            {
                //读文件
                QByteArray array =  file.readAll();
                emit reportData(QString(array).toUtf8().data());
                //一行一行的读
                //                    QByteArray array;
                //                    while(file.atEnd() == false)
                //                    {
                //                        array += file.readLine();
                //                    }
                //                  emit reportData(QString(array).toUtf8().data());
            }
        }

    }
    //        else{
    //            QMessageBox *m_box = new QMessageBox(QMessageBox::Information,QString("通知"),"出错了请重试");

    //            QFont qf;
    //            qf.setPixelSize(20);
    //            m_box->setFont(qf);
    //            m_box->setStyleSheet(""
    //                                 "QPushButton {"
    //                                 "background-color:#89AFDE;"
    //                                 " border-style: outset;"
    //                                 " border-width: 2px;"
    //                                 " border-radius: 10px;"
    //                                 " border-color: beige;"
    //                                 " font: bold 24px;"
    //                                 " min-width: 5em;"
    //                                 " min-height: 2em;"
    //                                 "}");
    //            m_box->show();

    //        }
    emit done();


}
int CheckIpUse::System_Check(int result)
{
    if((-1 != result) && (WIFEXITED(result)) && (!(WEXITSTATUS(result))))

        return 0;
    else

        return -1;
}
