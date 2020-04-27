#include "dialogeditnotsystemvar.h"
#include "ui_dialogeditnotsystemvar.h"
#include "plcvarcontainer.h"
#include <QMessageBox>

DialogEditNotSystemVar::DialogEditNotSystemVar(const QString &varName, const QString &varType, const QString &varComment, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditNotSystemVar)
{
    ui->setupUi(this);
    ui->lineEditVarName->setText(varName);
    ui->lineEditComment->setText(varComment);
    std::vector<QString> types = PLCVar::getSupportedTypes();
    for(const QString &type:types) {
        ui->comboBoxVarType->addItem(type);
    }
    ui->comboBoxVarType->setCurrentText(varType);
}

DialogEditNotSystemVar::~DialogEditNotSystemVar()
{
    delete ui;
}

QString DialogEditNotSystemVar::getInitValue() const
{
    return ui->lineEditInitValue->text();
}

void DialogEditNotSystemVar::setInitValue(const QString &value) const
{
    ui->lineEditInitValue->setText(value);
}

void DialogEditNotSystemVar::on_lineEditVarName_textChanged(const QString &arg1)
{
    if(arg1.contains(QRegExp("[А-Яа-я]"))) {
        QMessageBox::warning(this,tr("Некорректное имя"), tr("Имена переменных не должны содержать символов кириллицы"));
        ui->lineEditVarName->setText(varName);
    }else varName=arg1;
}

void DialogEditNotSystemVar::on_comboBoxVarType_currentTextChanged(const QString &arg1)
{
    varType=arg1;
}

void DialogEditNotSystemVar::on_lineEditComment_textChanged(const QString &arg1)
{
    varComment=arg1;
}
