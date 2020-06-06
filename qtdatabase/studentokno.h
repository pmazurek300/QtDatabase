#ifndef STUDENTOKNO_H
#define STUDENTOKNO_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QDateTime>

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
    explicit StudentOkno(QSqlQuery & query, QString & indentyfikator, QWidget *parent = nullptr);

signals:
    void pokazLogowanie();

private slots:
    void zegar_tick();
    void on_wyloguj_clicked();
    void on_kursy_itemClicked(QListWidgetItem *item);

    void on_grupy_itemClicked(QListWidgetItem *item);

    void on_zapisz_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_grupy_zapisany_itemClicked(QListWidgetItem *item);

    void on_grupy_przeg_itemClicked(QListWidgetItem *item);

    void on_wypisz_clicked();

private:
    Ui::StudentOkno *ui;
    QString identyfikator_studenta;
    QTimer *timerzegara;
    QSqlQuery queryS;
};

#endif // STUDENTOKNO_H
