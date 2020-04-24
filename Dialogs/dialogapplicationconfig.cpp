#include "dialogapplicationconfig.h"
#include "ui_dialogapplicationconfig.h"
#include <QRegExp>

DialogApplicationConfig::DialogApplicationConfig(const PLCConfig &config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogApplicationConfig)
{
    ui->setupUi(this);
    ui->lineEditAppName->setText(config.getAppName());
    ui->lineEditCheckSum->setText(QString::number(config.getAppCRC()));
    ui->spinBoxAppCN->setValue(config.getApplicationCN());

    QString vers = config.getAppVersion();
    QRegExp versExp("^(\\d+)\\D(\\d+)$");
    if(versExp.indexIn(vers)!=-1) {
        QString num1 = versExp.cap(1);
        QString num2 = versExp.cap(2);
        int num1Value = num1.toInt();
        int num2Value = num2.toInt();
        if(num1Value<0 || num1Value>100) num1Value = 1;
        while(num2Value>=10) num2Value = num2Value/10;
        double res = num1Value + static_cast<double>(num2Value)/10;
        ui->doubleSpinBoxVersion->setValue(res);
    }
    ui->comboBoxNodeNum->setCurrentText(QString::number(config.getNodeNum()));
    ui->comboBoxclusterNum->setCurrentText(QString::number(config.getClusterNum()));
    ui->lineEditAppTime->setText(config.getAppTime());

    ui->lineEditCheckSum->setVisible(false);

    ui->doubleSpinBoxVersion->setToolTip("Версия проекта (диапазон от 0.0 до 100.0)");
    ui->lineEditAppTime->setToolTip("Время создания или сохранения проекта");
    ui->comboBoxNodeNum->setToolTip("Влияет только на формирование карты памяти для скада системы");
    ui->comboBoxclusterNum->setToolTip("Влияет только на формирование карты памяти для скада системы");
}

DialogApplicationConfig::~DialogApplicationConfig()
{
    delete ui;
}

QString DialogApplicationConfig::getApplicationName() const
{
    return ui->lineEditAppName->text();
}

QString DialogApplicationConfig::getApplicationVersion() const
{
    double vers = ui->doubleSpinBoxVersion->value();
    int rem = remainder(vers,1)*10;
    while(rem>=10) rem = rem/10;
    return QString::number(static_cast<int>(vers)) + "." + QString::number(rem);
}

int DialogApplicationConfig::getClusterNum() const
{
    return ui->comboBoxclusterNum->currentText().toInt();
}

int DialogApplicationConfig::getNodeNum() const
{
    return ui->comboBoxNodeNum->currentText().toInt();
}

int DialogApplicationConfig::getAppCN() const
{
    return ui->spinBoxAppCN->value();
}

void DialogApplicationConfig::accept()
{
    QDialog::accept();
}
