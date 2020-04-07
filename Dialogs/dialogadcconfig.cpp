#include "dialogadcconfig.h"
#include "ui_dialogadcconfig.h"
#include "dialogsensorconfig.h"
#include <QDebug>
#include <QMessageBox>
#include <algorithm>
#include "plcvarcontainer.h"
#include "plcutils.h"

DialogADCconfig::DialogADCconfig(PLCConfig config, QWidget *parent) :
    QDialog(parent), config(config),
    ui(new Ui::DialogADCconfig)
{
    ui->setupUi(this);
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableWidget,&QTableWidget::cellDoubleClicked,this,[this](){editSensor();});
    std::vector<QComboBox*> inputs = {
        ui->comboBox_AIN1, ui->comboBox_AIN2, ui->comboBox_AIN3, ui->comboBox_AIN4, ui->comboBox_AIN5, ui->comboBox_AIN6, ui->comboBox_AIN7,
        ui->comboBox_AIN8, ui->comboBox_AIN9, ui->comboBox_AIN10, ui->comboBox_AIN11, ui->comboBox_AIN12, ui->comboBox_AIN13, ui->comboBox_AIN14
    };
    std::vector<QLineEdit*> inputsNames = {
        ui->lineEdit_AIN1, ui->lineEdit_AIN2, ui->lineEdit_AIN3, ui->lineEdit_AIN4, ui->lineEdit_AIN5, ui->lineEdit_AIN6, ui->lineEdit_AIN7,
        ui->lineEdit_AIN8, ui->lineEdit_AIN9, ui->lineEdit_AIN10, ui->lineEdit_AIN11, ui->lineEdit_AIN12, ui->lineEdit_AIN13, ui->lineEdit_AIN14
    };

    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("состояние","Аналоговые входы");
    if(vars.size()>=inputsNames.size()) {
        for(std::size_t i=0;i<inputsNames.size();i++) {
            inputsNames[i]->setValidator(new QRegExpValidator(QRegExp("^[\\w\\s]{0,16}$"), this));
            inputsNames[i]->setText(vars.at(i).getComment());
        }
    }

    std::vector<QString> sensNames;

    for(int i=0;i<config.getADCSensorsCount();i++) {
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(i+1)));
        auto sensor = config.getADCSensor(i);
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(sensor.getName()));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(sensor.getLowLimit()));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(sensor.getHighLimit()));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(sensor.getUnit()));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(sensor.getSensorTypeText()));
        sensNames.push_back(sensor.getName());
    }

    sort( sensNames.begin(), sensNames.end() );
    sensNames.erase( unique( sensNames.begin(), sensNames.end() ), sensNames.end() );

    for(std::size_t i=0;i<inputs.size();i++) {
        for(QString name:sensNames) inputs[i]->addItem(name);
        if(config.getSensorType(static_cast<int>(i))<inputs[i]->count()) inputs[i]->setCurrentIndex(config.getSensorType(static_cast<int>(i)));
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
        ui->comboBox_AIN1, ui->comboBox_AIN2, ui->comboBox_AIN3, ui->comboBox_AIN4, ui->comboBox_AIN5, ui->comboBox_AIN6, ui->comboBox_AIN7,
        ui->comboBox_AIN8, ui->comboBox_AIN9, ui->comboBox_AIN10, ui->comboBox_AIN11, ui->comboBox_AIN12, ui->comboBox_AIN13, ui->comboBox_AIN14,
    };
    for(std::size_t i=0;i<inputs.size();i++) {
        result.push_back(inputs.at(i)->currentIndex());
    }
    return result;
}

