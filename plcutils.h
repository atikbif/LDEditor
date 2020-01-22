#ifndef PLCUTILS_H
#define PLCUTILS_H

#include <QString>
#include <set>

class PLCUtils
{
    static std::set<QString> plcNames;
public:
    PLCUtils();
    static bool isPLCSupportEth(const QString &plcName);
    static bool checkPLCCode(const QString &plcName, int code);
};

#endif // PLCUTILS_H
