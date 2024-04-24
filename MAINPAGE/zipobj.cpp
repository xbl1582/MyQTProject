#include "zipobj.h"

Zipobj::Zipobj()
{

}

Zipobj::Zipobj(QString uuid, QString zipname, int updateloadtime, QString zipsize, int type, QString os, QString versiontype, int version,   QString versionchar,QString path, QString md5code)
{
    this->uuid=uuid;//主键
    this->zipname=zipname;//压缩包名词
    this->updateloadtime=updateloadtime;// 上传时间
    this->zipsize=zipsize;//压缩包大小
    this->type=type;//终端类型
    this->os=os;//系统
    this->versiontype=versiontype;//版本类型
    this-> version=version;//版本
    this->versionchar=versionchar;
    this->path=path;
}

QString Zipobj::getUuid() const
{
    return uuid;
}

void Zipobj::setUuid(const QString &value)
{
    uuid = value;
}

QString Zipobj::getZipname() const
{
    return zipname;
}

void Zipobj::setZipname(const QString &value)
{
    zipname = value;
}

 int Zipobj::getUpdateloadtime() const
{
    return updateloadtime;
}

void Zipobj::setUpdateloadtime(const int &value)
{
    updateloadtime = value;
}

QString Zipobj::getZipsize() const
{
    return zipsize;
}

void Zipobj::setZipsize(const QString &value)
{
    zipsize = value;
}

int Zipobj::getType() const
{
    return type;
}

void Zipobj::setType(int value)
{
    type = value;
}

QString Zipobj::getOs() const
{
    return os;
}

void Zipobj::setOs(const QString &value)
{
    os = value;
}

QString Zipobj::getVersiontype() const
{
    return versiontype;
}

void Zipobj::setVersiontype(const QString &value)
{
    versiontype = value;
}

 int Zipobj::getVersion() const
{
    return version;
}

void Zipobj::setVersion(const int &value)
{
    version = value;
}

QString Zipobj::getPath() const
{
    return path;
}

void Zipobj::setPath(const QString &value)
{
    path = value;
}

QString Zipobj::getMd5code() const
{
    return md5code;
}

void Zipobj::setMd5code(const QString &value)
{
    md5code = value;
}

QString Zipobj::getVersionchar() const
{
    return versionchar;
}

void Zipobj::setVersionchar(const QString &value)
{
    versionchar = value;
}
