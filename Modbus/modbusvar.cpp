#include "modbusvar.h"

const int ModbusVar::maxNum = 64;

ModbusVar::ModbusVar(int num)
{
    if(num>=0 && num<=maxNum) varNum = num;
}

void ModbusVar::setNumber(int value)
{
    if(value>=0 && value<=maxNum) varNum = value;
}

void ModbusVar::setCanNumber(int value)
{
    canNum = value;
}

void ModbusVar::setType(ModbusVar::memType value)
{
    mem = value;
}

void ModbusVar::setNetAddress(int value)
{
    netAddr = value;
}

void ModbusVar::setMemAddress(int value)
{
    memAddr = value;
}

void ModbusVar::setWriteFlag(bool value)
{
    writeFlag = value;
}

void ModbusVar::setActiveFlag(bool value)
{
    activ = value;
}

void ModbusVar::setComment(const QString &value)
{
    comment = value;
}
