#ifndef DIALOGMODBUSMASTER_H
#define DIALOGMODBUSMASTER_H

#include <QDialog>
#include "modbusvar.h"
#include "modbusvarsstorage.h"

namespace Ui {
class DialogModbusMaster;
}

class DialogModbusMaster : public QDialog
{
    Q_OBJECT
    ModbusVar var;
    //ModbusVarsStorage vars;

public:
    explicit DialogModbusMaster(QWidget *parent = nullptr);
    ~DialogModbusMaster();

private slots:
    void on_pushButtonAddVar_clicked();

    void on_pushButtonEdit_clicked();

    void on_pushButtonturnOnAll_clicked();

    void on_pushButtonturnOffAll_clicked();

    void on_pushButtonDeleteVar_clicked();

    void on_pushButtonCanal_clicked();

private:
    Ui::DialogModbusMaster *ui;
    void updateRow(int row, const ModbusVar &v);
};

#endif // DIALOGMODBUSMASTER_H
