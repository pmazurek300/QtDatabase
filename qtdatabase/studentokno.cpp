#include "studentokno.h"
#include "ui_studentokno.h"

StudentOkno::StudentOkno(QSqlQuery & query, QString &identyfikator, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StudentOkno)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Okno studenta"));

    queryS=query;
    identyfikator_studenta=identyfikator;

    timerzegara = new QTimer();
    connect(timerzegara,SIGNAL(timeout()),this,SLOT(zegar_tick()));
    timerzegara->start(1000);
    query.prepare("SELECT * FROM student WHERE login='"+identyfikator_studenta+"'");
    query.exec();
    query.next();
    ui->imie->setText("Imię: "+query.value("imie").toString());
    ui->nazwisko->setText("Nazwisko: "+query.value("nazwisko").toString());
    ui->indeks->setText("Indeks: "+query.value("id_indeks").toString());
    ui->kierunek->setText("Kierunek: "+query.value("id_kierunku").toString());
    ui->semestr->setText("Semestr: "+query.value("semestr").toString());
    ui->czas_zapisow->setText(query.value("termin_zapisow").toDateTime().toString(Qt::ISODate).replace(10,1," "));
    ui->prawo_do_zapisow->setText(query.value("prawo_do_zapisow").toString());

    query.prepare("SELECT * FROM kurs");
    query.exec();
    while(query.next()){
        QString result = query.value("id_kursu").toString()+ " " +
                         query.value("nazwa_kursu").toString() + " " +
                         query.value("forma_zajec").toString()[0];
    new QListWidgetItem(result, ui->kursy);
    }

    queryS.exec("SELECT * FROM zapis WHERE id_indeksu="+ui->indeks->text().split(" ")[1]);
    while(queryS.next()){
        new QListWidgetItem(queryS.value("id_grupy").toString(), ui->grupy_zapisany);
    }


}

void StudentOkno::zegar_tick(){
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    ui->zegar->display(text);
}

void StudentOkno::on_wyloguj_clicked()
{
    emit pokazLogowanie();
    delete this;
}

void StudentOkno::on_kursy_itemClicked(QListWidgetItem *item)
{
    ui->grupy->clear();
    QStringList l = item->text().split(' ', QString::SkipEmptyParts);

        queryS.prepare("SELECT * FROM grupa_zajeciowa WHERE id_kursu="+l.first());
        if(!queryS.exec()){
            QMessageBox::information(this,"Błąd","Błędne Query");
        }
        while(queryS.next()){
            if(queryS.value("dzien_tygodnia").toString()!=" ")
            new QListWidgetItem("AREK"+queryS.value("id_grupy").toString()+item->text()[item->text().size()-1], ui->grupy);
        }
}

void StudentOkno::on_grupy_itemClicked(QListWidgetItem *item)
{
    QStringList l = ui->kursy->currentItem()->text().split(' ', QString::SkipEmptyParts);

    queryS.prepare("SELECT * FROM kurs WHERE id_kursu="+l.first());
    queryS.exec();
    queryS.next();
    ui->forma_zajec->setText(queryS.value("forma_zajec").toString());
    ui->ects->setText(queryS.value("ects").toString());


    queryS.prepare("SELECT * FROM grupa_zajeciowa WHERE id_grupy="+item->text().remove(0,4).remove(item->text().remove(0,4).size()-1,item->text().remove(0,4).size()) +" AND id_kursu="+l.first());
    queryS.exec();
    queryS.next();
    ui->dzien_zajec->setText(queryS.value("dzien_tygodnia").toString());
    ui->parzystosc->setText(queryS.value("parzystosc_tygodnia").toString());
    ui->wolne_miejsca->setText(queryS.value("ilosc_miejsc").toString());
    ui->sala_zajec->setText(queryS.value("sala_zajeciowa").toString());
    QString id_prowadzacego = queryS.value("id_prowadzacego").toString();
    QString id_godziny_zajec = queryS.value("id_godziny_zajec").toString();

    queryS.prepare("SELECT * FROM godziny_zajec WHERE id_godziny_zajec="+id_godziny_zajec);
    queryS.exec();
    queryS.next();
    ui->godzina->setText(queryS.value("godzina_rozpoczecia").toString()+" - "+queryS.value("godzina_zakonczenia").toString());

    queryS.prepare("SELECT * FROM prowadzący WHERE id_prowadzacego="+id_prowadzacego);
    queryS.exec();
    queryS.next();
    ui->prowadzacy->setText(queryS.value("tytul").toString()+" "+queryS.value("imie").toString()+" "+queryS.value("nazwisko").toString());

}

