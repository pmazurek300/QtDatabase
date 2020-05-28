#include "studentokno.h"
#include "ui_studentokno.h"

StudentOkno::StudentOkno(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StudentOkno)
{
    ui->setupUi(this);
}
