#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowTitle(tr("Uczelniany system zapisów"));
    //connect(&oknoProwadzacego, SIGNAL(pokazLogowanie()), this, SLOT(logowanie()));
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
    bazaDanych.setDatabaseName(tr("baza"));
        // trzeba dopisac rozroznianie na prowadzacego i studenta (będa emitować 2 inne sygnały)
    QString login = ui->login->text();
    QString haslo = ui->haslo->text();
    if(bazaDanych.open()){
        QSqlQuery query(QSqlDatabase::database("MyConnect"));
        query.prepare("SELECT * FROM pracownik");

        if(!query.exec()){
            QMessageBox::information(this,"Błąd","Błędne Query");
        }
        else{
            while(query.next()){

                QString login_z_bd = query.value("login").toString();
                QString haslo_z_bd = query.value("haslo").toString();
                qDebug() << login_z_bd;
                qDebug() << haslo_z_bd;
                if(login_z_bd == login && haslo_z_bd == haslo){
                    this->hide();
                    oknoProwadzacego = new ProwadzacyOkno(query);
                    connect(oknoProwadzacego, SIGNAL(pokazLogowanie()), this, SLOT(logowanie()));
                    oknoProwadzacego->show();
                }
                else {                //do poprawy zeby nie pokazywalo za kazdym razem w while ( moze sprobowac cos z do while i od warunkow)
                    QMessageBox error;
                    error.setText(tr("Wprowadzone dane są niepoprawne!"));
                    error.setWindowTitle("Błąd");
                    error.exec();
                }
            }
        }
    }

}

void MainWindow::logowanie()
{
    this->show();
}
