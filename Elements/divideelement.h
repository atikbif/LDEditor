#ifndef DIVIDEELEMENT_H
#define DIVIDEELEMENT_H

#include "ldelement.h"

class DivideElement:public LDElement
{
public:
    DivideElement(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // DIVIDEELEMENT_H
