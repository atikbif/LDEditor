#include "modbusvarsstorage.h"
#include <algorithm>
#include <QDataStream>

ModbusVarsStorage &ModbusVarsStorage::getInstance()
{
    static ModbusVarsStorage instance;
    return instance;
}

ModbusVarsStorage::ModbusVarsStorage()
{

}

bool ModbusVarsStorage::addModbusVar(const ModbusVar &v)
{
    auto it = std::find_if(vars.begin(),vars.end(),[v](auto value){return value->getNumber()==v.getNumber();});
    if(it!=vars.end()) return false;
    QSharedPointer<ModbusVar> var(new ModbusVar(v));
    vars.push_back(var);
    return true;
}

bool ModbusVarsStorage::replaceModbusVar(int varNum, const ModbusVar &v)
{
    auto it = std::find_if(vars.begin(),vars.end(),[varNum](auto value){return value->getNumber()==varNum;});
    if(it!=vars.end()) {
        **it = v;
        return true;
    }
    return false;
}

bool ModbusVarsStorage::deleteModbusVar(int varNum)
{
    auto it = std::find_if(vars.begin(),vars.end(),[varNum](auto value){return value->getNumber()==varNum;});
    if(it!=vars.end()) {
        vars.erase(it);
        return true;
    }
    return false;
}

QSharedPointer<ModbusVar> ModbusVarsStorage::getModbusVarByIndex(int index)
{
    if((index>=0)&&(index<getVarCnt())) {
        return vars.at(index);
    }
    return QSharedPointer<ModbusVar>(new ModbusVar());
}

QSharedPointer<ModbusVar> ModbusVarsStorage::getModbusVarByNum(int varNum)
{
    auto it = std::find_if(vars.begin(),vars.end(),[varNum](auto value){return value->getNumber()==varNum;});
    if(it!=vars.end()) {
        return *it;
    }
    return QSharedPointer<ModbusVar>(new ModbusVar());
}

bool ModbusVarsStorage::isVarExists(int varNum) const
{
    auto it = std::find_if(vars.begin(),vars.end(),[varNum](auto value){return value->getNumber()==varNum;});
    if(it!=vars.end()) return true;
    return false;
}

void ModbusVarsStorage::enableAll()
{
    for(auto &var:vars) {
        var->setActiveFlag(true);
    }
}

void ModbusVarsStorage::disableAll()
{
    for(auto &var:vars) {
        var->setActiveFlag(false);
    }
}

int ModbusVarsStorage::getMaxLength(int canNum) const
{
    return canals.getMaxLength(canNum);
}

int ModbusVarsStorage::getMaxUnusedSpace(int canNum) const
{
    return canals.getMaxUnusedSpace(canNum);
}

int ModbusVarsStorage::getPeriod(int canNum) const
{
    return canals.getPeriod(canNum);
}

void ModbusVarsStorage::setMaxLength(int canNum, int value)
{
    canals.setMaxLength(canNum, value);
}

void ModbusVarsStorage::setMaxUnusedSpace(int canNum, int value)
{
    canals.setMaxUnusedSpace(canNum, value);
}

void ModbusVarsStorage::setPeriod(int canNum, int value)
{
    canals.setPeriod(canNum, value);
}

void ModbusVarsStorage::setModbusCanals(ModbusCanals value)
{
    canals = value;
}

void ModbusVarsStorage::reset()
{
    canals = ModbusCanals();
    vars.clear();
}

QByteArray ModbusVarsStorage::toBytes()
{
    QByteArray res;
    QDataStream out(&res,QIODevice::WriteOnly);
    int code = 12148;
    out << code;
    int version = 1;
    out << version;
    int cnt = getVarCnt();
    out << cnt;
    for(int i=0;i<cnt;i++) {
        auto v = getModbusVarByIndex(i);
        int vType = static_cast<int>(v->getType());
        out << vType;
        out << v->getNumber();
        out << v->getComment();
        out << v->getMemAddr();
        out << v->getWriteFlag();
        out << v->getActiveFlag();
        out << v->getNetAddress();
        out << v->getCanalNumber();
    }
    int canNums = canals.getCanNums();
    out << canNums;
    for(int i=0;i<canNums;i++) {
        out << canals.getPeriod(i+1);
        out << canals.getMaxLength(i+1);
        out << canals.getMaxUnusedSpace(i+1);
    }

    return res;
}

void ModbusVarsStorage::fromBytes(QByteArray &value)
{
    QDataStream in(&value,QIODevice::ReadOnly);
    reset();
    int code = 0;
    in >> code;
    int version = 0;
    in >> version;
    if(code==12148) {
        if(version==1) {
            int cnt = 0;
            in >> cnt;
            for(int i=0;i<cnt;i++) {
                ModbusVar v;
                int vType = 0;
                in >> vType;
                v.setType(static_cast<ModbusVar::memType>(vType));
                int vNum = 0;
                in >> vNum;
                v.setNumber(vNum);
                QString vComment;
                in >> vComment;
                v.setComment(vComment);
                int vMemAddr = 0;
                in >> vMemAddr;
                v.setMemAddress(vMemAddr);
                bool vWrFlag = false;
                in >> vWrFlag;
                v.setWriteFlag(vWrFlag);
                bool vActFlag = false;
                in >> vActFlag;
                v.setActiveFlag(vActFlag);
                int vNetAddr = 0;
                in >> vNetAddr;
                v.setNetAddress(vNetAddr);
                int vCanNum = 0;
                in >> vCanNum;
                v.setCanNumber(vCanNum);
                addModbusVar(v);
            }
            int canCnt = 0;
            in >> canCnt;
            for(int i=0;i<canCnt;i++) {
                int period = 0;
                in >> period;
                canals.setPeriod(i+1,period);
                int maxLength = 0;
                in >> maxLength;
                canals.setMaxLength(i+1,maxLength);
                int maxUnusedSpace = 0;
                in >> maxUnusedSpace;
                canals.setMaxUnusedSpace(i+1,maxUnusedSpace);
            }
        }
    }
}
