#ifndef VARIABLE_H
#define VARIABLE_H

#include "ldelement.h"

class Variable: public LDElement
{
public:
    Variable(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // VARIABLE_H
