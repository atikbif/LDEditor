#include "dialogconstvarproperties.h"
#include "ui_dialogconstvarproperties.h"

DialogConstVarProperties::DialogConstVarProperties(PLCVar::varType value, const QString &type, QWidget *parent) :
    QDialog(parent),value(value),type(type),
    ui(new Ui::DialogConstVarProperties)
{
    ui->setupUi(this);
    std::vector<QString> vTypes = PLCVar::getSupportedTypes();
    for(const auto &vType: vTypes) {
        ui->comboBox->addItem(vType);
    }
    ui->comboBox->setCurrentText(type);

    if(auto pval = std::get_if<unsigned short>(&value)) stringValue = QString::number(*pval);
    if(auto pval = std::get_if<unsigned long>(&value)) stringValue = QString::number(*pval);
    if(auto pval = std::get_if<double>(&value)) stringValue = QString::number(*pval);
    if(auto pval = std::get_if<bool>(&value)) {
        stringValue = (*pval?"1":"0");
    }
    if(auto pval = std::get_if<QString>(&value)) stringValue = (*pval);

    ui->lineEdit->setText(stringValue);
}

DialogConstVarProperties::~DialogConstVarProperties()
{
    delete ui;
}

void DialogConstVarProperties::on_DialogConstVarProperties_accepted()
{
    if(type=="unsigned short") {
        auto res = static_cast<quint16>(stringValue.toInt());
        value = static_cast<unsigned short>(res);
    }else if(type=="unsigned long") {
        qint32 res = stringValue.toLong();
        value = static_cast<unsigned long>(res);
    }else if(type=="double") {
        double res = stringValue.toDouble();
        value = res;
    }else if(type=="bool") {
        bool res = stringValue.toShort();
        value = res;
    }
}

void DialogConstVarProperties::on_comboBox_currentTextChanged(const QString &arg1)
{
    type = arg1;
}

void DialogConstVarProperties::on_lineEdit_textChanged(const QString &arg1)
{
    stringValue = arg1;
}
