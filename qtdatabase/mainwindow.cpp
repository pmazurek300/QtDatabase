#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowTitle(tr("Uczelniany system zapisów"));
    connect(&oknoProwadzacego, SIGNAL(pokazLogowanie()), this, SLOT(logowanie()));
  //      connect(this, SIGNAL(loguj_uzytkownika()),&oknoProwadzacego,SLOT(loguj_prowadzacego()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_zaloguj_clicked()
{
    bazaDanych = QSqlDatabase::addDatabase(tr("QMYSQL"),tr("MyConnect"));
    bazaDanych.setUserName("root");
    bazaDanych.setPassword("");
    bazaDanych.setDatabaseName(tr("next_baza"));
        // trzeba dopisac rozroznianie na prowadzacego i studenta (będa emitować 2 inne sygnały)
    QString login = ui->login->text();
    QString haslo = ui->haslo->text();

    if(bazaDanych.open()){
        QSqlQuery query(QSqlDatabase::database("MyConnect"));
        query.prepare("SELECT * FROM pracownik WHERE login = :login AND haslo = :haslo");
        query.bindValue(":login",login);            //poprawic
        query.bindValue(":haslo",haslo);
        qDebug()<< query.value(1).toString();

        if(!query.exec()){
            QMessageBox::information(this,"Failed","Query failed");
        }
        else{
            while(query.next()){

                QString login_z_bd = query.value(1).toString();
                QString haslo_z_bd = query.value(2).toString();

                if(login_z_bd == login && haslo_z_bd == haslo){
                    QMessageBox::information(this,"Success","Login success");
                }
                else{
                    QMessageBox::information(this,"Failed","Login failed");
                }
            }
        }
      // this->hide();

      //  oknoProwadzacego.show();

//    }
//    else{
//        QMessageBox error;
//        error.setText(tr("Wprowadzone dane są niepoprawne!"));
//        error.setWindowTitle("Błąd");
//        error.exec();
    }
}

void MainWindow::logowanie()
{
    this->show();
}
