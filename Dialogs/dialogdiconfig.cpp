#include "dialogdiconfig.h"
#include "ui_dialogdiconfig.h"

DialogDIConfig::DialogDIConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDIConfig)
{
    ui->setupUi(this);
}

DialogDIConfig::~DialogDIConfig()
{
    delete ui;
}

void DialogDIConfig::setInputs(const std::vector<QString> names)
{
    std::vector<QLineEdit*> inputsNames = {
        ui->lineEdit_DI1, ui->lineEdit_DI2, ui->lineEdit_DI3, ui->lineEdit_DI4, ui->lineEdit_DI5, ui->lineEdit_DI6, ui->lineEdit_DI7,
        ui->lineEdit_DI8, ui->lineEdit_DI9, ui->lineEdit_DI10, ui->lineEdit_DI11, ui->lineEdit_DI12, ui->lineEdit_DI13, ui->lineEdit_DI14
    };
    if(names.size()>=14) {
        for(int i=0;i<14;i++) {
            inputsNames[i]->setText(names.at(i));
        }
    }
}

std::vector<QString> DialogDIConfig::getInputs() const
{
    std::vector<QLineEdit*> inputsNames = {
        ui->lineEdit_DI1, ui->lineEdit_DI2, ui->lineEdit_DI3, ui->lineEdit_DI4, ui->lineEdit_DI5, ui->lineEdit_DI6, ui->lineEdit_DI7,
        ui->lineEdit_DI8, ui->lineEdit_DI9, ui->lineEdit_DI10, ui->lineEdit_DI11, ui->lineEdit_DI12, ui->lineEdit_DI13, ui->lineEdit_DI14
    };
    std::vector<QString> res;
    for(int i=0;i<14;i++) res.push_back(inputsNames.at(i)->text());
    return res;
}
