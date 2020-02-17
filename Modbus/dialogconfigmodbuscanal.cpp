#include "dialogconfigmodbuscanal.h"
#include "ui_dialogconfigmodbuscanal.h"

DialogConfigModbusCanal::DialogConfigModbusCanal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConfigModbusCanal)
{
    ui->setupUi(this);
    setCanals(canals);
}

DialogConfigModbusCanal::~DialogConfigModbusCanal()
{
    delete ui;
}

void DialogConfigModbusCanal::setCanals(ModbusCanals canals)
{
    int canNum = ui->spinBoxCanalNum->value();
    ui->spinBoxMaxLength->setValue(canals.getMaxLength(canNum));
    ui->spinBoxMaxUnusedSpace->setValue(canals.getMaxUnusedSpace(canNum));
    ui->spinBoxPeriod->setValue(canals.getPeriod(canNum));
}

void DialogConfigModbusCanal::on_spinBoxCanalNum_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    setCanals(canals);
}

void DialogConfigModbusCanal::on_spinBoxMaxLength_valueChanged(int arg1)
{
    int canNum = ui->spinBoxCanalNum->value();
    canals.setMaxLength(canNum,arg1);
}

void DialogConfigModbusCanal::on_spinBoxMaxUnusedSpace_valueChanged(int arg1)
{
    int canNum = ui->spinBoxCanalNum->value();
    canals.setMaxUnusedSpace(canNum,arg1);
}

void DialogConfigModbusCanal::on_spinBoxPeriod_valueChanged(int arg1)
{
    int canNum = ui->spinBoxCanalNum->value();
    canals.setPeriod(canNum,arg1);
}
