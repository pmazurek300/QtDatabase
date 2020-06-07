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
    QSqlQuery student;
};

#endif // STUDENT_H
