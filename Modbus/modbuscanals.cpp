#include "modbuscanals.h"

ModbusCanals::ModbusCanals()
{
    Canal c;
    c.period = 100;
    c.maxLength = 64;
    c.maxUnusedSpace = 4;
    canals.push_back(c);
    canals.push_back(c);
}

int ModbusCanals::getCanNums() const
{
    return canals.size();
}

int ModbusCanals::getMaxLength(int canNum) const
{
    if(canNum>=1 && canNum<=getCanNums()) return canals.at(canNum-1).maxLength;
    return 0;
}

int ModbusCanals::getMaxUnusedSpace(int canNum) const
{
    if(canNum>=1 && canNum<=getCanNums()) return canals.at(canNum-1).maxUnusedSpace;
    return 0;
}

int ModbusCanals::getPeriod(int canNum) const
{
    if(canNum>=1 && canNum<=getCanNums()) return canals.at(canNum-1).period;
    return 0;
}

void ModbusCanals::setMaxLength(int canNum, int value)
{
    if(canNum>=1 && canNum<=getCanNums()) {
        canals[canNum-1].maxLength = value;
    }
}

void ModbusCanals::setMaxUnusedSpace(int canNum, int value)
{
    if(canNum>=1 && canNum<=getCanNums()) {
        canals[canNum-1].maxUnusedSpace = value;
    }
}

void ModbusCanals::setPeriod(int canNum, int value)
{
    if(canNum>=1 && canNum<=getCanNums()) {
        canals[canNum-1].period = value;
    }
}
