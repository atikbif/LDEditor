#ifndef PLCVARCONTAINER_H
#define PLCVARCONTAINER_H

#include "plcvar.h"
#include <vector>
#include <optional>
#include <map>

class PLCVarContainer final
{
    static std::vector<PLCVar> vars;
    static std::vector<PLCVar> saveVars;
    static std::map<QString,QString> sysVarsComments;

public:

    static PLCVarContainer& getInstance();
    void clearSysVarsComments();
    std::map<QString,QString> getSysVarsComments() const {return sysVarsComments;}
    void addVar(const PLCVar &var);
    void deleteVar(const QString &group, const QString &name, QString parentGroup="");
    void updateComment(const QString &group, const QString &name,const QString &comment, QString parentGroup="");
    void renameGroup(const QString &group, const QString &newName, QString parentGroup="");
    void delGroup(const QString &group, QString parentGroup="");
    std::vector<QString> getVarGroups(QString parentGroup="") const;
    std::vector<QString> getParentGroups() const;
    std::vector<QString> getNotSystemVarGroups() const;
    std::vector<QString> getSystemVarGroups(QString parentGroup="") const;
    std::vector<PLCVar> getVarsByGroup(const QString &grName,QString parentGroup="") const;
    std::optional<PLCVar> getVarByGroupAndName(const QString &grName, const QString &name,QString parentGroup="") const;
    std::optional<PLCVar> getVarByName(const QString &name) const;
    void saveState();
    void recoveryState();

private:
    PLCVarContainer() = default;
    ~PLCVarContainer() = default;

    PLCVarContainer(const PLCVarContainer&) = delete;
    PLCVarContainer operator=(const PLCVarContainer&) = delete;
    PLCVarContainer(PLCVarContainer&&) = delete;
    PLCVarContainer& operator=(PLCVarContainer&&) = delete;
};

#endif // PLCVARCONTAINER_H
