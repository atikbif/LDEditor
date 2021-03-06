#include "vartypespecifier.h"
#include "Elements/constvar.h"
#include "plcvarcontainer.h"
#include <QDebug>

QString VarTypeSpecifier::getOuType(const LDElement &el, const std::vector<QString> &inpTypes)
{
    std::map<QString,int> typeMap = {{"bool",0},{"unsigned short",1},{"unsigned long",2},{"double",3}};
    QString type = el.getType();
    if(type=="closed contact") {
        return "bool";
    }
    if(type=="const variable") {
        const auto* v = dynamic_cast<const ConstVar*>(&el);
        if(v) {
            return v->getConstVarType();
        }
    }else if(type=="divide") {
        int maxValue = 1;
        QString maxType = "unsigned short";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        return maxType;
    }else if(type=="equal") {
        return "bool";
    }else if(type=="greather than") {
        return "bool";
    }else if(type=="greather or equal") {
        return "bool";
    }else if(type=="less than") {
        return "bool";
    }else if(type=="less or equal") {
        return "bool";
    }else if(type=="minus") {
        int maxValue = 1;
        QString maxType = "unsigned short";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        return maxType;
    }else if(type=="multiple two inputs") {
        int maxValue = 1;
        QString maxType = "unsigned short";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        return maxType;
    }else if(type=="not equal") {
        return "bool";
    }else if(type=="open contact") {
        return "bool";
    }else if(type=="plus two inputs") {
        int maxValue = 1;
        QString maxType = "unsigned short";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        return maxType;
    }else if(type=="relay") {
        QString vGroup = el.connectedVar.group;
        QString vName = el.connectedVar.name;
        QString vParent = el.connectedVar.parentGroup;
        std::optional<PLCVar> v = PLCVarContainer::getInstance().getVarByGroupAndName(vGroup,vName,vParent);
        if(v) return v->getType();
    }else if(type=="relay_enabled") {
        QString vGroup = el.connectedVar.group;
        QString vName = el.connectedVar.name;
        QString vParent = el.connectedVar.parentGroup;
        std::optional<PLCVar> v = PLCVarContainer::getInstance().getVarByGroupAndName(vGroup,vName,vParent);
        if(v) return v->getType();
    }else if(type=="variable") {
        QString vGroup = el.connectedVar.group;
        QString vName = el.connectedVar.name;
        QString vParent = el.connectedVar.parentGroup;
        std::optional<PLCVar> v = PLCVarContainer::getInstance().getVarByGroupAndName(vGroup,vName,vParent);
        if(v) return v->getType();
    }else if(type=="delay on") {
        if(!inpTypes.empty()) return inpTypes[0];
    }else if(type=="delay off") {
        if(!inpTypes.empty()) return inpTypes[0];
    }else if(type=="rs trigger") {
        return "bool";
    }else if(type=="sr trigger") {
        return "bool";
    }else if(type=="not") {
        return "bool";
    }else if(type=="counter") {
        return "unsigned long";
    }
    return "unsigned short";
}

QString VarTypeSpecifier::getFuncName(const LDElement &el, const std::vector<QString> &inpTypes)
{
    QString res = "empty(";
    QString type = el.getType();
    std::map<QString,int> typeMap = {{"bool",0},{"unsigned short",1},{"unsigned long",2},{"double",3}};
    if(type=="closed contact") {
        if(!el.connectedVar.name.isEmpty()) res = "clos_contact(" + el.connectedVar.name + ",";

    }else if(type=="const variable") {
        const auto* v = dynamic_cast<const ConstVar*>(&el);
        if(v) {
            res = "const_" + v->getConstVarType().replace(" ","_") + "("+v->getStringValue()+",";
        }
    }else if(type=="divide") {
        int maxValue = 1;
        QString maxType = "unsigned short";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        res = "divide_"+ maxType.replace(" ","_") + "(";
    }else if(type=="equal") {
        int maxValue = 0;
        QString maxType = "bool";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        res = "equal_"+ maxType.replace(" ","_") + "(";
    }else if(type=="greather than") {
        int maxValue = 0;
        QString maxType = "bool";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        res = "greater_"+ maxType.replace(" ","_") + "(";
    }else if(type=="greather or equal") {
        int maxValue = 0;
        QString maxType = "bool";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        res = "greater_or_equal_"+ maxType.replace(" ","_") + "(";
    }else if(type=="less than") {
        int maxValue = 0;
        QString maxType = "bool";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        res = "less_"+ maxType.replace(" ","_") + "(";
    }else if(type=="less or equal") {
        int maxValue = 0;
        QString maxType = "bool";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        res = "less_or_equal_"+ maxType.replace(" ","_") + "(";
    }else if(type=="minus") {
        int maxValue = 1;
        QString maxType = "unsigned short";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        res = "minus_"+ maxType.replace(" ","_") + "(";
    }else if(type=="multiple two inputs") {
        int maxValue = 1;
        QString maxType = "unsigned short";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        res = "multiple_"+ maxType.replace(" ","_") + "(";
    }else if(type=="not equal") {
        int maxValue = 0;
        QString maxType = "bool";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        res = "not_equal_"+ maxType.replace(" ","_") + "(";
    }else if(type=="open contact") {
        if(!el.connectedVar.name.isEmpty()) res = "open_contact(" + el.connectedVar.name + ",";

    }else if(type=="plus two inputs") {
        int maxValue = 1;
        QString maxType = "unsigned short";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        res = "plus_"+ maxType.replace(" ","_") + "(";
    }else if(type=="relay") {
        QString vGroup = el.connectedVar.group;
        QString vName = el.connectedVar.name;
        QString vParent = el.connectedVar.parentGroup;
        std::optional<PLCVar> v = PLCVarContainer::getInstance().getVarByGroupAndName(vGroup,vName,vParent);
        if(v) {
            res = "relay_"+ v->getType().replace(" ","_") + "(&" + vName + ",";
        }

    }else if(type=="relay_enabled") {
        QString vGroup = el.connectedVar.group;
        QString vName = el.connectedVar.name;
        QString vParent = el.connectedVar.parentGroup;
        std::optional<PLCVar> v = PLCVarContainer::getInstance().getVarByGroupAndName(vGroup,vName,vParent);
        if(v) {
            res = "relay_enabled_"+ v->getType().replace(" ","_") + "(&" + vName + ",";
        }

    }else if(type=="variable") {
        QString vGroup = el.connectedVar.group;
        QString vName = el.connectedVar.name;
        QString vParent = el.connectedVar.parentGroup;
        std::optional<PLCVar> v = PLCVarContainer::getInstance().getVarByGroupAndName(vGroup,vName,vParent);
        if(v) {
            res = "variable_"+ v->getType().replace(" ","_") + "(" + vName + ",";
        }
    }else if(type=="delay on") {
        res="delay_on(";
    }else if(type=="delay off") {
        res="delay_off(";
    }else if(type=="rs trigger") {
        res="rs_trig(";
    }else if(type=="sr trigger") {
        res="sr_trig(";
    }else if(type=="counter") {
        res="counter(";
    }else if(type=="not") {
        int maxValue = 0;
        QString maxType = "bool";
        for(const QString &t:inpTypes) if(typeMap[t]>maxValue) {maxValue=typeMap[t];maxType=t;}
        res = "not_operation_"+ maxType.replace(" ","_") + "(";
    }
    return res;
}
