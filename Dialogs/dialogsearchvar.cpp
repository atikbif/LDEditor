#include "dialogsearchvar.h"
#include "ui_dialogsearchvar.h"

DialogSearchVar::DialogSearchVar(const QString &grName, const QString &varName, QWidget *parent) :
    QDialog(parent), grName(grName), varName(varName),
    ui(new Ui::DialogSearchVar)
{
    ui->setupUi(this);
    ui->lineEditGroup->setText(grName);
    ui->lineEditVar->setText(varName);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    setWindowOpacity(0.8);
}

void DialogSearchVar::addElement(int pageNum, int row, int column, const QString &elName)
{
    int rowCnt = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCnt);
    ui->tableWidget->setItem(rowCnt,0,new QTableWidgetItem(QString::number(pageNum)));
    ui->tableWidget->setItem(rowCnt,1,new QTableWidgetItem(QString::number(row)));
    ui->tableWidget->setItem(rowCnt,2,new QTableWidgetItem(QString::number(column)));
    ui->tableWidget->setItem(rowCnt,3,new QTableWidgetItem(elName));
}

DialogSearchVar::~DialogSearchVar()
{
    delete ui;
}

void DialogSearchVar::on_tableWidget_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column)
    int pageNum = ui->tableWidget->item(row,0)->text().toInt();
    int elRow = ui->tableWidget->item(row,1)->text().toInt();
    int elcol = ui->tableWidget->item(row,2)->text().toInt();
    emit goToElement(pageNum, elRow, elcol);
}
