#ifndef GRUPAZAJECIOWA_H
#define GRUPAZAJECIOWA_H

#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class GrupaZajeciowa
{
public:
    GrupaZajeciowa();

private:
    QSqlQuery grupa_zajeciowa;
};

#endif // GRUPAZAJECIOWA_H
