#ifndef STUDENT_H
#define STUDENT_H

#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class Student
{
public:
    Student();

private:
    QString imie;
    QString nazwisko;
    QString id_indeks;
    QString pesel;
    QString login;
    QString haslo;
    QString email;
    QString plec;
    QString id_kierunku;
    QString semestr;
    QString prawo_do_zapisu;
    QString termin_zapisu;
};

#endif // STUDENT_H
