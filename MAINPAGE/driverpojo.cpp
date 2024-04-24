#include "driverpojo.h"

Driverpojo::Driverpojo()
{

}

QString Driverpojo::getUuid() const
{
    return uuid;
}

void Driverpojo::setUuid(const QString &value)
{
    uuid = value;
}

QString Driverpojo::getDrivername() const
{
    return drivername;
}

void Driverpojo::setDrivername(const QString &value)
{
    drivername = value;
}

QString Driverpojo::getDriversize() const
{
    return driversize;
}

void Driverpojo::setDriversize(const QString &value)
{
    driversize = value;
}

QString Driverpojo::getType() const
{
    return type;
}

void Driverpojo::setType(const QString &value)
{
    type = value;
}

QString Driverpojo::getUpdatetime() const
{
    return updatetime;
}

void Driverpojo::setUpdatetime(QString value)
{
    updatetime = value;
}

int Driverpojo::getDownloadtime() const
{
    return downloadtime;
}

void Driverpojo::setDownloadtime(int value)
{
    downloadtime = value;
}

QString Driverpojo::getPath() const
{
    return path;
}

void Driverpojo::setPath(const QString &value)
{
    path = value;
}

QString Driverpojo::getMd5code() const
{
    return md5code;
}

void Driverpojo::setMd5code(const QString &value)
{
    md5code = value;
}

int Driverpojo::getPriority() const
{
    return priority;
}

void Driverpojo::setPriority(int value)
{
    priority = value;
}
