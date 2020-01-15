#ifndef LESSELEMENT_H
#define LESSELEMENT_H

#include "ldelement.h"

class LessElement: public LDElement
{
public:
    LessElement(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // LESSELEMENT_H
