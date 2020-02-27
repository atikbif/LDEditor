#include "dialogeditmodbusvar.h"
#include "ui_dialogeditmodbusvar.h"

DialogEditModbusVar::DialogEditModbusVar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditModbusVar)
{
    ui->setupUi(this);
}

DialogEditModbusVar::~DialogEditModbusVar()
{
    delete ui;
}

void DialogEditModbusVar::setVar(ModbusVar value)
{
    var = value;
    ui->spinBoxCanNum->setValue(var.getCanalNumber());
    ui->spinBoxVarNum->setValue(var.getNumber());
    ui->spinBoxMemAddr->setValue(var.getMemAddr());
    ui->spinBoxNetAddr->setValue(var.getNetAddress());
    ui->checkBoxActive->setChecked(var.getActiveFlag());
    if(var.getWriteFlag()) ui->radioButtonWrite->setChecked(true);
    else ui->radioButtonRead->setChecked(true);
    ui->lineEditComment->setText(var.getComment());
    ui->comboBox->setCurrentIndex(var.getType());
}

void DialogEditModbusVar::on_spinBoxVarNum_valueChanged(int arg1)
{
    var.setNumber(arg1);
}

void DialogEditModbusVar::on_spinBoxCanNum_valueChanged(int arg1)
{
    var.setCanNumber(arg1);
}

void DialogEditModbusVar::on_comboBox_currentIndexChanged(int index)
{
    var.setType(ModbusVar::memType(index));
    if(ModbusVar::memType(index)==ModbusVar::INPUTS || ModbusVar::memType(index)==ModbusVar::INP_REG) {
        var.setWriteFlag(false);
        ui->radioButtonRead->setChecked(true);
    }
}

void DialogEditModbusVar::on_spinBoxNetAddr_valueChanged(int arg1)
{
    var.setNetAddress(arg1);
}

void DialogEditModbusVar::on_spinBoxMemAddr_valueChanged(int arg1)
{
    var.setMemAddress(arg1);
}

void DialogEditModbusVar::on_lineEditComment_textChanged(const QString &arg1)
{
    var.setComment(arg1);
}

void DialogEditModbusVar::on_radioButtonRead_clicked()
{
    var.setWriteFlag(false);
}

void DialogEditModbusVar::on_radioButtonWrite_clicked()
{
    if(ModbusVar::memType(ui->comboBox->currentIndex())==ModbusVar::INPUTS || ModbusVar::memType(ui->comboBox->currentIndex())==ModbusVar::INP_REG) {
        var.setWriteFlag(false);
        ui->radioButtonRead->setChecked(true);
    }else {
        var.setWriteFlag(true);
    }
}

void DialogEditModbusVar::on_checkBoxActive_clicked()
{
    var.setActiveFlag(ui->checkBoxActive->isChecked());
}

