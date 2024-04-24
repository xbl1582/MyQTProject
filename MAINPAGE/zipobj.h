#ifndef ZIPOBJ_H
#define ZIPOBJ_H

#include <QObject>

class Zipobj
{
public:
    Zipobj();
    Zipobj( QString uuid,//主键
            QString zipname,//压缩包名词
            int updateloadtime,// 上传时间
            QString zipsize,//压缩包大小
            int type,//终端类型
            QString  os,//系统
            QString versiontype,//版本类型
            int version,//版本
            QString versionchar,
            QString path,
            QString md5code
            );
    QString getUuid() const;
    void setUuid(const QString &value);

    QString getZipname() const;
    void setZipname(const QString &value);

    int getUpdateloadtime() const;
    void setUpdateloadtime(const int &value);

    QString getZipsize() const;
    void setZipsize(const QString &value);

    int getType() const;
    void setType(int value);

    QString getOs() const;
    void setOs(const QString &value);

    QString getVersiontype() const;
    void setVersiontype(const QString &value);

    int getVersion() const;
    void setVersion(const int &value);

    QString getPath() const;
    void setPath(const QString &value);

    QString getMd5code() const;
    void setMd5code(const QString &value);

    QString getVersionchar() const;
    void setVersionchar(const QString &value);

private:
    QString uuid;//主键
    QString zipname;//压缩包名词
    int updateloadtime;// 上传时间
    QString zipsize;//压缩包大小
    int type;//终端类型
    QString  os;//系统
    QString versiontype;//版本类型
     int version;//版本
    QString path;//存储路径
    QString md5code;//md5码
      QString versionchar;//版本号
};

#endif // ZIPOBJ_H
