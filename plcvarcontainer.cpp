#include "plcvarcontainer.h"
#include <set>

std::vector<PLCVar> PLCVarContainer::vars;
std::vector<PLCVar> PLCVarContainer::saveVars;


PLCVarContainer &PLCVarContainer::getInstance()
{
    static PLCVarContainer instance;
    return instance;
}

void PLCVarContainer::addVar(const PLCVar &var)
{
    vars.push_back(var);
}

void PLCVarContainer::deleteVar(const QString &group, const QString &name)
{
    auto it = std::find_if(vars.begin(),vars.end(),[group,name](const PLCVar &var){return (var.getName()==name && var.getGroup()==group);});
    if(it!=vars.end()) {
        vars.erase(it);
    }
}

void PLCVarContainer::updateComment(const QString &group, const QString &name, const QString &comment)
{
    auto it = std::find_if(vars.begin(),vars.end(),[group,name](const PLCVar &var){return (var.getName()==name && var.getGroup()==group);});
    if(it!=vars.end()) {
        it->setComment(comment);
    }
}

void PLCVarContainer::renameGroup(const QString &group, const QString &newName)
{
    for(PLCVar &var:vars) {
        if(var.getGroup()==group) var.setGroup(newName);
    }
}

void PLCVarContainer::delGroup(const QString &group)
{
    std::vector<PLCVar> vars = getVarsByGroup(group);
    std::vector<QString> varNames;
    varNames.reserve(vars.size());
    for(const auto & var:vars) {
        varNames.push_back(var.getName());
    }
    for(const QString &vName:varNames) {
        deleteVar(group,vName);
    }
}

std::vector<QString> PLCVarContainer::getVarGroups() const
{
    std::set<QString> groups;
    for(const auto &var:vars) {
        groups.insert(var.getGroup());
    }
    std::vector<QString> result;
    std::copy(groups.begin(), groups.end(), std::back_inserter(result));
    return result;
}

std::vector<QString> PLCVarContainer::getNotSystemVarGroups() const
{
    std::set<QString> groups;
    for(const auto &var:vars) {
        if(!var.isSystem()) groups.insert(var.getGroup());
    }
    std::vector<QString> result;
    std::copy(groups.begin(), groups.end(), std::back_inserter(result));
    return result;
}

std::vector<QString> PLCVarContainer::getSystemVarGroups() const
{
    std::set<QString> groups;
    for(const auto &var:vars) {
        if(var.isSystem()) groups.insert(var.getGroup());
    }
    std::vector<QString> result;
    std::copy(groups.begin(), groups.end(), std::back_inserter(result));
    return result;
}

std::vector<PLCVar> PLCVarContainer::getVarsByGroup(const QString &grName) const
{
    std::vector<PLCVar> grVars;
    for(const auto &var:vars) {
        if(var.getGroup()==grName) grVars.push_back(var);
    }
    return grVars;
}

std::optional<PLCVar> PLCVarContainer::getVarByGroupAndName(const QString &grName, const QString &name) const
{
    for(const auto &var:vars) {
        if(var.getGroup()==grName && var.getName()==name) return var;
    }
    return std::nullopt;
}

std::optional<PLCVar> PLCVarContainer::getVarByName(const QString &name) const
{
    for(const auto &var:vars) {
        if(var.getName()==name) return var;
    }
    return std::nullopt;
}

void PLCVarContainer::saveState()
{
    saveVars.clear();
    std::copy(vars.begin(),vars.end(),std::back_inserter(saveVars));
}

void PLCVarContainer::recoveryState()
{
    vars.clear();
    std::copy(saveVars.begin(),saveVars.end(),std::back_inserter(vars));
}

