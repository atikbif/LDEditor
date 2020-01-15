#ifndef HORLINE_H
#define HORLINE_H

#include "ldelement.h"

class HorLine: public LDElement
{
public:
    HorLine(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // HORLINE_H
