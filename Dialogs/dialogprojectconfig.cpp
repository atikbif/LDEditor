#include "dialogprojectconfig.h"
#include "ui_dialogprojectconfig.h"
#include "plcutils.h"

DialogProjectConfig::DialogProjectConfig(const QString &plcName, int msCnt, QWidget *parent) :
    QDialog(parent),_plcName(plcName),
    ui(new Ui::DialogProjectConfig)
{
    ui->setupUi(this);
    if(msCnt==0) ui->radioButton_0ms->setChecked(true);
    else if(msCnt==1) ui->radioButton_1ms->setChecked(true);
    else if(msCnt==10) ui->radioButton_10ms->setChecked(true);
    else if(msCnt==50) ui->radioButton_50ms->setChecked(true);
    else if(msCnt==100) ui->radioButton_100ms->setChecked(true);
    else if(msCnt==1000) ui->radioButton_1000ms->setChecked(true);

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");

    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->lineEditIP->setValidator(ipValidator);
    ui->groupBox_ETH->setVisible(false);
    if(PLCUtils::isPLCSupportEth(plcName)) {
        ui->groupBox_ETH->setVisible(true);
    }

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

QString DialogProjectConfig::getIP() const
{
    return ui->lineEditIP->text();
}

void DialogProjectConfig::setIP(const QString &value)
{
    ui->lineEditIP->setText(value);
}

bool DialogProjectConfig::useIPasDefault() const
{
    if(PLCUtils::isPLCSupportEth(_plcName)) return ui->checkBoxEthUse->isChecked();
    return false;
}

void DialogProjectConfig::setIPasDefault(bool value)
{
    ui->checkBoxEthUse->setChecked(value);
}
