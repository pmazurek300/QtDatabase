#ifndef GODZINYZAJEC_H
#define GODZINYZAJEC_H

#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class GodzinyZajec
{
public:
    GodzinyZajec();

private:
    QSqlQuery godziny_zajec;
};

#endif // GODZINYZAJEC_H
