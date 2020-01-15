#include "dialogsearch.h"
#include "ui_dialogsearch.h"

DialogSearch::DialogSearch(int &pageNum, int &col, int &row, QWidget *parent) :
    QDialog(parent),
    pageNum(pageNum),col(col),row(row),
    ui(new Ui::DialogSearch)

{
    ui->setupUi(this);
    setWindowOpacity(0.95);
}

DialogSearch::~DialogSearch()
{
    delete ui;
}

void DialogSearch::on_radioButtonDocument_toggled(bool checked)
{
    config.fullDocument = checked;
}

void DialogSearch::on_pushButtonSearch_clicked()
{
    config.searchNames = ui->checkBoxName->isChecked();
    config.searchComments = ui->checkBoxComment->isChecked();
    config.searchConnections = ui->checkBoxConnection->isChecked();
    config.fullDocument = ui->radioButtonDocument->isChecked();
    config.registerFlag = ui->checkBoxRegister->isChecked();
    config.fullWord = ui->checkBoxFull->isChecked();
    config.searchWord = ui->lineEditSearch->text();
    config.connectionVarName = ui->lineEditConnection->text();
    config.replaceVarName = ui->lineEditReplace->text();
    emit startSearch(config,pageNum,col,row);
}

void DialogSearch::on_pushButtonReplaceAll_clicked()
{
    emit replaceVarName(ui->lineEditConnection->text(),ui->lineEditReplace->text());
}
