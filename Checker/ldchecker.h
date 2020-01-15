#ifndef LDCHECKER_H
#define LDCHECKER_H

#include "ldelement.h"

class LDChecker
{
public:
    LDChecker() = default;
    static std::vector<QString> checkVarConnectedElements(std::vector<LDElement*> &els);
};

#endif // LDCHECKER_H