void StudentOkno::on_zapisz_clicked()
{
    QVector<int> grupy;

    if(ui->kursy->currentItem() != nullptr){
        queryS.exec("SELECT * FROM grupa_zajeciowa WHERE id_kursu='"+ui->kursy->currentItem()->text()+"'");
        while(queryS.next()){
            grupy.append(queryS.value("id_grupy").toInt());
        }
    }
    else if(ui->kursy->currentItem() == nullptr){
        QMessageBox::information(this,"Błąd","Wybierz kurs!");
        return;
    }

    if(ui->grupy->currentItem() == nullptr){
        QMessageBox::information(this,"Błąd","Wybierz grupę!");
        return;
    }
    else{
        queryS.exec("SELECT * FROM grupa_zajeciowa WHERE id_grupy='"+ui->grupy->currentItem()->text().remove(0,4).remove(ui->grupy->currentItem()->text().remove(0,4).size()-1,ui->grupy->currentItem()->text().remove(0,4).size())+"'");
        queryS.next();
        ui->wolne_miejsca->setText(queryS.value("ilosc_miejsc").toString());
        if(queryS.value("ilosc_miejsc").toInt() == 0){
            QMessageBox::information(this,"Błąd","Brak wolnych miejsc!");
            return;
        }
    }

     queryS.exec("SELECT * FROM zapis WHERE id_indeksu="+ui->indeks->text().split(" ")[1]);
     while(queryS.next() &&  ui->grupy->currentItem() != nullptr){
         for(int i =0; i<grupy.length(); i++){
             if(grupy[i] == queryS.value("id_grupy").toInt()){
                 QMessageBox::information(this,"Błąd","Jesteś już zapisany(a)!");
                 return;
             }
        }
    }

    queryS.exec("SELECT * FROM student WHERE login='"+identyfikator_studenta+"'");
    queryS.next();
    QDateTime tim;
    if(queryS.value("termin_zapisow").toDateTime().toSecsSinceEpoch() <= tim.currentSecsSinceEpoch() && queryS.value("prawo_do_zapisow").toString() == "posiada" && ui->wolne_miejsca->text().toInt() >= 0 ){
        queryS.exec("UPDATE grupa_zajeciowa SET ilosc_miejsc=ilosc_miejsc-1 WHERE id_grupy="+ui->grupy->currentItem()->text().remove(0,4).remove(ui->grupy->currentItem()->text().remove(0,4).size()-1,ui->grupy->currentItem()->text().remove(0,4).size()));
        queryS.next();
        queryS.exec("SELECT * FROM zapis WHERE id_zapisu=(SELECT MAX(id_zapisu) FROM zapis)");
        queryS.next();
        int id_do_zapisu = queryS.value("id_zapisu").toInt() + 1;

        queryS.prepare("INSERT INTO zapis (id_zapisu, id_indeksu, id_grupy) "
                       "VALUES (:id_zapisu, :id_indeksu, :id_grupy)");
        queryS.bindValue(":id_zapisu",id_do_zapisu);
        queryS.bindValue(":id_indeksu",ui->indeks->text().split(" ")[1].toInt());
        queryS.bindValue(":id_grupy",ui->grupy->currentItem()->text().remove(0,4).remove(ui->grupy->currentItem()->text().remove(0,4).size()-1,ui->grupy->currentItem()->text().remove(0,4).size()).toInt());
        queryS.exec();
        queryS.next();
        QMessageBox::information(this,"","Zapis przeprowadzony poprawnie!");
    }
    else{
        QMessageBox::information(this,"Błąd","Nie zostałeś(aś) zapisany(a)!");
        return;
    }

}

