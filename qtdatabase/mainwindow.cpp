#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowTitle("Uczelniany system zapisów");
    connect(&oknoProwadzacego, SIGNAL(pokazLogowanie()), this, SLOT(logowanie()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_zaloguj_clicked()
{
    bazaDanych = QSqlDatabase::addDatabase("QMYSQL");
    bazaDanych.setUserName(ui->login->text());
    bazaDanych.setPassword(ui->haslo->text());
    bazaDanych.setDatabaseName("next_baza");
        // trzeba dopisac rozroznianie na prowadzacego i studenta (będa emitować 2 inne sygnały)
    if(bazaDanych.open()){
        this->hide();
        oknoProwadzacego.show();
    }
    else{
        QMessageBox::information(this,"Błąd","Wprowadzone dane są niepoprawne!");
    }
}

void MainWindow::logowanie(){
    // rozroznianie na prowadzacego i studenta
    this->show();
}