void DialogADCconfig::on_pushButtonAddSensor_clicked()
{
    auto *dialog  = new DialogSensorConfig();
    if(dialog->exec()==QDialog::Accepted) {
        auto sensor = dialog->getSensor();
        QString name = sensor.getName();
        QString lowLimit = sensor.getLowLimit();
        QString highLimit = sensor.getHighLimit();
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(row+1)));
        ui->tableWidget->setItem(row,1,new QTableWidgetItem(name));
        ui->tableWidget->setItem(row,2,new QTableWidgetItem(lowLimit));
        ui->tableWidget->setItem(row,3,new QTableWidgetItem(highLimit));
        ui->tableWidget->setItem(row,4,new QTableWidgetItem(sensor.getUnit()));
        ui->tableWidget->setItem(row,5,new QTableWidgetItem(sensor.getSensorTypeText()));
        updateSensNames();
        config.addADCSensor(sensor);
    }
    delete dialog;
}

void DialogADCconfig::on_pushButtonRemoveSensor_clicked()
{
    if(ui->tableWidget->rowCount()) {
        QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
        if(items.count()==0) {
            ui->tableWidget->selectRow(0);
            items = ui->tableWidget->selectedItems();
        }
        int row=0;
        for(QTableWidgetItem *i:items) {
            if(i->column()==1) {
                row=i->row();
            }
        }
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение операции",
                                                                  "Удалить датчик " + ui->tableWidget->item(row,1)->text() + "?",
                                    QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QString name = ui->tableWidget->item(row,1)->text();
            ui->tableWidget->removeRow(row);
            updateTDU();
            updateSensNames();
            auto sensors = config.getADCSensors();
            auto it = std::remove_if(sensors.begin(),sensors.end(),[name](AINSensor sensor) {return sensor.getName()==name;});
            sensors.erase(it,sensors.end());
            config.clearADCSensors();
            for(auto sensor:sensors) config.addADCSensor(sensor);
        }
    }
}

void DialogADCconfig::on_pushButtonUp_clicked()
{
    if(ui->tableWidget->rowCount()) {
        QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
        if(items.count()==0) {
            ui->tableWidget->selectRow(0);
            items = ui->tableWidget->selectedItems();
        }
        int row=0;
        for(QTableWidgetItem *i:items) {
            if(i->column()==1) {
                row=i->row();
            }
        }
        if(row) {
            QString name = ui->tableWidget->item(row,1)->text();
            QString lowLimit = ui->tableWidget->item(row,2)->text();
            QString highLimit = ui->tableWidget->item(row,3)->text();
            QString unit = ui->tableWidget->item(row,4)->text();
            QString type = ui->tableWidget->item(row,5)->text();
            ui->tableWidget->item(row,1)->setText(ui->tableWidget->item(row-1,1)->text());
            ui->tableWidget->item(row,2)->setText(ui->tableWidget->item(row-1,2)->text());
            ui->tableWidget->item(row,3)->setText(ui->tableWidget->item(row-1,3)->text());
            ui->tableWidget->item(row,4)->setText(ui->tableWidget->item(row-1,4)->text());
            ui->tableWidget->item(row,5)->setText(ui->tableWidget->item(row-1,5)->text());
            ui->tableWidget->item(row-1,1)->setText(name);
            ui->tableWidget->item(row-1,2)->setText(lowLimit);
            ui->tableWidget->item(row-1,3)->setText(highLimit);
            ui->tableWidget->item(row-1,4)->setText(unit);
            ui->tableWidget->item(row-1,5)->setText(type);
            updateTDU();
            ui->tableWidget->selectRow(row-1);
            auto sensors = getAINSensors();
            std::iter_swap(sensors.begin()+row,sensors.begin()+row-1);
            config.clearADCSensors();
            for(auto sensor:sensors) config.addADCSensor(sensor);
        }
    }
}

