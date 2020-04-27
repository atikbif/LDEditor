#include "dialogvarinitlist.h"
#include "ui_dialogvarinitlist.h"

DialogVarInitList::DialogVarInitList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogVarInitList)
{
    ui->setupUi(this);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

DialogVarInitList::~DialogVarInitList()
{
    delete ui;
}

void DialogVarInitList::addVar(const QString &vName, const QString &userVarName, const QString &initValue)
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(vName));
    ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(userVarName));
    ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem(initValue));
}
