#ifndef PLCPARAMS_H
#define PLCPARAMS_H

#include <QString>
#include <vector>

class PLCParams
{
public:
    static int aiCnt;
    static int diCnt;
    static int doCnt;
    static int ibit_cnt;
    static int ireg_cnt;
    static int tmrms_cnt;
    static int tmrs_cnt;
    static int frsh_cnt;
    static int frd_cnt;
    static int frl_cnt;
    static QString osBuildName;
    static int code;
    static void readParamsByPLCName(const QString &plcName);
    static std::vector<QString> readPLCNames();
};

#endif // PLCPARAMS_H
