#include "dialogcycleconfig.h"
#include "ui_dialogcycleconfig.h"

DialogCycleConfig::DialogCycleConfig(int msCnt, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCycleConfig)
{
    ui->setupUi(this);
    if(msCnt==0) ui->radioButton_0ms->setChecked(true);
    else if(msCnt==1) ui->radioButton_1ms->setChecked(true);
    else if(msCnt==10) ui->radioButton_10ms->setChecked(true);
    else if(msCnt==50) ui->radioButton_50ms->setChecked(true);
    else if(msCnt==100) ui->radioButton_100ms->setChecked(true);
    else if(msCnt==1000) ui->radioButton_1000ms->setChecked(true);
}

DialogCycleConfig::~DialogCycleConfig()
{
    delete ui;
}

int DialogCycleConfig::getDelay() const
{
    if(ui->radioButton_0ms->isChecked()) return 0;
    if(ui->radioButton_1ms->isChecked()) return 1;
    if(ui->radioButton_10ms->isChecked()) return 10;
    if(ui->radioButton_50ms->isChecked()) return 50;
    if(ui->radioButton_100ms->isChecked()) return 100;
    if(ui->radioButton_1000ms->isChecked()) return 1000;
    return 0;
}
