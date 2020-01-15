#ifndef RELAY_H
#define RELAY_H

#include "ldelement.h"

class Relay:public LDElement
{
public:
    Relay(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // RELAY_H
