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

    queryP.prepare("SELECT * FROM kurs");
    queryP.exec();
    while(queryP.next()){
        QString result = queryP.value("id_kursu").toString() + " " +
                queryP.value("nazwa_kursu").toString() + " " +
                queryP.value("forma_zajec").toString()[0];
        new QListWidgetItem(result, ui->listakursow);
        new QListWidgetItem(result, ui->listaKURS);

    }
    ui->dziengrupa->addItem("PN");
    ui->dziengrupa->addItem("WT");
    ui->dziengrupa->addItem("ŚR");
    ui->dziengrupa->addItem("CZW");
    ui->dziengrupa->addItem("PT");

    ui->parzystoscgrupa->addItem("AL");
    ui->parzystoscgrupa->addItem("TP");
    ui->parzystoscgrupa->addItem("TN");

    ui->formaKURS->addItem("Wykład");
    ui->formaKURS->addItem("Ćwiczenia");
    ui->formaKURS->addItem("Laboratorium");
    for (int i=1;i<8;i++) {
        ui->semestrKURS->addItem(QString::number(i));
    }

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

void ProwadzacyOkno::on_listakursow_itemClicked(QListWidgetItem *item)
{
    ui->listagrup->clear();
    QStringList l = item->text().split(' ', QString::SkipEmptyParts);
    qDebug() << l.first();
    queryP.prepare("SELECT * FROM grupa_zajeciowa WHERE id_kursu="+l.first());
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    while(queryP.next()){
        new QListWidgetItem(queryP.value("id_grupy").toString(), ui->listagrup);
    }
}

void ProwadzacyOkno::on_listagrup_itemClicked(QListWidgetItem *item)
{
    ui->prowadzacygrupa->clear();
    ui->godzinagrupa->clear();
    QString idgodz, idprowadz;
    queryP.prepare("SELECT * FROM grupa_zajeciowa WHERE id_grupy="+item->text());
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    queryP.next();
    ui->kodgrupy->setText(item->text());
    ui->dziengrupa->setCurrentIndex(ui->dziengrupa->findData(queryP.value("dzien_tygodnia").toString(),Qt::DisplayRole));
    ui->iloscgrupa->setText(queryP.value("ilosc_miejsc").toString());
    ui->salagrupa->setText(queryP.value("sala_zajeciowa").toString());
    ui->parzystoscgrupa->setCurrentIndex(ui->parzystoscgrupa->findData(queryP.value("parzystosc_tygodnia").toString(),Qt::DisplayRole));
    idgodz = queryP.value("id_godziny_zajec").toString();
    idprowadz = queryP.value("id_prowadzacego").toString();
    queryP.prepare("SELECT * FROM godziny_zajec");
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    while(queryP.next()){
        ui->godzinagrupa->addItem(queryP.value("godzina_rozpoczecia").toString());
    }
    queryP.prepare("SELECT * FROM godziny_zajec WHERE id_godziny_zajec="+idgodz);
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    queryP.next();
    ui->godzinagrupa->setCurrentIndex(ui->godzinagrupa->findData(queryP.value("godzina_rozpoczecia").toString(),Qt::DisplayRole));


    queryP.prepare("SELECT * FROM prowadzący");
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    while(queryP.next()){
        QString result = queryP.value("tytul").toString() + " " +
                queryP.value("imie").toString() + " " +
                queryP.value("nazwisko").toString();
        ui->prowadzacygrupa->addItem(result);
    }
    queryP.prepare("SELECT * FROM prowadzący WHERE id_prowadzacego="+idprowadz);
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    queryP.next();
    QString tmp = queryP.value("tytul").toString() + " " +
            queryP.value("imie").toString() + " " +
            queryP.value("nazwisko").toString();
    ui->prowadzacygrupa->setCurrentIndex(ui->prowadzacygrupa->findData(tmp,Qt::DisplayRole));

}

void ProwadzacyOkno::on_createnewgroup_clicked()
{
    int licznik = 1;
    queryP.prepare("SELECT * from grupa_zajeciowa");
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    while(queryP.next()){
        licznik++;
    }
    QString dzien = ui->dziengrupa->currentText();
    QString sala  = ui->salagrupa->toMarkdown().trimmed();
    QString parzy = ui->parzystoscgrupa->currentText();
    QString ilosc = ui->iloscgrupa->toMarkdown().trimmed();
    QString prowa = QString::number(ui->prowadzacygrupa->currentIndex()+1);
    QStringList l = ui->listakursow->currentItem()->text().split(' ', QString::SkipEmptyParts);
    QString kurs  = l.first();

    QString godz  = QString::number(ui->godzinagrupa->currentIndex()+1);
    qDebug() << kurs;
    queryP.prepare("INSERT INTO grupa_zajeciowa (id_grupy,id_kursu,id_prowadzacego,id_godziny_zajec,dzien_tygodnia,parzystosc_tygodnia,ilosc_miejsc,sala_zajeciowa) "
                   "VALUES ('"+QString::number(licznik)+"','"+kurs+"','"+prowa+"','"+godz+"','"+dzien+"','"+parzy+"','"+ilosc+"','"+sala+"')");
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    queryP.next();
    new QListWidgetItem(QString::number(licznik), ui->listagrup);
}

