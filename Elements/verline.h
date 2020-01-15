#ifndef VERLINE_H
#define VERLINE_H

#include "ldelement.h"

class VerLine: public LDElement
{
public:
    VerLine(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // VERLINE_H
