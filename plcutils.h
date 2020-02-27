#ifndef PLCUTILS_H
#define PLCUTILS_H

#include <QString>
#include <set>

class PLCUtils
{
    static std::set<QString> ethPLCNames;
    static std::set<QString> plcNames;
    static std::set<QString> adcPLCNames;
    static std::set<QString> modbVastPLCNames;
public:
    PLCUtils();
    static bool isPLCSupportEth(const QString &plcName);
    static bool isPLCSupportADC(const QString &plcName);
    static bool isPLCSupportModbusMaster(const QString &plcName);
    static bool checkPLCCode(const QString &plcName, int code);
};

#endif // PLCUTILS_H
