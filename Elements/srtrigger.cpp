#include "srtrigger.h"

SRTrigger::SRTrigger(qreal cell_width, qreal cell_height):
    TwoInputElement (cell_width,cell_height,":/images/srtrigger.svg")
{
    setType("sr trigger");
}
