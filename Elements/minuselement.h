#ifndef MINUSELEMENT_H
#define MINUSELEMENT_H

#include "ldelement.h"

class MinusElement:public LDElement
{
public:
    MinusElement(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // MINUSELEMENT_H
