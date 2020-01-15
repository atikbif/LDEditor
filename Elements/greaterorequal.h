#ifndef GREATEROREQUAL_H
#define GREATEROREQUAL_H

#include "ldelement.h"

class GreaterOrEqual: public LDElement
{
public:
    GreaterOrEqual(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // GREATEROREQUAL_H
