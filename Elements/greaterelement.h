#ifndef GREATERELEMENT_H
#define GREATERELEMENT_H

#include "ldelement.h"

class GreaterElement: public LDElement
{
public:
    GreaterElement(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // GREATERELEMENT_H
