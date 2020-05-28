#ifndef PROWADZACYOKNO_H
#define PROWADZACYOKNO_H

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
#include <QTimer>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ProwadzacyOkno; }
QT_END_NAMESPACE

class ProwadzacyOkno : public QWidget
{
    Q_OBJECT
public:
    explicit ProwadzacyOkno(QSqlQuery & query, QWidget *parent = nullptr);


signals:
    void pokazLogowanie();

private slots:
    void on_wyloguj_clicked();
    void zegar_tick();

private:
    Ui::ProwadzacyOkno *ui;
    QSqlDatabase baza_danych_prowadzacego;
    QTimer * timerzegara;

};

#endif // PROWADZACYOKNO_H
