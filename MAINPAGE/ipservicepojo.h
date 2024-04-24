#ifndef IPSERVICEPOJO_H
#define IPSERVICEPOJO_H

#include <QObject>

class IPServicePoJo : public QObject
{
    Q_OBJECT
public:
    explicit IPServicePoJo(QObject *parent = nullptr);
    int getDevicetype() const;
    void setDevicetype(int value);

    QString getIp() const;
    void setIp(const QString &value);

    QString getOuturl() const;
    void setOuturl(const QString &value);

    int getPort() const;
    void setPort(int value);

    QString getSystemtype() const;
    void setSystemtype(const QString &value);

    QString getUrltype() const;
    void setUrltype(const QString &value);



private:
    int devicetype;
    QString ip;
    QString outurl;
    int port=0;
    QString systemtype;
    QString urltype;


signals:

};

#endif // IPSERVICEPOJO_H
