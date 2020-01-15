#include "rstrigger.h"

RSTrigger::RSTrigger(qreal cell_width, qreal cell_height):
    TwoInputElement (cell_width,cell_height,":/images/rstrigger.svg")
{
    setType("rs trigger");
}
