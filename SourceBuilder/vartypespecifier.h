#ifndef VARTYPESPECIFIER_H
#define VARTYPESPECIFIER_H

#include <QString>
#include <vector>
#include "ldelement.h"

class VarTypeSpecifier
{
public:
    VarTypeSpecifier() = default;
    static QString getOuType(const LDElement &el, const std::vector<QString> inpTypes);
    static QString getFuncName(const LDElement &el, const std::vector<QString> inpTypes);
};

#endif // VARTYPESPECIFIER_H
