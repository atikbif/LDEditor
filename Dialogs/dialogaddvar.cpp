#include "dialogaddvar.h"
#include "ui_dialogaddvar.h"
#include "plcvarcontainer.h"
#include <set>
#include <QMessageBox>

QString DialogAddVar::lastGroup="";

DialogAddVar::DialogAddVar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddVar)
{
    ui->setupUi(this);
    std::vector<QString> groups = PLCVarContainer::getInstance().getNotSystemVarGroups();
    for(const QString &group:groups) {
        ui->listWidget->addItem(group);
    }
    if(!groups.empty()) {
        ui->listWidget->setCurrentRow(0);
        for(int i=0;i<ui->listWidget->count();i++) {
            if(ui->listWidget->item(i)->text()==lastGroup) {
                ui->listWidget->setCurrentRow(i);
                break;
            }
        }
    }
    std::vector<QString> types = PLCVar::getSupportedTypes();
    for(const QString &t:types) {
        ui->comboBox->addItem(t);
    }
    ui->lineEditVarName->setFocus();
}

DialogAddVar::~DialogAddVar()
{
    delete ui;
}

void DialogAddVar::on_listWidget_currentRowChanged(int currentRow)
{
    QString group = ui->listWidget->item(currentRow)->text();
    ui->lineEditGroupName->setText(group);
}

void DialogAddVar::on_DialogAddVar_accepted()
{
    varName = ui->lineEditVarName->text();
    if(varName.isEmpty()) {
        QMessageBox::information(this,tr("Сообщение"),tr("Пустое имя переменной"));
    }else {
        grName = ui->lineEditGroupName->text();
        /*if(grName.isEmpty()) {
            QMessageBox::information(this,tr("Сообщение"),tr("Некорректное название группы"));
        }else*/ {
            lastGroup = grName;
            if(PLCVarContainer::getInstance().getVarByGroupAndName(grName,varName)) {
                QMessageBox::information(this,tr("Сообщение"),tr("Переменная с таким именем и группой уже существует"));
            }else {
                PLCVar var(varName,grName);
                var.setComment(ui->lineEditComment->text());
                var.setReadable(true);
                var.setWriteable(true);
                QString type = ui->comboBox->currentText();
                if(type=="short") var.setValue(static_cast<short>(0));
                else if(type=="long") var.setValue(0l);
                else if(type=="double") var.setValue(0.0);
                else if(type=="bool") var.setValue(false);
                PLCVarContainer::getInstance().addVar(var);
            }
        }
    }
}

void DialogAddVar::on_lineEditVarName_textChanged(const QString &arg1)
{
    if(arg1.contains(QRegExp("[А-Яа-я]"))) {
        QMessageBox::information(this,tr("Сообщение"),tr("Имя переменной не должно содержать символов кириллицы"));
        ui->lineEditVarName->setText("");
    }
}
