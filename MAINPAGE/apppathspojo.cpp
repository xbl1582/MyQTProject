#include "apppathspojo.h"

apppathspojo::apppathspojo()
{

}

int apppathspojo::getId() const
{
    return id;
}

void apppathspojo::setId(int value)
{
    id = value;
}

QString apppathspojo::getPath() const
{
    return path;
}

void apppathspojo::setPath(const QString &value)
{
    path = value;
}
