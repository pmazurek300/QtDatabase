#include "prowadzacyokno.h"
#include "ui_prowadzacyokno.h"

ProwadzacyOkno::ProwadzacyOkno(QSqlQuery & query, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProwadzacyOkno)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Okno prowadzącego"));
    query.prepare("SELECT * FROM student");
    if(!query.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    while(query.next()){
        QString result = query.value("id_indeks").toString() + " " +
                         query.value("imie").toString() + " " +
                         query.value("nazwisko").toString();
    new QListWidgetItem(result, ui->listastudentow);
    }
    timerzegara = new QTimer();
    connect(timerzegara,SIGNAL(timeout()),this,SLOT(zegar_tick()));
    timerzegara->start(1000);
}

void ProwadzacyOkno::zegar_tick(){
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    ui->zegar->display(text);
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

