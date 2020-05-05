#include "prowadzacyokno.h"
#include "ui_prowadzacyokno.h"

ProwadzacyOkno::ProwadzacyOkno(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProwadzacyOkno)
{
    ui->setupUi(this);
}


void ProwadzacyOkno::on_wyloguj_clicked()
{
    this->close();
    emit pokazLogowanie();
}

//  qry = new QSqlQueryModel();
//  qry->setQuery("SELECT * FROM godziny_zajec");   // to jest do zapytan
//   ui->tableView->setModel(qry);
