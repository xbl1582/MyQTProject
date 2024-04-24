#ifndef EMPOWERPOJO_H
#define EMPOWERPOJO_H
#include <QString>

class EmpowerPoJo
{
public:
    EmpowerPoJo();
    int getId() const;
    void setId(int value);

    QString getUuid() const;
    void setUuid(const QString &value);

    QString getSn() const;
    void setSn(const QString &value);

    QString getSncode() const;
    void setSncode(const QString &value);

    QString getLic() const;
    void setLic(const QString &value);

    QString getSnpath() const;
    void setSnpath(const QString &value);

    QString getLicpath() const;
    void setLicpath(const QString &value);

    QString getUploadtime() const;
    void setUploadtime(const QString &value);

    int getSndownloadtime() const;
    void setSndownloadtime(int value);

    int getLicdownloadtime() const;
    void setLicdownloadtime(int value);

    QString getSnsize() const;
    void setSnsize(const QString &value);

    QString getLicsize() const;
    void setLicsize(const QString &value);

    QString getLicmd5code() const;
    void setLicmd5code(const QString &value);

    QString getType() const;
    void setType(const QString &value);

private:
    int id;
    QString uuid;
    QString sn;
    QString sncode;
       QString   lic;
       QString   snpath;
       QString   licpath;
       QString   uploadtime;
       int  sndownloadtime;
       int  licdownloadtime;
       QString   snsize;
       QString   licsize;
       QString licmd5code;
       QString type;
};

#endif // EMPOWERPOJO_H
