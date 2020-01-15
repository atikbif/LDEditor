#include "dialogprojectconfig.h"
#include "ui_dialogprojectconfig.h"

DialogProjectConfig::DialogProjectConfig(int msCnt, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProjectConfig)
{
    ui->setupUi(this);
    if(msCnt==0) ui->radioButton_0ms->setChecked(true);
    else if(msCnt==1) ui->radioButton_1ms->setChecked(true);
    else if(msCnt==10) ui->radioButton_10ms->setChecked(true);
    else if(msCnt==50) ui->radioButton_50ms->setChecked(true);
    else if(msCnt==100) ui->radioButton_100ms->setChecked(true);
    else if(msCnt==1000) ui->radioButton_1000ms->setChecked(true);
}

DialogProjectConfig::~DialogProjectConfig()
{
    delete ui;
}

int DialogProjectConfig::getDelay() const
{
    if(ui->radioButton_0ms->isChecked()) return 0;
    if(ui->radioButton_1ms->isChecked()) return 1;
    if(ui->radioButton_10ms->isChecked()) return 10;
    if(ui->radioButton_50ms->isChecked()) return 50;
    if(ui->radioButton_100ms->isChecked()) return 100;
    if(ui->radioButton_1000ms->isChecked()) return 1000;
    return 0;
}

int DialogProjectConfig::getNetAddress() const
{
    return ui->spinBoxNetAddr->value();
}

void DialogProjectConfig::setNetAddress(int value)
{
    ui->spinBoxNetAddr->setValue(value);
}

QString DialogProjectConfig::getBaudrate() const
{
    return ui->comboBoxSpeed->currentText();
}

void DialogProjectConfig::setBaudrate(const QString &value)
{
    ui->comboBoxSpeed->setCurrentText(value);
}

QString DialogProjectConfig::getParity() const
{
    return  ui->comboBoxParity->currentText();
}

void DialogProjectConfig::setParity(const QString &value)
{
    ui->comboBoxParity->setCurrentText(value);
}

int DialogProjectConfig::getStopBits() const
{
    return ui->comboBoxStopBits->currentIndex()+1;
}

void DialogProjectConfig::setStopBits(int value)
{
    if(value<1 || value>2) value = 1;
    ui->comboBoxStopBits->setCurrentIndex(value-1);
}
