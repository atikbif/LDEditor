#ifndef MODBUSVAR_H
#define MODBUSVAR_H

#include <QString>

class ModbusVar
{
public:
    enum memType {INPUTS, COILS, HOLD_REG, INP_REG};
private:
    static const int maxNum;
    int varNum;
    int canNum;
    memType mem;
    int netAddr;
    bool writeFlag;
    int memAddr;
    bool activ;
    QString comment;
public:
    explicit ModbusVar(int num=1);
    int getNumber() const {return varNum;}
    int getCanalNumber() const {return canNum;}
    memType getType() const {return mem;}
    int getNetAddress() const {return netAddr;}
    bool getWriteFlag() const {return writeFlag;}
    int getMemAddr() const {return memAddr;}
    bool getActiveFlag() const {return activ;}
    QString getComment() const {return comment;}
    void setNumber(int value);
    void setCanNumber(int value);
    void setType(memType value);
    void setNetAddress(int value);
    void setMemAddress(int value);
    void setWriteFlag(bool value);
    void setActiveFlag(bool value);
    void setComment(const QString &value);
};

#endif // MODBUSVAR_H
