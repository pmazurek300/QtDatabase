#include "prowadzacyokno.h"
#include "ui_prowadzacyokno.h"

ProwadzacyOkno::ProwadzacyOkno(QSqlQuery & query, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProwadzacyOkno)
{
    ui->setupUi(this);
}


void ProwadzacyOkno::on_wyloguj_clicked()
{
    emit pokazLogowanie();
    delete this;
//    qDebug() << baza_danych_prowadzacego.hostName();
//    baza_danych_prowadzacego.open();
//    qry = new QSqlQueryModel();
//    qry->query().prepare("SELECT * FROM godziny_zajec");   // to jest do zapytan
//    qDebug() <<qry->query().prepare("SELECT * FROM godziny_zajec");
}

