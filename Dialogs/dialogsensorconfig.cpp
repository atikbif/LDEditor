#include "dialogsensorconfig.h"
#include "ui_dialogsensorconfig.h"
#include <QRegExp>
#include <QMessageBox>

DialogSensorConfig::DialogSensorConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSensorConfig)
{
    ui->setupUi(this);
    if(editMode) setWindowTitle("редактирование датчика"); else setWindowTitle("добавить датчик");
}

DialogSensorConfig::~DialogSensorConfig()
{
    delete ui;
}

void DialogSensorConfig::setSensor(const AINSensor &sensor)
{
    this->sensor = sensor;
    ui->lineEditName->setText(sensor.getName());
    ui->lineEditUnit->setText(sensor.getUnit());
    ui->lineEditLowLimit->setText(sensor.getLowLimit());
    ui->lineEditHighLimit->setText(sensor.getHighLimit());
    if(sensor.getSensorType()>=0 && sensor.getSensorType()<ui->comboBoxType->count()) {
        ui->comboBoxType->setCurrentIndex(sensor.getSensorType());
    }

}

void DialogSensorConfig::setEditMode(bool value)
{
    editMode = value;
    if(editMode) setWindowTitle("редактирование датчика");
}

AINSensor DialogSensorConfig::getSensor()
{
    sensor.setName(ui->lineEditName->text());
    sensor.setUnit(ui->lineEditUnit->text());
    sensor.setLowLimit(ui->lineEditLowLimit->text());
    sensor.setHighLimit(ui->lineEditHighLimit->text());
    sensor.setSensorType(ui->comboBoxType->currentIndex());
    return sensor;
}

void DialogSensorConfig::accept()
{
    QString name = ui->lineEditName->text();
    QString lowLimit = ui->lineEditLowLimit->text();
    QString highLimit = ui->lineEditHighLimit->text();
    if(!name.isEmpty()) {
        if(!lowLimit.isEmpty()) {
            if(!highLimit.isEmpty()) {
                QDialog::accept();
            }else QMessageBox::warning(this, "Уведомление", "Не введён верхний порог датчика");
        }else QMessageBox::warning(this, "Уведомление", "Не введён нижний порог датчика");
    }else QMessageBox::warning(this, "Уведомление", "Не введёно имя датчика");
}
