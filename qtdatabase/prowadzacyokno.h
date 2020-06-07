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
    explicit ProwadzacyOkno(QSqlQuery & query,QString & indentyfikator, QWidget *parent = nullptr);


signals:
    void pokazLogowanie();

private slots:
    void on_wyloguj_clicked();
    void zegar_tick();

    void on_listastudentow_itemClicked(QListWidgetItem *item);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    //void on_terminZapis_textChanged();

    void on_pushButton_3_clicked();


    void on_textedit_selectionChanged();

    void on_listakursow_itemClicked(QListWidgetItem *item);

    void on_listagrup_itemClicked(QListWidgetItem *item);

    void on_createnewgroup_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_listaKURS_itemClicked(QListWidgetItem *item);

    void on_createKURS_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_kursy_zapisz_itemClicked(QListWidgetItem *item);

    void on_grupy_zapisz_itemClicked(QListWidgetItem *item);

    void on_zapisz_zapisz_clicked();

    void on_wypisz_zapisz_clicked();

private:
    Ui::ProwadzacyOkno *ui;
    QSqlDatabase baza_danych_prowadzacego;
    QTimer * timerzegara;
    QSqlQuery queryP;

};

#endif // PROWADZACYOKNO_H
