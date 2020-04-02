#ifndef PLCUTILS_H
#define PLCUTILS_H

#include <QString>
#include <set>

class PLCUtils
{
    static std::set<QString> ethPLCNames;
    static std::set<QString> plcNames;
    static std::set<QString> adcPLCNames;
    static std::set<QString> diPLCNames;
    static std::set<QString> doPLCNames;
    static std::set<QString> modbVastPLCNames;
    static std::set<QString> canPLCNames;
public:
    PLCUtils();
    static bool isPLCSupportEth(const QString &plcName);
    static bool isPLCSupportADC(const QString &plcName);
    static bool isPLCSupportDI(const QString &plcName);
    static bool isPLCSupportDO(const QString &plcName);
    static bool isPLCSupportModbusMaster(const QString &plcName);
    static bool isPLCSupportCAN(const QString &plcName);
    static bool checkPLCCode(const QString &plcName, int code);
    static QString getADCName(int adcNum);
    static QString getDIName(int diNum);
    static QString getDOName(int doNum);
};

#endif // PLCUTILS_H
