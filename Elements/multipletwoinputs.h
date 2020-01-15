#ifndef MULTIPLETWOINPUTS_H
#define MULTIPLETWOINPUTS_H

#include "ldelement.h"

class MultipleTwoInputs : public LDElement
{
public:
    MultipleTwoInputs(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // MULTIPLETWOINPUTS_H