void ProwadzacyOkno::on_pushButton_4_clicked()
{
    if(ui->listagrup->currentItem()!=nullptr){
        queryP.prepare("DELETE FROM grupa_zajeciowa WHERE id_grupy="+ui->listagrup->currentItem()->text());
        if(!queryP.exec()){
            QMessageBox::information(this,"Błąd","Błędne Query");
        }
        queryP.next();
        delete ui->listagrup->currentItem();
    }
}

void ProwadzacyOkno::on_pushButton_5_clicked()
{
    if(ui->listagrup->currentItem()!=nullptr){
        QString dzien = ui->dziengrupa->currentText();
        QString sala  = ui->salagrupa->toMarkdown().trimmed();
        QString parzy = ui->parzystoscgrupa->currentText();
        QString ilosc = ui->iloscgrupa->toMarkdown().trimmed();
        QString prowa = QString::number(ui->prowadzacygrupa->currentIndex()+1);
        QStringList l = ui->listakursow->currentItem()->text().split(' ', QString::SkipEmptyParts);
        QString kurs  = l.first();
        QString grupa = ui->listagrup->currentItem()->text();

        QString godz  = QString::number(ui->godzinagrupa->currentIndex()+1);
        queryP.prepare("UPDATE grupa_zajeciowa SET id_prowadzacego='"+prowa+"',id_godziny_zajec='"+godz+"',dzien_tygodnia='"+dzien+"',parzystosc_tygodnia='"+parzy+
                       "',ilosc_miejsc='"+ilosc+"',sala_zajeciowa='"+sala+"' WHERE id_grupy="+grupa);
        if(!queryP.exec()){
            QMessageBox::information(this,"Błąd","Błędne Query");
        }
        queryP.next();
    }
}

void ProwadzacyOkno::on_listaKURS_itemClicked(QListWidgetItem *item)
{
    QStringList l = item->text().split(' ', QString::SkipEmptyParts);
    ui->idkursKURS->setText(l.first());
    queryP.prepare("SELECT * FROM kurs WHERE id_kursu="+l.first());
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    queryP.next();
    ui->nazwaKURS->setText(queryP.value("nazwa_kursu").toString());
    ui->ectsKURS->setText(queryP.value("ects").toString());
    ui->semestrKURS->setCurrentIndex(ui->semestrKURS->findData(queryP.value("semestr").toString(),Qt::DisplayRole));
    ui->formaKURS->setCurrentIndex(ui->formaKURS->findData(queryP.value("forma_zajec").toString(),Qt::DisplayRole));
}

void ProwadzacyOkno::on_createKURS_clicked()
{
    queryP.exec("SELECT * FROM kurs WHERE id_kursu=(SELECT MAX(id_kursu) FROM kurs)");
    queryP.next();
    int id = queryP.value("id_kursu").toInt()+1;
    QString nazwa = ui->nazwaKURS->toMarkdown().trimmed();
    QString semestr = ui->semestrKURS->currentText();
    QString forma = ui->formaKURS->currentText();
    QString ects = ui->ectsKURS->toMarkdown().trimmed();
    queryP.prepare("INSERT INTO kurs (id_kursu,nazwa_kursu,semestr,forma_zajec,ects)"
                   "VALUES ('"+QString::number(id)+"','"+nazwa+"','"+semestr+"','"+forma+"','"+ects+"')");
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    queryP.next();
    ui->listaKURS->clear();
    queryP.prepare("SELECT * FROM kurs");
    queryP.exec();
    while(queryP.next()){
        QString result = queryP.value("id_kursu").toString() + " " +
                queryP.value("nazwa_kursu").toString() + " " +
                queryP.value("forma_zajec").toString()[0];
        new QListWidgetItem(result, ui->listaKURS);

    }

}

void ProwadzacyOkno::on_pushButton_6_clicked()
{
    if(ui->listaKURS->currentItem()!=nullptr){
        QStringList l = ui->listaKURS->currentItem()->text().split(' ', QString::SkipEmptyParts);
        queryP.prepare("DELETE FROM kurs WHERE id_kursu="+l.first());
        if(!queryP.exec()){
            QMessageBox::information(this,"Błąd","Błędne Query");
        }
        queryP.next();
        delete ui->listaKURS->currentItem();
    }
}

void ProwadzacyOkno::on_pushButton_7_clicked()
{
    if(ui->listaKURS->currentItem()!=nullptr){
        QStringList l = ui->listaKURS->currentItem()->text().split(' ', QString::SkipEmptyParts);
        QString nazwa = ui->nazwaKURS->toMarkdown().trimmed();
        QString semestr = ui->semestrKURS->currentText();
        QString forma = ui->formaKURS->currentText();
        QString ects = ui->ectsKURS->toMarkdown().trimmed();
        queryP.prepare("UPDATE kurs SET nazwa_kursu='"+nazwa+"',semestr='"+semestr+"',forma_zajec='"+forma+"',ects='"+ects+"' WHERE id_kursu="+l.first());
        if(!queryP.exec()){
            QMessageBox::information(this,"Błąd","Błędne Query");
        }
        queryP.next();
    }
}
