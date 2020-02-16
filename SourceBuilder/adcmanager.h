#ifndef ADCMANAGER_H
#define ADCMANAGER_H

#include <vector>
#include <QString>
#include "plcconfig.h"

class ADCManager
{
public:
    ADCManager();
    static std::vector<QString> getConverterFunction(PLCConfig config, int cnt);
};

#endif // ADCMANAGER_H
