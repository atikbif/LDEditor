#include "delayoff.h"

DelayOff::DelayOff(qreal cell_width, qreal cell_height):TwoInputElement (cell_width,cell_height,":/images/delayOff.svg")
{
    setType("delay off");
}