void StudentOkno::on_tabWidget_tabBarClicked(int index)
{
    if(index ==0){
        ui->kursy->clear();
        ui->grupy->clear();
        ui->kursy->setCurrentItem(nullptr);
        queryS.exec("SELECT * FROM kurs");
        while(queryS.next()){
            QString result = queryS.value("id_kursu").toString()+ " " +
                             queryS.value("nazwa_kursu").toString() + " " +
                             queryS.value("forma_zajec").toString()[0];
        new QListWidgetItem(result, ui->kursy);
        }
    }
    if(index ==1){
        QVector<QString> grupy;
        ui->grupy_zapisany->clear();
        queryS.exec("SELECT * FROM zapis WHERE id_indeksu="+ui->indeks->text().split(" ")[1]);
        while(queryS.next()){
            grupy.append(queryS.value("id_grupy").toString());
        }
        for(int i =0;i<grupy.size();i++){
            queryS.exec("SELECT * FROM grupa_zajeciowa WHERE id_grupy="+grupy[i]);
            queryS.next();
            queryS.exec("SELECT * FROM kurs WHERE id_kursu="+queryS.value("id_kursu").toString());
            queryS.next();
            new QListWidgetItem("AREK"+grupy[i]+queryS.value("forma_zajec").toString()[0], ui->grupy_zapisany);
        }
        QVector<QString> kursy;
        int ects=0;
        for(int i=0; i<grupy.size(); i++){
            queryS.exec("SELECT * FROM grupa_zajeciowa WHERE id_grupy="+grupy[i]);
            queryS.next();
            kursy.append(queryS.value("id_kursu").toString());

            queryS.exec("SELECT * FROM kurs WHERE id_kursu="+kursy[i]);
            queryS.next();
            ects+=queryS.value("ects").toInt();
        }
        ui->ects_ogol->setText(QString::number(ects));

    }
    if(index ==2){
        QVector<QString> grupy;
        ui->grupy_przeg->clear();
        ui->godziny_zajec_przeg->clear();
        queryS.exec("SELECT * FROM grupa_zajeciowa");
        while(queryS.next()){
            if(queryS.value("dzien_tygodnia").toString()!=" ")
            grupy.append(queryS.value("id_grupy").toString());
        }
        for(int i =0;i<grupy.size();i++){
            queryS.exec("SELECT * FROM grupa_zajeciowa WHERE id_grupy="+grupy[i]);
            queryS.next();
            queryS.exec("SELECT * FROM kurs WHERE id_kursu="+queryS.value("id_kursu").toString());
            queryS.next();
            new QListWidgetItem("AREK"+grupy[i]+queryS.value("forma_zajec").toString()[0],ui->grupy_przeg);
        }

        queryS.exec("SELECT * FROM godziny_zajec");
        while(queryS.next()){
            new QListWidgetItem(queryS.value("godzina_rozpoczecia").toString()+" - "+queryS.value("godzina_zakonczenia").toString(),ui->godziny_zajec_przeg);
        }

    }
}

void StudentOkno::on_grupy_zapisany_itemClicked(QListWidgetItem *item)
{
    queryS.exec("SELECT * FROM grupa_zajeciowa WHERE id_grupy='"+ui->grupy_zapisany->currentItem()->text().remove(0,4).remove(item->text().remove(0,4).size()-1,item->text().remove(0,4).size())+"'");
    queryS.next();
    ui->parzystosc_grupy->setText(queryS.value("parzystosc_tygodnia").toString());
    ui->sala_zajeciowa_grupy->setText(queryS.value("sala_zajeciowa").toString());
    ui->dzien_zajec_grupy->setText(queryS.value("dzien_tygodnia").toString());

    queryS.exec("SELECT * FROM kurs WHERE id_kursu="+queryS.value("id_kursu").toString());
    queryS.next();
    ui->kurs_grupy->setText(queryS.value("nazwa_kursu").toString());
    ui->forma_zajec_grupy->setText(queryS.value("forma_zajec").toString());
    ui->ects_grupy->setText(queryS.value("ects").toString());

    queryS.exec("SELECT * FROM grupa_zajeciowa WHERE id_grupy='"+ui->grupy_zapisany->currentItem()->text().remove(0,4).remove(item->text().remove(0,4).size()-1,item->text().remove(0,4).size())+"'");
    queryS.next();

    queryS.exec("SELECT * FROM prowadzący WHERE id_prowadzacego="+queryS.value("id_prowadzacego").toString());
    queryS.next();
    ui->prowadzacy_grupy->setText(queryS.value("tytul").toString()+" "+queryS.value("imie").toString()+" "+queryS.value("nazwisko").toString());

    queryS.exec("SELECT * FROM grupa_zajeciowa WHERE id_grupy='"+ui->grupy_zapisany->currentItem()->text().remove(0,4).remove(item->text().remove(0,4).size()-1,item->text().remove(0,4).size())+"'");
    queryS.next();

    queryS.exec("SELECT * FROM godziny_zajec WHERE id_godziny_zajec="+queryS.value("id_godziny_zajec").toString());
    queryS.next();
    ui->godziny_zajec_grupy->setText(queryS.value("godzina_rozpoczecia").toString()+" - "+queryS.value("godzina_zakonczenia").toString());

}

