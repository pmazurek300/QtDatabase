#include "prowadzacyokno.h"
#include "ui_prowadzacyokno.h"

ProwadzacyOkno::ProwadzacyOkno(QSqlQuery & query, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProwadzacyOkno)
{
    ui->setupUi(this);
    queryP = query;
    queryP.prepare("SELECT * FROM student");
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    while(queryP.next()){
        QString result = queryP.value("id_indeks").toString() + " " +
                         queryP.value("imie").toString() + " " +
                         queryP.value("nazwisko").toString();
    new QListWidgetItem(result, ui->listastudentow);
    }
    timerzegara = new QTimer();
    connect(timerzegara,SIGNAL(timeout()),this,SLOT(zegar_tick()));
    timerzegara->start(1000);
    ui->textedit->setText("YYYY-MM-DD hh-mm-ss");
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
}


void ProwadzacyOkno::on_listastudentow_itemClicked(QListWidgetItem *item)
{
    ui->textedit->setText("YYYY-MM-DD hh-mm-ss");
    QStringList l = item->text().split(' ', QString::SkipEmptyParts);
    qDebug() << l.first();
    queryP.prepare("SELECT * FROM student WHERE id_indeks="+l.first());
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    queryP.next();
    ui->imieL->setText(queryP.value("imie").toString());
    ui->nazwiskoL->setText(queryP.value("nazwisko").toString());
    ui->indeksL->setText(queryP.value("id_indeks").toString());
    ui->peselL->setText(queryP.value("pesel").toString());
    ui->kierunekL->setText(queryP.value("id_kierunku").toString());
    ui->prawoL->setText(queryP.value("prawo_do_zapisow").toString());
    ui->semestrL->setText(queryP.value("semestr").toString());
    ui->terminL->setText((queryP.value("termin_zapisow").toString()).replace(10,1," "));
    //qDebug() << (queryP.value("termin_zapisow").toString());
}



void ProwadzacyOkno::on_pushButton_clicked()//nadaj
{
    queryP.prepare("UPDATE student SET prawo_do_zapisow = 'posiada' WHERE id_indeks ="+ui->indeksL->text());
    queryP.exec();
    queryP.next();
    queryP.prepare("SELECT * FROM student WHERE id_indeks="+ui->indeksL->text());
    queryP.exec();
    queryP.next();
    ui->prawoL->setText(queryP.value("prawo_do_zapisow").toString());

}



void ProwadzacyOkno::on_pushButton_2_clicked()//odbierz
{
    queryP.prepare("UPDATE student SET prawo_do_zapisow = 'brak' WHERE id_indeks ="+ui->indeksL->text());
    queryP.exec();
    queryP.next();
    queryP.prepare("SELECT * FROM student WHERE id_indeks="+ui->indeksL->text());
    queryP.exec();
    queryP.next();
    ui->prawoL->setText(queryP.value("prawo_do_zapisow").toString());
    queryP.prepare("UPDATE student SET termin_zapisow = ' ' WHERE id_indeks ="+ui->indeksL->text());
    queryP.exec();
    queryP.next();
    ui->terminL->setText(" ");

}

/*void ProwadzacyOkno::on_terminZapis_textChanged()
{
    qDebug() << ui->textedit;
}*/

void ProwadzacyOkno::on_pushButton_3_clicked()
{
    queryP.prepare("UPDATE student SET termin_zapisow = '"+ui->textedit->toMarkdown().trimmed()+"' WHERE id_indeks ="+ui->indeksL->text());
    queryP.exec();
    queryP.next();
    queryP.prepare("SELECT * FROM student WHERE id_indeks="+ui->indeksL->text());
    queryP.exec();
    queryP.next();
    ui->terminL->setText(queryP.value("termin_zapisow").toString().replace(10,1," "));
}



void ProwadzacyOkno::on_textedit_selectionChanged()
{
    ui->textedit->setText("");
}
