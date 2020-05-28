#ifndef STUDENTOKNO_H
#define STUDENTOKNO_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>

#include <godzinyzajec.h>
#include <grupazajeciowa.h>
#include <kierunek.h>
#include <pracownik.h>
#include <prowadzacy.h>
#include <student.h>
#include <kurs.h>

QT_BEGIN_NAMESPACE
namespace Ui { class StudentOkno; }
QT_END_NAMESPACE

class StudentOkno : public QWidget
{
    Q_OBJECT
public:
    explicit StudentOkno(QWidget *parent = nullptr);

signals:

private:
    Ui::StudentOkno *ui;

};

#endif // STUDENTOKNO_H
