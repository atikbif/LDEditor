#include "plcutils.h"
#include <Qdebug>

std::set<QString> PLCUtils::plcNames = {"PC21-1 2019"};

PLCUtils::PLCUtils()
{

}

bool PLCUtils::isPLCSupportEth(const QString &plcName)
{
    if(plcNames.find(plcName)!=plcNames.end()) return true;
    return false;
}

bool PLCUtils::checkPLCCode(const QString &plcName, int code)
{
    if(plcNames.find(plcName)!=plcNames.end()) {
        if(plcName=="PC21-1 2019") {
            code = code >> 16;
            if(code==0x01C4) {qDebug() << "PLC";return true;} // plc
            if(code==0x01F5) {qDebug() << "BOOT";return true;} // bootloader
        }
    }
    return false;
}
