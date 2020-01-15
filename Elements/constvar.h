#ifndef CONSTVAR_H
#define CONSTVAR_H

#include "ldelement.h"

class ConstVar: public LDElement
{
    PLCVar::varType value=static_cast<short>(0);
    QString constVarType = "short";

public:
    ConstVar(qreal cell_width, qreal cell_height);
    PLCVar::varType getValue() const {return value;}
    void setValue(const PLCVar::varType &value);
    QString getConstVarType() const {return constVarType;}
    QString getStringValue() const;
    void setConstVartype(const QString &vType) {constVarType=vType;}
    void setStringValue(const QString &varValue);

    // LDElement interface
public:
    LDElement *clone();

    // LDElement interface
public:
    void updateProperties();
};

#endif // CONSTVAR_H
