#include "delayon.h"

#include <QDebug>

DelayOn::DelayOn(qreal cell_width, qreal cell_height):TwoInputElement (cell_width,cell_height,":/images/delayOn.svg")
{
    setType("delay on");
}
