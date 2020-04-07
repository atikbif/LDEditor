#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <QDataStream>
#include "ldelement.h"
#include "closedcontact.h"
#include "opencontact.h"
#include "commentelement.h"
#include "constvar.h"
#include "variable.h"
#include "delayoff.h"
#include "delayon.h"
#include "divideelement.h"
#include "equalelement.h"
#include "greaterelement.h"
#include "greaterorequal.h"
#include "lesselement.h"
#include "lessorequal.h"
#include "minuselement.h"
#include "multipletwoinputs.h"
#include "notequalelement.h"
#include "plustwoinputs.h"
#include "relay.h"
#include "relayenabled.h"
#include "rstrigger.h"
#include "srtrigger.h"
#include "horline.h"
#include "verline.h"
#include "notelement.h"
#include "plcvarcontainer.h"
#include "libraryelement.h"


QDataStream &operator<<(QDataStream &stream, const LDElement &item);
std::pair<LDElement*,QString> readLDelement(QDataStream &stream);
void checkLDElement(LDElement* el);

QDataStream &operator<<(QDataStream &stream, const CommentElement &item);
QDataStream &operator<<(QDataStream &stream, const ConstVar &item);

void savePLCVarContainer(QDataStream &stream);
void readPLCVarContainer(QDataStream &stream);

void setCellConfig(qreal width, qreal height, int col_count);

#endif // SERIALIZE_H
