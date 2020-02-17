#ifndef MODBUSVARSSTORAGE_H
#define MODBUSVARSSTORAGE_H

#include "modbuscanals.h"
#include <vector>
#include <QSharedPointer>
#include "modbusvar.h"

class ModbusVarsStorage
{
    ModbusCanals canals;
    std::vector<QSharedPointer<ModbusVar>> vars;
public:
    ModbusVarsStorage();
    int getVarCnt() const {return vars.size();}
    bool addModbusVar(const ModbusVar &v);
    bool replaceModbusVar(int varNum, const ModbusVar &v);
    bool deleteModbusVar(int varNum);
    QSharedPointer<ModbusVar> getModbusVarByIndex(int index);
    QSharedPointer<ModbusVar> getModbusVarByNum(int varNum);
    ModbusCanals getModbusCanals() const {return canals;}
    bool isVarExists(int varNum) const;
    void enableAll();
    void disableAll();
    int getMaxLength(int canNum) const;
    int getMaxUnusedSpace(int canNum) const;
    int getPeriod(int canNum) const;
    void setMaxLength(int canNum, int value);
    void setMaxUnusedSpace(int canNum, int value);
    void setPeriod(int canNum, int value);
    void setModbusCanals(ModbusCanals value);
    QByteArray toBytes() const;
    void fromBytes(QByteArray &value);
};

#endif // MODBUSVARSSTORAGE_H
