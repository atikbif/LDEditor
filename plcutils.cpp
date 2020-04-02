#include "plcutils.h"
#include <QDebug>
#include "plcvarcontainer.h"
#include "transcripter.h"

std::set<QString> PLCUtils::ethPLCNames = {"PC21-1 2019"};
std::set<QString> PLCUtils::plcNames = {"PC21-1 2019"};
std::set<QString> PLCUtils::adcPLCNames = {"PC21-1 2019"};
std::set<QString> PLCUtils::diPLCNames = {"PC21-1 2019"};
std::set<QString> PLCUtils::doPLCNames = {"PC21-1 2019"};
std::set<QString> PLCUtils::modbVastPLCNames = {"PC21-1 2019"};
std::set<QString> PLCUtils::canPLCNames = {"PC21-1 2019"};

PLCUtils::PLCUtils()
{

}

bool PLCUtils::isPLCSupportEth(const QString &plcName)
{
    if(ethPLCNames.find(plcName)!=ethPLCNames.end()) return true;
    return false;
}

bool PLCUtils::isPLCSupportADC(const QString &plcName)
{
    if(adcPLCNames.find(plcName)!=adcPLCNames.end()) return true;
    return false;
}

bool PLCUtils::isPLCSupportDI(const QString &plcName)
{
    if(diPLCNames.find(plcName)!=diPLCNames.end()) return true;
    return false;
}

bool PLCUtils::isPLCSupportDO(const QString &plcName)
{
    if(doPLCNames.find(plcName)!=doPLCNames.end()) return true;
    return false;
}

bool PLCUtils::isPLCSupportModbusMaster(const QString &plcName)
{
    if(modbVastPLCNames.find(plcName)!=modbVastPLCNames.end()) return true;
    return false;
}

bool PLCUtils::isPLCSupportCAN(const QString &plcName)
{
    if(canPLCNames.find(plcName)!=canPLCNames.end()) return true;
    return false;
}

bool PLCUtils::checkPLCCode(const QString &plcName, int code)
{
    if(plcNames.find(plcName)!=plcNames.end()) {
        if(plcName=="PC21-1 2019") {
            code = code >> 16;
            if(code==0x01C4) {return true;} // plc
            if(code==0x01F5) {return true;} // bootloader
        }
    }
    return false;
}

QString PLCUtils::getADCName(int adcNum)
{
    QString res;
    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("Аналоговые входы");
    if(adcNum>=0 && adcNum<static_cast<int>(vars.size())) {
        res = vars.at(static_cast<std::size_t>(adcNum)).getComment();
        res = Transcripter::getTranscriptedWord(res);
        if(res.size()>16) res.resize(16);
    }
    return res;
}

QString PLCUtils::getDIName(int diNum)
{
    QString res;
    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("Дискретные входы");
    if(diNum>=0 && diNum < static_cast<int>(vars.size())) {
        res = vars.at(static_cast<std::size_t>(diNum)).getComment();
        res = Transcripter::getTranscriptedWord(res);
        if(res.size()>16) res.resize(16);
    }
    return res;
}

QString PLCUtils::getDOName(int doNum)
{
    QString res;
    std::vector<PLCVar> vars = PLCVarContainer::getInstance().getVarsByGroup("Дискретные выходы");
    if(doNum>=0 && doNum < static_cast<int>(vars.size())) {
        res = vars.at(static_cast<std::size_t>(doNum)).getComment();
        res = Transcripter::getTranscriptedWord(res);
        if(res.size()>16) res.resize(16);
    }
    return res;
}
