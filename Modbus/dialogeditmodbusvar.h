#ifndef DIALOGEDITMODBUSVAR_H
#define DIALOGEDITMODBUSVAR_H

#include <QDialog>
#include "modbusvar.h"

namespace Ui {
class DialogEditModbusVar;
}

class DialogEditModbusVar : public QDialog
{
    Q_OBJECT

    ModbusVar var;

public:
    explicit DialogEditModbusVar(QWidget *parent = nullptr);
    ~DialogEditModbusVar();
    ModbusVar getVar() const {return var;}
    void setVar(ModbusVar value);

private slots:
    void on_spinBoxVarNum_valueChanged(int arg1);

    void on_spinBoxCanNum_valueChanged(int arg1);

    void on_comboBox_currentIndexChanged(int index);

    void on_spinBoxNetAddr_valueChanged(int arg1);

    void on_spinBoxMemAddr_valueChanged(int arg1);

    void on_lineEditComment_textChanged(const QString &arg1);

    void on_radioButtonRead_clicked();

    void on_radioButtonWrite_clicked();

    void on_checkBoxActive_clicked();

private:
    Ui::DialogEditModbusVar *ui;
};

#endif // DIALOGEDITMODBUSVAR_H
