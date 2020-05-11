#ifndef KURS_H
#define KURS_H

#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class Kurs
{
public:
    Kurs();

private:
    QSqlQuery kurs;
};

#endif // KURS_H
