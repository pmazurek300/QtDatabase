#include "studentokno.h"
#include "ui_studentokno.h"

StudentOkno::StudentOkno(QSqlQuery & query, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StudentOkno)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Okno studenta"));
}
