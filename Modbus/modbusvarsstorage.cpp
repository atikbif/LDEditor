#include "modbusvarsstorage.h"
#include <algorithm>

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

QByteArray ModbusVarsStorage::toBytes() const
{
    QByteArray res;
    return res;
}

void ModbusVarsStorage::fromBytes(QByteArray &value)
{
    Q_UNUSED(value)
}