void StudentOkno::on_grupy_przeg_itemClicked(QListWidgetItem *item)
{
    queryS.exec("SELECT * FROM grupa_zajeciowa WHERE id_grupy='"+ui->grupy_przeg->currentItem()->text().remove(0,4).remove(item->text().remove(0,4).size()-1,item->text().remove(0,4).size())+"'");
    queryS.next();
    ui->parzystosc_przeg->setText(queryS.value("parzystosc_tygodnia").toString());
    ui->sala_przeg->setText(queryS.value("sala_zajeciowa").toString());
    ui->dzien_przeg->setText(queryS.value("dzien_tygodnia").toString());
    ui->wolne_przeg->setText(queryS.value("ilosc_miejsc").toString());

    queryS.exec("SELECT * FROM kurs WHERE id_kursu="+queryS.value("id_kursu").toString());
    queryS.next();
    ui->nazwa_kursu_przeg->setText(queryS.value("nazwa_kursu").toString());
    ui->forma_zajec_przeg->setText(queryS.value("forma_zajec").toString());
    ui->ects_przeg->setText(queryS.value("ects").toString());

    queryS.exec("SELECT * FROM grupa_zajeciowa WHERE id_grupy='"+ui->grupy_przeg->currentItem()->text().remove(0,4).remove(item->text().remove(0,4).size()-1,item->text().remove(0,4).size())+"'");
    queryS.next();

    queryS.exec("SELECT * FROM prowadzący WHERE id_prowadzacego="+queryS.value("id_prowadzacego").toString());
    queryS.next();
    ui->prowadzacy_przeg->setText(queryS.value("tytul").toString()+" "+queryS.value("imie").toString()+" "+queryS.value("nazwisko").toString());

    queryS.exec("SELECT * FROM grupa_zajeciowa WHERE id_grupy='"+ui->grupy_przeg->currentItem()->text().remove(0,4).remove(item->text().remove(0,4).size()-1,item->text().remove(0,4).size())+"'");
    queryS.next();

    queryS.exec("SELECT * FROM godziny_zajec WHERE id_godziny_zajec="+queryS.value("id_godziny_zajec").toString());
    queryS.next();
    ui->godz_przeg->setText(queryS.value("godzina_rozpoczecia").toString()+" - "+queryS.value("godzina_zakonczenia").toString());


}

void StudentOkno::on_wypisz_clicked()
{
    if(ui->kursy->currentItem() == nullptr){
        QMessageBox::information(this,"Błąd","Wybierz kurs!");
        return;
    }

    if(ui->grupy->currentItem() == nullptr){
        QMessageBox::information(this,"Błąd","Wybierz grupę!");
        return;
    }

    queryS.exec("SELECT id_grupy="+ui->grupy->currentItem()->text().remove(0,4).remove(ui->grupy->currentItem()->text().remove(0,4).size()-1,ui->grupy->currentItem()->text().remove(0,4).size())+" FROM zapis WHERE id_indeksu="+ui->indeks->text().split(" ")[1]);
    bool czy_zapisany=false;
    while(queryS.next()){
        if(queryS.value("id_grupy="+ui->grupy->currentItem()->text().remove(0,4).remove(ui->grupy->currentItem()->text().remove(0,4).size()-1,ui->grupy->currentItem()->text().remove(0,4).size())) == "1"){
            czy_zapisany=true;
        }
    }
    if(czy_zapisany == false){
        QMessageBox::information(this,"Błąd","Nie byłeś(aś) zapisany(a)!");
        return;
    }

    queryS.exec("SELECT * FROM student WHERE login='"+identyfikator_studenta+"'");
    queryS.next();
    QDateTime tim;
    if(queryS.value("termin_zapisow").toDateTime().toSecsSinceEpoch() <= tim.currentSecsSinceEpoch() && queryS.value("prawo_do_zapisow").toString() == "posiada"){
        queryS.exec("UPDATE grupa_zajeciowa SET ilosc_miejsc=ilosc_miejsc+1 WHERE id_grupy="+ui->grupy->currentItem()->text().remove(0,4).remove(ui->grupy->currentItem()->text().remove(0,4).size()-1,ui->grupy->currentItem()->text().remove(0,4).size()));
        queryS.next();
        queryS.exec("DELETE FROM zapis WHERE id_grupy="+ui->grupy->currentItem()->text().remove(0,4).remove(ui->grupy->currentItem()->text().remove(0,4).size()-1,ui->grupy->currentItem()->text().remove(0,4).size())+" AND id_indeksu="+ui->indeks->text().split(" ")[1]);
        queryS.next();
        QMessageBox::information(this,"","Zostałeś(aś) wypisany(a)!");
        return;
    }
    else{
        QMessageBox::information(this,"Błąd","Nie zostałeś(aś) wypisany(a)!");
        return;
    }
}
