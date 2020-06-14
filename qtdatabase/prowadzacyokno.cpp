#include "prowadzacyokno.h"
#include "ui_prowadzacyokno.h"

ProwadzacyOkno::ProwadzacyOkno(QSqlQuery & query,QString & identyfikator, QWidget *parent)
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
        new QListWidgetItem(result, ui->listastudentMODY);
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

    query.exec("SELECT * FROM pracownik WHERE login='"+identyfikator+"'");
    query.next();
    ui->imieLabel->setText("Imię: "+query.value("imie").toString());
    ui->nazwiskoLabel->setText("Nazwisko: "+query.value("nazwisko").toString());

    queryP.prepare("SELECT * FROM kurs");
    queryP.exec();
    while(queryP.next()){
        QString result = queryP.value("id_kursu").toString() + " " +
                queryP.value("nazwa_kursu").toString() + " " +
                queryP.value("forma_zajec").toString()[0];
        new QListWidgetItem(result, ui->kursy_zapisz);
    }
    queryP.prepare("SELECT * FROM student");
    queryP.exec();
    while(queryP.next()){
        QString result = queryP.value("id_indeks").toString() + " " +
                queryP.value("imie").toString() + " " +
                queryP.value("nazwisko").toString();
        new QListWidgetItem(result, ui->studenci_zapisz);
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
    queryP.prepare("SELECT * FROM grupa_zajeciowa WHERE id_kursu="+l.first());
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    while(queryP.next()){
        if(queryP.value("dzien_tygodnia").toString()!=" ")
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
    queryP.prepare("INSERT INTO grupa_zajeciowa (id_grupy,id_kursu,id_prowadzacego,id_godziny_zajec,dzien_tygodnia,parzystosc_tygodnia,ilosc_miejsc,sala_zajeciowa) "
                   "VALUES ('"+QString::number(licznik)+"','"+kurs+"','"+prowa+"','"+godz+"','"+dzien+"','"+parzy+"','"+ilosc+"','"+sala+"')");
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    queryP.next();
    new QListWidgetItem(QString::number(licznik), ui->listagrup);
}

void ProwadzacyOkno::on_pushButton_4_clicked()//usun grupe
{
    //    queryP.prepare("DELETE FROM grupa_zajeciowa WHERE id_grupy="+ui->listagrup->currentItem()->text());
    //    if(!queryP.exec()){
    //        QMessageBox::information(this,"Błąd","Błędne Query");
    //    }
    //    queryP.next();
    //    delete ui->listagrup->currentItem();

    if(ui->listagrup->currentItem() != nullptr){
    queryP.exec("UPDATE grupa_zajeciowa SET dzien_tygodnia=' ' WHERE id_grupy="+ui->listagrup->currentItem()->text());
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    queryP.next();
    delete ui->listagrup->currentItem();
}
}

void ProwadzacyOkno::on_tabWidget_currentChanged(int index)
{
    if(index == 3){
        ui->studenci_zapisz->clear();
        ui->kursy_zapisz->clear();
        ui->grupy_zapisz->clear();
        queryP.exec("SELECT * FROM student");
        while(queryP.next()){
            new QListWidgetItem(queryP.value("id_indeks").toString()+" "+queryP.value("imie").toString()+" "+queryP.value("nazwisko").toString(),ui->studenci_zapisz);
        }
        queryP.exec("SELECT * FROM kurs");
        while(queryP.next()){
            new QListWidgetItem(queryP.value("id_kursu").toString()+" "+queryP.value("nazwa_kursu").toString()+" "+queryP.value("forma_zajec").toString()[0], ui->kursy_zapisz);
        }

    }
}

void ProwadzacyOkno::on_kursy_zapisz_itemClicked(QListWidgetItem *item)
{
    ui->grupy_zapisz->clear();
    queryP.exec("SELECT * FROM grupa_zajeciowa WHERE id_kursu="+item->text().split(" ")[0]);
    while(queryP.next()){
        if(queryP.value("dzien_tygodnia").toString()!=" ")
        new QListWidgetItem(queryP.value("id_grupy").toString(),ui->grupy_zapisz);
    }

}

void ProwadzacyOkno::on_grupy_zapisz_itemClicked(QListWidgetItem *item)
{
    queryP.exec("SELECT * FROM grupa_zajeciowa WHERE id_grupy="+item->text());
    queryP.next();

    ui->dzien_zajec_zapisz->setText(queryP.value("dzien_tygodnia").toString());
    ui->wolne_miejsca_zapisz->setText(queryP.value("ilosc_miejsc").toString());
    ui->sala_zajeciowa_zapisz->setText(queryP.value("sala_zajeciowa").toString());
    ui->parzystosc_zapisz->setText(queryP.value("parzystosc_tygodnia").toString());

    queryP.exec("SELECT * FROM godziny_zajec WHERE id_godziny_zajec="+queryP.value("id_godziny_zajec").toString());
    queryP.next();

    ui->godziny_zajec_zapisz->setText(queryP.value("godzina_rozpoczecia").toString()+" - "+queryP.value("godzina_zakonczenia").toString());

    queryP.exec("SELECT * FROM grupa_zajeciowa WHERE id_grupy="+item->text());
    queryP.next();

    queryP.exec("SELECT * FROM prowadzący WHERE id_prowadzacego="+queryP.value("id_prowadzacego").toString());
    queryP.next();

    ui->prowadzacy_zapisz->setText(queryP.value("tytul").toString()+" "+queryP.value("imie").toString()+" "+queryP.value("nazwisko").toString());

    queryP.exec("SELECT * FROM grupa_zajeciowa WHERE id_grupy="+item->text());
    queryP.next();

    queryP.exec("SELECT * FROM kurs WHERE id_kursu="+queryP.value("id_kursu").toString());
    queryP.next();

    ui->ects_zapisz->setText(queryP.value("ects").toString());
}

void ProwadzacyOkno::on_zapisz_zapisz_clicked()
{
    if(ui->studenci_zapisz->currentItem() == nullptr){
        QMessageBox::information(this,"Błąd","Wybierz studenta!");
        return;
    }

    QVector<int> grupy;

    if(ui->kursy_zapisz->currentItem() != nullptr){
        queryP.exec("SELECT * FROM grupa_zajeciowa WHERE id_kursu='"+ui->kursy_zapisz->currentItem()->text()+"'");
        while(queryP.next()){
            grupy.append(queryP.value("id_grupy").toInt());
        }
    }

    else if(ui->kursy_zapisz->currentItem() == nullptr){
        QMessageBox::information(this,"Błąd","Wybierz kurs!");
        return;
    }
    if(ui->grupy_zapisz->currentItem() == nullptr){
        QMessageBox::information(this,"Błąd","Wybierz grupę!");
        return;
    }
    else{
        queryP.exec("SELECT * FROM grupa_zajeciowa WHERE id_grupy='"+ui->grupy_zapisz->currentItem()->text()+"'");
        queryP.next();
        ui->wolne_miejsca_zapisz->setText(queryP.value("ilosc_miejsc").toString());
        if(queryP.value("ilosc_miejsc").toInt() == 0){
            QMessageBox::information(this,"Błąd","Brak wolnych miejsc!");
            return;
        }
    }

    queryP.exec("SELECT * FROM zapis WHERE id_indeksu="+ui->studenci_zapisz->currentItem()->text().split(" ")[0]);
    while(queryP.next()){
        for(int i =0; i<grupy.length(); i++){
            if(grupy[i] == queryP.value("id_grupy").toInt()){
                QMessageBox::information(this,"Błąd","Student jest już zapisany!");
                return;
            }
        }
    }

    if(ui->wolne_miejsca_zapisz->text().toInt() >= 0 ){
        queryP.exec("UPDATE grupa_zajeciowa SET ilosc_miejsc=ilosc_miejsc-1 WHERE id_grupy="+ui->grupy_zapisz->currentItem()->text());
        queryP.next();
        queryP.exec("SELECT * FROM zapis WHERE id_zapisu=(SELECT MAX(id_zapisu) FROM zapis)");
        queryP.next();
        int id_do_zapisu = queryP.value("id_zapisu").toInt() + 1;

        queryP.prepare("INSERT INTO zapis (id_zapisu, id_indeksu, id_grupy) "
                       "VALUES (:id_zapisu, :id_indeksu, :id_grupy)");
        queryP.bindValue(":id_zapisu",id_do_zapisu);
        queryP.bindValue(":id_indeksu",ui->studenci_zapisz->currentItem()->text().split(" ")[0]);
        queryP.bindValue(":id_grupy",ui->grupy_zapisz->currentItem()->text().toInt());
        queryP.exec();
        queryP.next();
        QMessageBox::information(this,"","Zapis przeprowadzony poprawnie!");
    }
    else{
        QMessageBox::information(this,"Błąd","Student nie został zapisany!");
        return;
    }

}

void ProwadzacyOkno::on_wypisz_zapisz_clicked()
{
    if(ui->studenci_zapisz->currentItem() == nullptr){
        QMessageBox::information(this,"Błąd","Wybierz studenta!");
        return;
    }

    if(ui->kursy_zapisz->currentItem() == nullptr){
        QMessageBox::information(this,"Błąd","Wybierz kurs!");
        return;
    }

    if(ui->grupy_zapisz->currentItem() == nullptr){
        QMessageBox::information(this,"Błąd","Wybierz grupę!");
        return;
    }

    queryP.exec("SELECT id_grupy="+ui->grupy_zapisz->currentItem()->text()+" FROM zapis WHERE id_indeksu="+ui->studenci_zapisz->currentItem()->text().split(" ")[0]);
    bool czy_zapisany=false;
    while(queryP.next()){
        if(queryP.value("id_grupy="+ui->grupy_zapisz->currentItem()->text()) == "1"){
            czy_zapisany=true;
        }
    }
    if(czy_zapisany == false){
        QMessageBox::information(this,"Błąd","Student nie jest zapisany!");
        return;
    }
    queryP.exec("UPDATE grupa_zajeciowa SET ilosc_miejsc=ilosc_miejsc+1 WHERE id_grupy="+ui->grupy_zapisz->currentItem()->text());
    queryP.next();
    queryP.exec("DELETE FROM zapis WHERE id_grupy="+ui->grupy_zapisz->currentItem()->text()+" AND id_indeksu="+ui->studenci_zapisz->currentItem()->text().split(" ")[0]);
    queryP.next();
    QMessageBox::information(this,"","Student został wypisany!");

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

void ProwadzacyOkno::on_listastudentMODY_itemClicked(QListWidgetItem *item)
{
    QStringList l = item->text().split(' ', QString::SkipEmptyParts);
    queryP.prepare("SELECT * FROM student WHERE id_indeks="+l.first());
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    queryP.next();
    ui->imieMODY->setText(queryP.value("imie").toString());
    ui->nazwiskoMODY->setText(queryP.value("nazwisko").toString());
    ui->peselMODY->setText(queryP.value("pesel").toString());
    ui->kierunekMODY->setText(queryP.value("id_kierunku").toString());
    ui->semestrMODY->setText(queryP.value("semestr").toString());
    ui->plecMODY->setText(queryP.value("plec").toString());
    ui->kierunekMODY->setText(queryP.value("id_kierunku").toString());
    ui->loginMODY->setText(queryP.value("login").toString());
    ui->hasloMODY->setText(queryP.value("haslo").toString());
}

void ProwadzacyOkno::on_pushButton_8_clicked()
{
    queryP.exec("SELECT * FROM student WHERE id_indeks=(SELECT MAX(id_indeks) FROM student)");
    queryP.next();
    int indeks = queryP.value("id_indeks").toInt()+1;
    QString imie = ui->imieMODY->toMarkdown().trimmed();
    QString nazwisko = ui->nazwiskoMODY->toMarkdown().trimmed();
    QString pesel = ui->peselMODY->toMarkdown().trimmed();
    QString login = ui->loginMODY->toMarkdown().trimmed();
    QString haslo = ui->hasloMODY->text();
    QString plec = ui->plecMODY->toMarkdown().trimmed();
    QString kierunek = ui->kierunekMODY->toMarkdown().trimmed();
    QString semestr = ui->semestrMODY->toMarkdown().trimmed();
    QString email = QString::number(indeks) + "@student.pwr.edu.pl";
    //    queryP.prepare("UPDATE student SET imie='"+imie+"',nazwisko='"+nazwisko+"',pesel='"+pesel+"',login='"+login+
    //                   "',haslo='"+haslo+"',plec='"+plec+"',id_kierunku='"+kierunek+"',semestr='"+semestr+"' WHERE id_indeks="+indeks);
    queryP.prepare("INSERT INTO student (id_indeks,imie,nazwisko,pesel,login,haslo,email,plec,id_kierunku,semestr)"
                   "VALUES ('"+QString::number(indeks)+"','"+imie+"','"+nazwisko+"','"+pesel+"','"+login+"','"+haslo+"','"+email+"','"+plec+"','"+kierunek+"','"+semestr+"')");
    if(!queryP.exec()){
        QMessageBox::information(this,"Błąd","Błędne Query");
    }
    else{
        queryP.next();
        QString result = QString::number(indeks) + " " +
                imie + " " +
                nazwisko;
        new QListWidgetItem(result, ui->listastudentMODY);
    }
}

void ProwadzacyOkno::on_pushButton_9_clicked()
{
    if(ui->listastudentMODY->currentItem()!=nullptr){
        QStringList l = ui->listastudentMODY->currentItem()->text().split(' ', QString::SkipEmptyParts);
        queryP.prepare("DELETE FROM student WHERE id_indeks="+l.first());
        if(!queryP.exec()){
            QMessageBox::information(this,"Błąd","Błędne Query");
        }
        queryP.next();
        delete ui->listastudentMODY->currentItem();
    }
}

void ProwadzacyOkno::on_tabWidget_tabBarClicked(int index)
{
    if(index==0){
        ui->listastudentow->clear();
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
    }
    if(index==1){
        ui->listakursow->clear();
        queryP.prepare("SELECT * FROM kurs");
        queryP.exec();
        while(queryP.next()){
            QString result = queryP.value("id_kursu").toString() + " " +
                    queryP.value("nazwa_kursu").toString() + " " +
                    queryP.value("forma_zajec").toString()[0];
            new QListWidgetItem(result, ui->listakursow);
        }
    }
    if(index==2){
        ui->listaKURS->clear();
        ui->listaKURS->setCurrentItem(nullptr);
        queryP.prepare("SELECT * FROM kurs");
        queryP.exec();
        while(queryP.next()){
            QString result = queryP.value("id_kursu").toString() + " " +
                    queryP.value("nazwa_kursu").toString() + " " +
                    queryP.value("forma_zajec").toString()[0];
            new QListWidgetItem(result, ui->listaKURS);
        }
    }
    if(index==3){
        ui->studenci_zapisz->clear();
        ui->kursy_zapisz->clear();
        queryP.prepare("SELECT * FROM kurs");
        queryP.exec();
        while(queryP.next()){
            QString result = queryP.value("id_kursu").toString() + " " +
                    queryP.value("nazwa_kursu").toString() + " " +
                    queryP.value("forma_zajec").toString()[0];
            new QListWidgetItem(result, ui->kursy_zapisz);
        }
        queryP.prepare("SELECT * FROM student");
        queryP.exec();
        while(queryP.next()){
            QString result = queryP.value("id_indeks").toString() + " " +
                    queryP.value("imie").toString() + " " +
                    queryP.value("nazwisko").toString();
            new QListWidgetItem(result, ui->studenci_zapisz);
        }
    }
    if(index==4){
        ui->listastudentMODY->clear();
        queryP.exec("SELECT * FROM student");
        while(queryP.next()){
            QString result = queryP.value("id_indeks").toString() + " " +
                    queryP.value("imie").toString() + " " +
                    queryP.value("nazwisko").toString();
            new QListWidgetItem(result, ui->listastudentMODY);
        }
        ui->listastudentMODY->setCurrentItem(nullptr);
    }
}

void ProwadzacyOkno::on_pushButton_10_clicked()
{
    if(ui->listastudentMODY->currentItem()!=nullptr){
    QStringList l = ui->listastudentMODY->currentItem()->text().split(' ', QString::SkipEmptyParts);
    QString indeks = l.first();
    QString imie = ui->imieMODY->toMarkdown().trimmed();
    QString nazwisko = ui->nazwiskoMODY->toMarkdown().trimmed();
    QString pesel = ui->peselMODY->toMarkdown().trimmed();
    QString login = ui->loginMODY->toMarkdown().trimmed();
    QString haslo = ui->hasloMODY->text();
    QString plec = ui->plecMODY->toMarkdown().trimmed();
    QString kierunek = ui->kierunekMODY->toMarkdown().trimmed();
    QString semestr = ui->semestrMODY->toMarkdown().trimmed();
    queryP.prepare("UPDATE student SET imie='"+imie+"',nazwisko='"+nazwisko+"',pesel='"+pesel+"',login='"+login+
                   "',haslo='"+haslo+"',plec='"+plec+"',id_kierunku='"+kierunek+"',semestr='"+semestr+"' WHERE id_indeks="+indeks);
    queryP.exec();
    queryP.next();
    ui->listastudentMODY->currentItem()->setText(l.first()+" "+imie+" "+nazwisko);
    }
}
