#include "dialogaddmodbusvar.h"
#include "ui_dialogaddmodbusvar.h"

DialogAddModbusVar::DialogAddModbusVar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddModbusVar)
{
    ui->setupUi(this);
}

DialogAddModbusVar::~DialogAddModbusVar()
{
    delete ui;
}

void DialogAddModbusVar::setVar(ModbusVar value)
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

void DialogAddModbusVar::on_spinBoxVarNum_valueChanged(int arg1)
{
    var.setNumber(arg1);
}

void DialogAddModbusVar::on_spinBoxCanNum_valueChanged(int arg1)
{
    var.setCanNumber(arg1);
}

void DialogAddModbusVar::on_comboBox_currentIndexChanged(int index)
{
    var.setType(ModbusVar::memType(index));
}

void DialogAddModbusVar::on_spinBoxNetAddr_valueChanged(int arg1)
{
    var.setNetAddress(arg1);
}

void DialogAddModbusVar::on_spinBoxMemAddr_valueChanged(int arg1)
{
    var.setMemAddress(arg1);
}

void DialogAddModbusVar::on_lineEditComment_textChanged(const QString &arg1)
{
    var.setComment(arg1);
}

void DialogAddModbusVar::on_radioButtonRead_clicked()
{
    var.setWriteFlag(false);
}

void DialogAddModbusVar::on_radioButtonWrite_clicked()
{
    var.setWriteFlag(true);
}

void DialogAddModbusVar::on_checkBoxActive_clicked()
{
    var.setActiveFlag(ui->checkBoxActive->isChecked());
}
