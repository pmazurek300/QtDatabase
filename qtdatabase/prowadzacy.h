#ifndef PROWADZACY_H
#define PROWADZACY_H

#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class Prowadzacy
{
public:
    Prowadzacy();

private:
    QSqlQuery prowadzacy;
};

#endif // PROWADZACY_H
