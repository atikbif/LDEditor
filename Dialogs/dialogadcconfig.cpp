#include "dialogadcconfig.h"
#include "ui_dialogadcconfig.h"

DialogADCconfig::DialogADCconfig(PLCConfig config, QWidget *parent) :
    QDialog(parent), config(config),
    ui(new Ui::DialogADCconfig)
{
    ui->setupUi(this);
    std::vector<QComboBox*> inputs = {
        ui->comboBox_IN1, ui->comboBox_IN2, ui->comboBox_IN3, ui->comboBox_IN4, ui->comboBox_IN5, ui->comboBox_IN6, ui->comboBox_IN7,
        ui->comboBox_IN8, ui->comboBox_IN9, ui->comboBox_IN10, ui->comboBox_IN11, ui->comboBox_IN12, ui->comboBox_IN13, ui->comboBox_IN14,
    };
    for(int i=0;i<inputs.size();i++) {
        inputs[i]->setCurrentIndex(config.getSensorType(i));
    }
}

DialogADCconfig::~DialogADCconfig()
{
    delete ui;
}

std::vector<int> DialogADCconfig::getSensorTypes() const
{
    std::vector<int> result;
    std::vector<QComboBox*> inputs = {
        ui->comboBox_IN1, ui->comboBox_IN2, ui->comboBox_IN3, ui->comboBox_IN4, ui->comboBox_IN5, ui->comboBox_IN6, ui->comboBox_IN7,
        ui->comboBox_IN8, ui->comboBox_IN9, ui->comboBox_IN10, ui->comboBox_IN11, ui->comboBox_IN12, ui->comboBox_IN13, ui->comboBox_IN14,
    };
    for(int i=0;i<inputs.size();i++) {
        result.push_back(inputs.at(i)->currentIndex());
    }
    return result;
}
