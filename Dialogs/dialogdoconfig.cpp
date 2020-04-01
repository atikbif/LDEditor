#include "dialogdoconfig.h"
#include "ui_dialogdoconfig.h"

DialogDOConfig::DialogDOConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDOConfig)
{
    ui->setupUi(this);
}

DialogDOConfig::~DialogDOConfig()
{
    delete ui;
}

void DialogDOConfig::setOuts(const std::vector<QString> names)
{
    std::vector<QLineEdit*> outsNames = {ui->lineEdit_DO1, ui->lineEdit_DO2, ui->lineEdit_DO3, ui->lineEdit_DO4, ui->lineEdit_DO5, ui->lineEdit_DO6};
    if(names.size()>=6) {
        for(int i=0;i<6;i++) {
            outsNames[i]->setText(names.at(i));
        }
    }
}

std::vector<QString> DialogDOConfig::getOuts() const
{
    std::vector<QLineEdit*> outsNames = {ui->lineEdit_DO1, ui->lineEdit_DO2, ui->lineEdit_DO3, ui->lineEdit_DO4, ui->lineEdit_DO5, ui->lineEdit_DO6};
    std::vector<QString> res;
    for(int i=0;i<6;i++) res.push_back(outsNames.at(i)->text());
    return res;
}
