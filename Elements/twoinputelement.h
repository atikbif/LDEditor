#ifndef TWOINPUTELEMENT_H
#define TWOINPUTELEMENT_H

#include "ldelement.h"

class TwoInputElement:public LDElement
{
    QString iconFile;
public:
    TwoInputElement(qreal cell_width, qreal cell_height, const QString icon);

    // LDElement interface
public:
    LDElement *clone();
};

#endif // TWOINPUTELEMENT_H
