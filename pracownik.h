#ifndef PRACOWNIK_H
#define PRACOWNIK_H

#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class Pracownik
{
public:
    Pracownik();

private:
    QSqlQuery pracownik;
};

#endif // PRACOWNIK_H