void DialogADCconfig::on_pushButtonDown_clicked()
{
    if(ui->tableWidget->rowCount()) {
        QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
        if(items.count()==0) {
            ui->tableWidget->selectRow(0);
            items = ui->tableWidget->selectedItems();
        }
        int row=0;
        for(QTableWidgetItem *i:items) {
            if(i->column()==1) {
                row=i->row();
            }
        }
        if(row!=ui->tableWidget->rowCount()-1) {
            QString name = ui->tableWidget->item(row,1)->text();
            QString lowLimit = ui->tableWidget->item(row,2)->text();
            QString highLimit = ui->tableWidget->item(row,3)->text();
            QString unit = ui->tableWidget->item(row,4)->text();
            QString type = ui->tableWidget->item(row,5)->text();
            ui->tableWidget->item(row,1)->setText(ui->tableWidget->item(row+1,1)->text());
            ui->tableWidget->item(row,2)->setText(ui->tableWidget->item(row+1,2)->text());
            ui->tableWidget->item(row,3)->setText(ui->tableWidget->item(row+1,3)->text());
            ui->tableWidget->item(row,4)->setText(ui->tableWidget->item(row+1,4)->text());
            ui->tableWidget->item(row,5)->setText(ui->tableWidget->item(row+1,5)->text());
            ui->tableWidget->item(row+1,1)->setText(name);
            ui->tableWidget->item(row+1,2)->setText(lowLimit);
            ui->tableWidget->item(row+1,3)->setText(highLimit);
            ui->tableWidget->item(row+1,4)->setText(unit);
            ui->tableWidget->item(row+1,5)->setText(type);
            updateTDU();
            ui->tableWidget->selectRow(row+1);
            auto sensors = getAINSensors();
            std::iter_swap(sensors.begin()+row,sensors.begin()+row+1);
            config.clearADCSensors();
            for(auto sensor:sensors) config.addADCSensor(sensor);
        }
    }
}

void DialogADCconfig::editSensor()
{
    std::vector<QComboBox*> inputs = {
        ui->comboBox_AIN1, ui->comboBox_AIN2, ui->comboBox_AIN3, ui->comboBox_AIN4, ui->comboBox_AIN5, ui->comboBox_AIN6, ui->comboBox_AIN7,
        ui->comboBox_AIN8, ui->comboBox_AIN9, ui->comboBox_AIN10, ui->comboBox_AIN11, ui->comboBox_AIN12, ui->comboBox_AIN13, ui->comboBox_AIN14,
    };

    if(ui->tableWidget->rowCount()) {
        QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
        if(items.count()==0) {
            ui->tableWidget->selectRow(0);
            items = ui->tableWidget->selectedItems();
        }
        int row=0;
        for(QTableWidgetItem *i:items) {
            if(i->column()==1) {
                row=i->row();
            }
        }

        QString prevName = ui->tableWidget->item(row,1)->text();

        auto *dialog  = new DialogSensorConfig();
        AINSensor sensor;
        sensor.setName(ui->tableWidget->item(row,1)->text());
        sensor.setLowLimit(ui->tableWidget->item(row,2)->text());
        sensor.setHighLimit(ui->tableWidget->item(row,3)->text());
        sensor.setUnit(ui->tableWidget->item(row,4)->text());
        sensor.setSensorTypeText(ui->tableWidget->item(row,5)->text());
        dialog->setSensor(sensor);
        dialog->setEditMode(true);
        if(dialog->exec()==QDialog::Accepted) {
            auto sensor = dialog->getSensor();
            QString name = sensor.getName();
            QString lowLimit = sensor.getLowLimit();
            QString highLimit = sensor.getHighLimit();
            ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(row+1)));
            ui->tableWidget->setItem(row,1,new QTableWidgetItem(name));
            ui->tableWidget->setItem(row,2,new QTableWidgetItem(lowLimit));
            ui->tableWidget->setItem(row,3,new QTableWidgetItem(highLimit));
            ui->tableWidget->setItem(row,4,new QTableWidgetItem(sensor.getUnit()));
            ui->tableWidget->setItem(row,5,new QTableWidgetItem(sensor.getSensorTypeText()));
            updateSensNames(prevName,name);
            auto sensors = getAINSensors();
            sensors[static_cast<std::size_t>(row)] = sensor;
            config.clearADCSensors();
            for(auto sensor:sensors) config.addADCSensor(sensor);
        }
    }
}

void DialogADCconfig::updateTDU()
{
    for(int i=0;i<ui->tableWidget->rowCount();i++) {
        ui->tableWidget->item(i,0)->setText(QString::number(i+1));
    }
}

