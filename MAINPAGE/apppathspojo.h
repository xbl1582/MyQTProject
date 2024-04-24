#ifndef APPPATHSPOJO_H
#define APPPATHSPOJO_H
#include"QString"

class apppathspojo
{
public:
    apppathspojo();
    int getId() const;
    void setId(int value);

    QString getPath() const;
    void setPath(const QString &value);

private:
    int id;
    QString path;

};

#endif // APPPATHSPOJO_H
