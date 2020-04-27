#include "dialogeditsystemvar.h"
#include "ui_dialogeditsystemvar.h"

DialogEditSystemVar::DialogEditSystemVar(const QString &varComment, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditSystemVar)
{
    ui->setupUi(this);
    ui->lineEditComment->setText(varComment);
}

DialogEditSystemVar::~DialogEditSystemVar()
{
    delete ui;
}

void DialogEditSystemVar::setInitValue(const QString &value)
{
    ui->lineEditInitValue->setText(value);
}

QString DialogEditSystemVar::getInitValue() const
{
    return ui->lineEditInitValue->text();
}

void DialogEditSystemVar::on_lineEditComment_textChanged(const QString &arg1)
{
    varComment=arg1;
}