void DialogADCconfig::updateSensNames()
{
    std::vector<QComboBox*> inputs = {
        ui->comboBox_AIN1, ui->comboBox_AIN2, ui->comboBox_AIN3, ui->comboBox_AIN4, ui->comboBox_AIN5, ui->comboBox_AIN6, ui->comboBox_AIN7,
        ui->comboBox_AIN8, ui->comboBox_AIN9, ui->comboBox_AIN10, ui->comboBox_AIN11, ui->comboBox_AIN12, ui->comboBox_AIN13, ui->comboBox_AIN14,
    };

    std::vector<QString> sensNames;
    for(int i=0;i<ui->tableWidget->rowCount();i++) {
        sensNames.push_back(ui->tableWidget->item(i,1)->text());
    }
    sort( sensNames.begin(), sensNames.end() );
    sensNames.erase( unique( sensNames.begin(), sensNames.end() ), sensNames.end() );
    for(QComboBox* input:inputs) {
        QString current = input->currentText();
        input->clear();
        input->addItem("не подключен");
        for(QString sensName:sensNames) input->addItem(sensName);
        if(std::find(sensNames.begin(),sensNames.end(),current)!=sensNames.end()) input->setCurrentText(current);
        else input->setCurrentIndex(0);
    }
}

void DialogADCconfig::updateSensNames(const QString &prevName, const QString &updName)
{
    std::vector<QComboBox*> inputs = {
        ui->comboBox_AIN1, ui->comboBox_AIN2, ui->comboBox_AIN3, ui->comboBox_AIN4, ui->comboBox_AIN5, ui->comboBox_AIN6, ui->comboBox_AIN7,
        ui->comboBox_AIN8, ui->comboBox_AIN9, ui->comboBox_AIN10, ui->comboBox_AIN11, ui->comboBox_AIN12, ui->comboBox_AIN13, ui->comboBox_AIN14,
    };

    std::vector<QString> sensNames;
    for(int i=0;i<ui->tableWidget->rowCount();i++) {
        sensNames.push_back(ui->tableWidget->item(i,1)->text());
    }
    sort( sensNames.begin(), sensNames.end() );
    sensNames.erase( unique( sensNames.begin(), sensNames.end() ), sensNames.end() );
    for(QComboBox* input:inputs) {
        QString current = input->currentText();
        if(current==prevName) current = updName;
        input->clear();
        input->addItem("не подключен");
        for(QString sensName:sensNames) input->addItem(sensName);
        if(std::find(sensNames.begin(),sensNames.end(),current)!=sensNames.end()) input->setCurrentText(current);
        else input->setCurrentIndex(0);
    }
}

void DialogADCconfig::accept()
{
    std::vector<QLineEdit*> inputsNames = {
        ui->lineEdit_AIN1, ui->lineEdit_AIN2, ui->lineEdit_AIN3, ui->lineEdit_AIN4, ui->lineEdit_AIN5, ui->lineEdit_AIN6, ui->lineEdit_AIN7,
        ui->lineEdit_AIN8, ui->lineEdit_AIN9, ui->lineEdit_AIN10, ui->lineEdit_AIN11, ui->lineEdit_AIN12, ui->lineEdit_AIN13, ui->lineEdit_AIN14
    };

    std::vector<QString> names;
    int row = ui->tableWidget->rowCount();
    for(int i=0;i<row;i++) {
        names.push_back(ui->tableWidget->item(i,1)->text());
    }
    sort(names.begin(), names.end());
    auto it = std::unique( names.begin(), names.end() );
    bool wasUnique = (it == names.end() );
    if(!wasUnique) QMessageBox::warning(this, "Уведомление", "Обнаружены повторяющиеся имена датчиков");
    else {
        std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("состояние","Аналоговые входы");
        if(vars.size()>=inputsNames.size()) {
            for(std::size_t i=0;i<inputsNames.size();i++) {
                PLCUtils::updateSystemVarComment(vars.at(i).getName(),"состояние",inputsNames[i]->text(),"Аналоговые входы");
            }
        }
        QDialog::accept();
    }
}
