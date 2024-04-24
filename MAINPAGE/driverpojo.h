#ifndef DRIVERPOJO_H
#define DRIVERPOJO_H
#include"QString"

class Driverpojo
{
public:
    Driverpojo();
    QString getUuid() const;
    void setUuid(const QString &value);

    QString getDrivername() const;
    void setDrivername(const QString &value);

    QString getDriversize() const;
    void setDriversize(const QString &value);

    QString getType() const;
    void setType(const QString &value);

    QString getUpdatetime() const;
    void setUpdatetime(QString value);

    int getDownloadtime() const;
    void setDownloadtime(int value);

    QString getPath() const;
    void setPath(const QString &value);

    QString getMd5code() const;
    void setMd5code(const QString &value);

    int getPriority() const;
    void setPriority(int value);

private:
    QString  uuid;
    QString  drivername;
    QString driversize;
    QString type;
   QString updatetime;
    int downloadtime;
    QString path;
    QString md5code;
    int priority;
};

#endif // DRIVERPOJO_H
