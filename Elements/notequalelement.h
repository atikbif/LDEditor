#ifndef NOTEQUALELEMENT_H
#define NOTEQUALELEMENT_H

#include "ldelement.h"

class NotEqualElement: public LDElement
{
public:
    NotEqualElement(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // NOTEQUALELEMENT_H
