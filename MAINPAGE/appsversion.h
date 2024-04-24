#ifndef APPSVERSION_H
#define APPSVERSION_H
#include "QString"

class AppsVersion
{
public:
    AppsVersion();
    int getId() const;
    void setId(const int &value);

    QString getUuid() const;
    void setUuid(const QString &value);

    QString getAppname() const;
    void setAppname(const QString &value);

    QString getApppath() const;
    void setApppath(const QString &value);

    int getApprtmversion() const;
    void setApprtmversion(int value);

    int getApprtmupdatetime() const;
    void setApprtmupdatetime(int value);

    int getAppupdatedversion() const;
    void setAppupdatedversion(int value);

    int getAppupdatedtime() const;
    void setAppupdatedtime(int value);

    QString getAppupdateddesc() const;
    void setAppupdateddesc(const QString &value);

    QString getAppsize() const;
    void setAppsize(const QString &value);

    QString getAppico() const;
    void setAppico(const QString &value);

    QString getAppsystem() const;
    void setAppsystem(const QString &value);

    int getAppdevice() const;
    void setAppdevice(int value);

    int getPriority() const;
    void setPriority(int value);

    QString getApprtmversionchar() const;
    void setApprtmversionchar(const QString &value);

    QString getAppupdatedversionchar() const;
    void setAppupdatedversionchar(const QString &value);

private:
    int id;
    QString uuid;
    QString appname;
    QString apppath;
    int apprtmversion;
   QString apprtmversionchar;
   int apprtmupdatetime;
   int appupdatedversion;
   QString appupdatedversionchar;
   int appupdatedtime;
   QString appupdateddesc;
   QString appsize;
   QString appico;
   QString appsystem;
   int appdevice;
   int priority;
};

#endif // APPSVERSION_H
