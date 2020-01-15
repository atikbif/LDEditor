#ifndef PLUSTWOINPUTS_H
#define PLUSTWOINPUTS_H

#include "ldelement.h"

class PlusTwoInputs : public LDElement
{
public:
    PlusTwoInputs(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // PLUSTWOINPUTS_H
