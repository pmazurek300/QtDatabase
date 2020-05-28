#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowTitle(tr("Uczelniany system zapisów"));
    flaga_logowania=0;
    bazaDanych = QSqlDatabase::addDatabase(tr("QMYSQL"),tr("MyConnect"));
    bazaDanych.setUserName("root");
    bazaDanych.setPassword("");
    bazaDanych.setDatabaseName(tr("next_baza"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_zaloguj_clicked()
{
    QString login = ui->login->text();
        QString haslo = ui->haslo->text();

        if(bazaDanych.open()){
            QSqlQuery query_pracownik(QSqlDatabase::database("MyConnect"));
            QSqlQuery query_student(QSqlDatabase::database("MyConnect"));
            query_pracownik.prepare("SELECT * FROM pracownik");
            query_student.prepare("SELECT * FROM student");

            if(!query_pracownik.exec() || !query_student.exec()){
                QMessageBox::information(this,"Błąd","Błędne Query");
            }

            else{
                while(query_pracownik.next() && this->flaga_logowania==0){

                    QString login_z_bd = query_pracownik.value("login").toString();
                    QString haslo_z_bd = query_pracownik.value("haslo").toString();

                    if(login_z_bd == login && haslo_z_bd == haslo){
                        this->hide();
                        oknoProwadzacego = new ProwadzacyOkno(query_pracownik);
                        connect(oknoProwadzacego, SIGNAL(pokazLogowanie()), this, SLOT(logowanie()));
                        oknoProwadzacego->show();
                        this->flaga_logowania=1;
                        break;
                    }
                }
                while(query_student.next() && this->flaga_logowania==0){
                    QString login_z_bd = query_student.value("login").toString();
                    QString haslo_z_bd = query_student.value("haslo").toString();

                    if(login_z_bd == login && haslo_z_bd == haslo){
                        this->hide();
                        oknoStudenta = new StudentOkno(query_student);
                        oknoStudenta->show();
                        this->flaga_logowania=1;
                        break;
                    }
                }
                if(flaga_logowania ==0){
                    QMessageBox error;
                    error.setText(tr("Wprowadzone dane są niepoprawne!"));
                    error.setWindowTitle("Błąd");
                    error.exec();
                }
            }

        }
}

void MainWindow::logowanie()
{
    this->show();
}
