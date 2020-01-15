#ifndef CLOSEDCONTACT_H
#define CLOSEDCONTACT_H

#include "ldelement.h"
#include <QDataStream>

class ClosedContact: public LDElement
{
public:
    static const QString elementType;
    ClosedContact(qreal cell_width, qreal cell_height);

    // LDElement interface
public:
    LDElement *clone() override;
};

#endif // CLOSEDCONTACT_H
