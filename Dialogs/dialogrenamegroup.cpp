#include "dialogrenamegroup.h"
#include "ui_dialogrenamegroup.h"

DialogRenameGroup::DialogRenameGroup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRenameGroup)
{
    ui->setupUi(this);
}

DialogRenameGroup::~DialogRenameGroup()
{
    delete ui;
}

void DialogRenameGroup::on_lineEdit_textChanged(const QString &arg1)
{
    groupName=arg1;
}
