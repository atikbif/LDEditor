#ifndef LESSOREQUAL_H
#define LESSOREQUAL_H

#include "ldelement.h"

class LessOrEqual:public LDElement
{
public:
    LessOrEqual(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // LESSOREQUAL_H
