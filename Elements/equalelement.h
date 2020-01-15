#ifndef EQUALELEMENT_H
#define EQUALELEMENT_H

#include "ldelement.h"

class EqualElement: public LDElement
{
public:
    EqualElement(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // EQUALELEMENT_H
