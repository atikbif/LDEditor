#ifndef RELAYENABLED_H
#define RELAYENABLED_H

#include "ldelement.h"

class RelayEnabled : public LDElement
{
public:
    RelayEnabled(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // RELAYENABLED_H
