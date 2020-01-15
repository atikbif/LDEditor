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

void DialogEditSystemVar::on_lineEditComment_textChanged(const QString &arg1)
{
    varComment=arg1;
}
