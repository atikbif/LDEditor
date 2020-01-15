#ifndef NOTELEMENT_H
#define NOTELEMENT_H

#include "ldelement.h"

class NotElement : public LDElement
{
public:
    NotElement(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // NOTELEMENT_H
