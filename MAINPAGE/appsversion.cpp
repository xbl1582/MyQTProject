#include "appsversion.h"

AppsVersion::AppsVersion()
{
    
}
int AppsVersion::getId() const
{
    return id;
}

void AppsVersion::setId(const int &value)
{
    id = value;
}

QString AppsVersion::getUuid() const
{
    return uuid;
}

void AppsVersion::setUuid(const QString &value)
{
    uuid = value;
}

QString AppsVersion::getAppname() const
{
    return appname;
}

void AppsVersion::setAppname(const QString &value)
{
    appname = value;
}

QString AppsVersion::getApppath() const
{
    return apppath;
}

void AppsVersion::setApppath(const QString &value)
{
    apppath = value;
}

int AppsVersion::getApprtmversion() const
{
    return apprtmversion;
}

void AppsVersion::setApprtmversion(int value)
{
    apprtmversion = value;
}

int AppsVersion::getApprtmupdatetime() const
{
    return apprtmupdatetime;
}

void AppsVersion::setApprtmupdatetime(int value)
{
    apprtmupdatetime = value;
}

int AppsVersion::getAppupdatedversion() const
{
    return appupdatedversion;
}

void AppsVersion::setAppupdatedversion(int value)
{
    appupdatedversion = value;
}

int AppsVersion::getAppupdatedtime() const
{
    return appupdatedtime;
}

void AppsVersion::setAppupdatedtime(int value)
{
    appupdatedtime = value;
}

QString AppsVersion::getAppupdateddesc() const
{
    return appupdateddesc;
}

void AppsVersion::setAppupdateddesc(const QString &value)
{
    appupdateddesc = value;
}

QString AppsVersion::getAppsize() const
{
    return appsize;
}

void AppsVersion::setAppsize(const QString &value)
{
    appsize = value;
}

QString AppsVersion::getAppico() const
{
    return appico;
}

void AppsVersion::setAppico(const QString &value)
{
    appico = value;
}

QString AppsVersion::getAppsystem() const
{
    return appsystem;
}

void AppsVersion::setAppsystem(const QString &value)
{
    appsystem = value;
}

int AppsVersion::getAppdevice() const
{
    return appdevice;
}

void AppsVersion::setAppdevice(int value)
{
    appdevice = value;
}

int AppsVersion::getPriority() const
{
    return priority;
}

void AppsVersion::setPriority(int value)
{
    priority = value;
}

QString AppsVersion::getApprtmversionchar() const
{
    return apprtmversionchar;
}

void AppsVersion::setApprtmversionchar(const QString &value)
{
    apprtmversionchar = value;
}

QString AppsVersion::getAppupdatedversionchar() const
{
    return appupdatedversionchar;
}

void AppsVersion::setAppupdatedversionchar(const QString &value)
{
    appupdatedversionchar = value;
}
