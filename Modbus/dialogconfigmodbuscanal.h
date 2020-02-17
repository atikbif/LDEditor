#ifndef DIALOGCONFIGMODBUSCANAL_H
#define DIALOGCONFIGMODBUSCANAL_H

#include <QDialog>
#include "modbuscanals.h"

namespace Ui {
class DialogConfigModbusCanal;
}

class DialogConfigModbusCanal : public QDialog
{
    Q_OBJECT

    ModbusCanals canals;

public:
    explicit DialogConfigModbusCanal(QWidget *parent = nullptr);
    ~DialogConfigModbusCanal();
    void setCanals(ModbusCanals canals);
    ModbusCanals getCanals() const {return canals;}
private slots:
    void on_spinBoxCanalNum_valueChanged(int arg1);

    void on_spinBoxMaxLength_valueChanged(int arg1);

    void on_spinBoxMaxUnusedSpace_valueChanged(int arg1);

    void on_spinBoxPeriod_valueChanged(int arg1);

private:
    Ui::DialogConfigModbusCanal *ui;
};

#endif // DIALOGCONFIGMODBUSCANAL_H
