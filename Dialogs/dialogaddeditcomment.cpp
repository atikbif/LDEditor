#include "dialogaddeditcomment.h"
#include "ui_dialogaddeditcomment.h"

DialogAddEditComment::DialogAddEditComment(const QString &comment, QWidget *parent) :
    QDialog(parent),comment(comment),
    ui(new Ui::DialogAddEditComment)
{
    ui->setupUi(this);
    ui->lineEdit->setText(comment);
    ui->lineEdit->setSelection(0,comment.length());
}

DialogAddEditComment::~DialogAddEditComment()
{
    delete ui;
}

void DialogAddEditComment::on_lineEdit_textChanged(const QString &arg1)
{
    comment = arg1;
}
