#include "dialogadcconfig.h"
#include "ui_dialogadcconfig.h"

DialogADCconfig::DialogADCconfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogADCconfig)
{
    ui->setupUi(this);
}

DialogADCconfig::~DialogADCconfig()
{
    delete ui;
}
