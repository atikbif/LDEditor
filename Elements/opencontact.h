#ifndef OPENCONTACT_H
#define OPENCONTACT_H

#include "ldelement.h"

class OpenContact: public LDElement
{
public:
    OpenContact(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // OPENCONTACT_H
