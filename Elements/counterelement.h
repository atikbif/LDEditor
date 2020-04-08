#ifndef COUNTERELEMENT_H
#define COUNTERELEMENT_H

#include "ldelement.h"

class CounterElement: public LDElement
{
public:
    CounterElement(qreal cell_width, qreal cell_height);
public:
    LDElement *clone();
};

#endif // COUNTERELEMENT_H

