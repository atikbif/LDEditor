#ifndef PLCVARCONTAINER_H
#define PLCVARCONTAINER_H

#include "plcvar.h"
#include <vector>
#include <optional>

class PLCVarContainer final
{
    static std::vector<PLCVar> vars;
    static std::vector<PLCVar> saveVars;

public:
    static PLCVarContainer& getInstance();
    void addVar(const PLCVar &var);
    void deleteVar(const QString &group, const QString &name);
    void updateComment(const QString &group, const QString &name,const QString &comment);
    void renameGroup(const QString &group, const QString &newName);
    void delGroup(const QString &group);
    std::vector<QString> getVarGroups() const;
    std::vector<QString> getNotSystemVarGroups() const;
    std::vector<QString> getSystemVarGroups() const;
    std::vector<PLCVar> getVarsByGroup(const QString &grName) const;
    std::optional<PLCVar> getVarByGroupAndName(const QString &grName, const QString &name) const;
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
