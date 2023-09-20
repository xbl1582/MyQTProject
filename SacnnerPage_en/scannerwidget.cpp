#include "scannerwidget.h"
#include "ui_scannerwidget.h"

scannerwidget::scannerwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scannerwidget)
{
    QRect screenRect=QGuiApplication::primaryScreen()->geometry();
    this->screenwidth=screenRect.width();
    this->screenheight=screenRect.height();
    this->resize(screenwidth,screenheight);
    Qt::WindowFlags m_flags = windowFlags();
    this->setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
    ui->setupUi(this);
    initscannerwidget();
}

scannerwidget::~scannerwidget()
{
    delete ui;
}

void scannerwidget::initscannerwidget()
{
    Qt::WindowFlags m_flags=this->windowFlags();
     this->setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
    this->resize(this->screenwidth,this->screenheight);
    this->setWindowTitle(tr("ScanPage"));
    QMovie *movie = new QMovie(":/image/loading.gif");
    ui->label->setStyleSheet("color:rgba(255,255,255,1)");
    ui->label->setMovie(movie);
    ui->label->move(this->screenwidth/2,200);
    movie->start();


    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);


}
QString scannerwidget::readpath(QString key){
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
     return foldepath.toString();
}

void scannerwidget::paintEvent(QPaintEvent *)
{

        //设置窗体透明层颜色
        QPainter painter(this);
        painter.fillRect(rect(), QColor(0, 0, 0, 1));

}
int scannerwidget::readfloadimage()
{
     QString folderstring=readpath("imageinitpath");
     QDir dir(folderstring);
    if(dir.exists()){
        //设置过滤器
           QStringList filters;
           filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.png")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp");
           dir.setFilter (QDir::Files | QDir::NoSymLinks); // 设置类型过滤器，只为文件格式
           dir.setNameFilters (filters);  // 设置文件名称过滤器，只为 filters 格式（后缀为.jpeg 等图片格式）
           int dir_count = dir.count();

           return dir_count;
    }else{
         return 0;
    }

}

void scannerwidget::movefiletoresult()
{
    //读取json文件
    QString filepath=QCoreApplication::applicationDirPath()+"/json/pathjson.json";
    QFile loadFile(filepath);
          if(!loadFile.open(QIODevice::ReadOnly| QIODevice::Text))
          {

                 return ;
          }
    QByteArray array = loadFile.readAll(); //将数据读到array中
    loadFile.close();
    //json错误抛出
    QJsonParseError json_error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(array,&json_error);
    if(json_error.error != QJsonParseError::NoError)//判断是否可以解析
     {
           qDebug()<<"json error!";
           return ;
      }
     QJsonObject rootObj = jsonDoc.object();
     QJsonValue foldepath = rootObj.value("imageinitpath");
      QJsonValue resultimagepath = rootObj.value("resultimagepath");
     QString folderstring=foldepath.toString();
      QString resultstring=resultimagepath.toString();

     QDir dir(folderstring);
     QDir dir2(resultstring);
     if(!dir2.exists()){
         dir2.mkdir(resultstring);
     }

    if(dir.exists()){
        //设置过滤器
           QStringList filters;
           filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.png")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp");
           dir.setFilter (QDir::Files | QDir::NoSymLinks); // 设置类型过滤器，只为文件格式
           dir.setNameFilters (filters);  // 设置文件名称过滤器，只为 filters 格式（后缀为.jpeg 等图片格式）
           int dir_count = dir.count();
           if(dir_count<=0){
               return ;
           }else{
               QChar separator = QChar('/');
               if(!folderstring.contains(separator))
               {
                   separator = QChar('\\');
               }
               QChar last_char =folderstring.at(folderstring.length()-1);
               if(last_char == separator)
               {
                   separator = QChar();
               }

               QString newfilepath=resultstring+separator+QString::number(QDateTime::currentMSecsSinceEpoch())+separator;
               QChar h0 = 0x00;
               if ( newfilepath.contains(h0))
               {
                    newfilepath=newfilepath.replace(h0, "");
               }
               QDir temp(newfilepath);
               if(!temp.exists()){
                  temp.mkdir(newfilepath);
               }
               for(int i=0; i<dir_count; i++)
               {
                   QString file_name = dir [i];  // 文件名称
                   QString file_path = folderstring + separator + file_name;   // 文件全路径

                   if ( file_path.contains(h0))
                   {
                        file_path=file_path.replace(h0, "");
                   }
                   QString qnewfilepath=newfilepath+"/"+file_name;
                           if ( qnewfilepath.contains(h0))
                           {
                                qnewfilepath=qnewfilepath.replace(h0, "");
                           }

                   QFile::copy(file_path,qnewfilepath);
                   QFile::remove(file_path);
               }
           }
    }
}


