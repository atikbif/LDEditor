#ifndef MODBUSCANALS_H
#define MODBUSCANALS_H

#include <vector>

class ModbusCanals
{
    struct Canal{
        int maxLength;
        int period;
        int maxUnusedSpace;
    };
    std::vector<Canal> canals;
public:
    ModbusCanals();
    int getCanNums() const;
    int getMaxLength(int canNum) const;
    int getMaxUnusedSpace(int canNum) const;
    int getPeriod(int canNum) const;
    void setMaxLength(int canNum, int value);
    void setMaxUnusedSpace(int canNum, int value);
    void setPeriod(int canNum, int value);
};

#endif // MODBUSCANALS_H
