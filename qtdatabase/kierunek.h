#ifndef KIERUNEK_H
#define KIERUNEK_H

#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class Kierunek
{
public:
    Kierunek();

private:
    QSqlQuery kierunek;
};

#endif // KIERUNEK_H
