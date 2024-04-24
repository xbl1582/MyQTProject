#include "ipservicepojo.h"

IPServicePoJo::IPServicePoJo(QObject *parent) : QObject(parent)
{

}

int IPServicePoJo::getDevicetype() const
{
    return devicetype;
}

void IPServicePoJo::setDevicetype(int value)
{
    devicetype = value;
}

QString IPServicePoJo::getIp() const
{
    return ip;
}

void IPServicePoJo::setIp(const QString &value)
{
    ip = value;
}

QString IPServicePoJo::getOuturl() const
{
    return outurl;
}

void IPServicePoJo::setOuturl(const QString &value)
{
    outurl = value;
}

int IPServicePoJo::getPort() const
{
    return port;
}

void IPServicePoJo::setPort(int value)
{
    port = value;
}

QString IPServicePoJo::getSystemtype() const
{
    return systemtype;
}

void IPServicePoJo::setSystemtype(const QString &value)
{
    systemtype = value;
}

QString IPServicePoJo::getUrltype() const
{
    return urltype;
}

void IPServicePoJo::setUrltype(const QString &value)
{
    urltype = value;
}
