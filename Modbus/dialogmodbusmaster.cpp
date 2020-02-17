#include "dialogmodbusmaster.h"
#include "ui_dialogmodbusmaster.h"
#include "dialogaddmodbusvar.h"
#include "dialogeditmodbusvar.h"
#include "dialogconfigmodbuscanal.h"
#include <QMessageBox>

DialogModbusMaster::DialogModbusMaster(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogModbusMaster)
{
    ui->setupUi(this);
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    var.setType(ModbusVar::INP_REG);
    var.setNumber(1);
    var.setComment("");
    var.setCanNumber(2);
    var.setWriteFlag(false);
    var.setActiveFlag(true);
    var.setMemAddress(0);
    var.setNetAddress(1);
    connect(ui->tableWidget,&QTableWidget::cellDoubleClicked,this,[this](){on_pushButtonEdit_clicked();});
}

DialogModbusMaster::~DialogModbusMaster()
{
    delete ui;
}

void DialogModbusMaster::on_pushButtonAddVar_clicked()
{
    auto *dialog = new DialogAddModbusVar(this);
    dialog->setVar(var);
    if(dialog->exec()==QDialog::Accepted) {
        var = dialog->getVar();
        if(!vars.addModbusVar(var)) {
            QMessageBox::warning(this, "Неккоректные данные", "Переменная с таким номером уже используется!");
        }else {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            updateRow(row,var);
            var.setNumber(var.getNumber()+1);
            var.setMemAddress(var.getMemAddr()+1);
        }
    }
    delete dialog;
}

void DialogModbusMaster::on_pushButtonEdit_clicked()
{
    if(ui->tableWidget->rowCount()) {
        QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
        if(items.count()==0) {
            ui->tableWidget->selectRow(0);
            items = ui->tableWidget->selectedItems();
        }
        int varNum=0;
        int varRow=0;
        for(QTableWidgetItem *i:items) {
            if(i->column()==1) {
                varNum=i->text().remove("MODB").toInt();
                varRow=i->row();
            }
        }
        if(varNum>0) {
            QSharedPointer<ModbusVar> selVar = vars.getModbusVarByNum(varNum);
            auto *dialog = new DialogEditModbusVar(this);
            dialog->setVar(*selVar);
            if(dialog->exec()==QDialog::Accepted) {
                ModbusVar v = dialog->getVar();
                if(v.getNumber()!= varNum && vars.isVarExists(v.getNumber())) QMessageBox::warning(this, "Неккоректные данные", "Переменная с таким номером уже используется!");
                else {
                    var = v;
                    vars.replaceModbusVar(varNum,var);
                    updateRow(varRow,var);
                }

            }
            delete dialog;
        }
    }else {
        QMessageBox::warning(this, "Невозможно выполнить операцию", "Отсутствуют переменные для редактирования!");
    }
}

void DialogModbusMaster::on_pushButtonturnOnAll_clicked()
{
    vars.enableAll();
    for(int i=0;i<ui->tableWidget->rowCount();++i) {
        ui->tableWidget->setItem(i,6,new QTableWidgetItem("вкл"));
    }
}

void DialogModbusMaster::on_pushButtonturnOffAll_clicked()
{
    vars.disableAll();
    for(int i=0;i<ui->tableWidget->rowCount();++i) {
        ui->tableWidget->setItem(i,6,new QTableWidgetItem("откл"));
    }
}

void DialogModbusMaster::on_pushButtonDeleteVar_clicked()
{
    if(ui->tableWidget->rowCount()) {
        QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
        if(items.count()==0) {
            ui->tableWidget->selectRow(0);
            items = ui->tableWidget->selectedItems();
        }
        int varNum=0;
        int varRow=0;
        for(QTableWidgetItem *i:items) {
            if(i->column()==1) {
                varNum=i->text().remove("MODB").toInt();
                varRow=i->row();
            }
        }
        if(varNum>0) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение операции",
                                                                      "Удалить переменную MODB" + QString::number(varNum) + "?",
                                        QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                vars.deleteModbusVar(varNum);
                ui->tableWidget->removeRow(varRow);
            }
        }
    }else {
        QMessageBox::warning(this, "Невозможно выполнить операцию", "Отсутствуют переменные для удаления!");
    }
}

void DialogModbusMaster::on_pushButtonCanal_clicked()
{
    auto *dialog = new DialogConfigModbusCanal(this);
    dialog->setCanals(vars.getModbusCanals());
    if(dialog->exec()==QDialog::Accepted) {
        vars.setModbusCanals(dialog->getCanals());
    }
    delete dialog;
}

void DialogModbusMaster::updateRow(int row, const ModbusVar &v)
{
    ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(v.getCanalNumber())));
    ui->tableWidget->setItem(row,1,new QTableWidgetItem("MODB"+QString::number(v.getNumber())));
    ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::number(v.getNetAddress())));
    ModbusVar::memType mType = v.getType();
    if(mType==ModbusVar::INPUTS) ui->tableWidget->setItem(row,3,new QTableWidgetItem("DISCRETE INPUTS"));
    else if(mType==ModbusVar::COILS) ui->tableWidget->setItem(row,3,new QTableWidgetItem("COILS"));
    else if(mType==ModbusVar::HOLD_REG) ui->tableWidget->setItem(row,3,new QTableWidgetItem("HOLDING REGISTERS"));
    else if(mType==ModbusVar::INP_REG) ui->tableWidget->setItem(row,3,new QTableWidgetItem("INPUT REGISTERS"));
    QString memAddr = QString::number(v.getMemAddr(),16);
    if(memAddr.length()<2) memAddr="0"+memAddr;

    memAddr = QString::number(v.getMemAddr()) + " (0x" + memAddr.toUpper() + ")";
    ui->tableWidget->setItem(row,4,new QTableWidgetItem(memAddr));
    ui->tableWidget->setItem(row,5,new QTableWidgetItem(v.getWriteFlag()?"запись":"чтение"));
    ui->tableWidget->setItem(row,6,new QTableWidgetItem(v.getActiveFlag()?"вкл":"откл"));
    ui->tableWidget->setItem(row,7,new QTableWidgetItem(v.getComment()));
}
