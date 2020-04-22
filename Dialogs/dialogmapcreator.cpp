#include "dialogmapcreator.h"
#include "ui_dialogmapcreator.h"
#include <QRegExp>

DialogMapCreator::DialogMapCreator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMapCreator)
{
    ui->setupUi(this);
}

DialogMapCreator::~DialogMapCreator()
{
    delete ui;
}

void DialogMapCreator::setApplicationName(const QString &value)
{
    ui->lineEditAppName->setText(value);
}

QString DialogMapCreator::getApplicationName() const
{
    return ui->lineEditAppName->text();
}

void DialogMapCreator::setApplicationVersion(const QString &value)
{
    QRegExp versExp("^(\\d+)\\D(\\d+)$");
    if(versExp.indexIn(value)!=-1) {
        QString num1 = versExp.cap(1);
        QString num2 = versExp.cap(2);
        int num1Value = num1.toInt();
        int num2Value = num2.toInt();
        if(num1Value<0 || num1Value>100) num1Value = 1;
        while(num2Value>=10) num2Value = num2Value/10;
        double res = num1Value + static_cast<double>(num2Value)/10;
        ui->doubleSpinBoxVersion->setValue(res);
    }
}

QString DialogMapCreator::getApplicationVersion() const
{
    double vers = ui->doubleSpinBoxVersion->value();
    int rem = remainder(vers,1)*10;
    while(rem>=10) rem = rem/10;
    return QString::number(static_cast<int>(vers)) + "." + QString::number(rem);
}

void DialogMapCreator::setClusterNum(int value)
{
    ui->comboBoxclusterNum->setCurrentText(QString::number(value));
}

int DialogMapCreator::getClusterNum() const
{
    return ui->comboBoxclusterNum->currentText().toInt();
}

void DialogMapCreator::setNodeNum(int value)
{
    ui->comboBoxNodeNum->setCurrentText(QString::number(value));
}

int DialogMapCreator::getNodeNum() const
{
    return ui->comboBoxNodeNum->currentText().toInt();
}
