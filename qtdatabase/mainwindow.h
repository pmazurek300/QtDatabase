#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

#include <prowadzacyokno.h>
#include <studentokno.h>

#include <godzinyzajec.h>
#include <grupazajeciowa.h>
#include <kierunek.h>
#include <pracownik.h>
#include <prowadzacy.h>
#include <student.h>
#include <kurs.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_zaloguj_clicked();
    void logowanie();

signals:


private:
    Ui::MainWindow *ui;
    QSqlDatabase bazaDanych;
    ProwadzacyOkno oknoProwadzacego;
    StudentOkno oknoStudenta;
};
#endif // MAINWINDOW_H
