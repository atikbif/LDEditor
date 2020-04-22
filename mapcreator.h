#ifndef MAPCREATOR_H
#define MAPCREATOR_H

#include "mapcontent.h"

class MapCreator
{
    const MapContent &content;
public:
    MapCreator(const MapContent &content);
    bool createFile(const QString &fName);
};

#endif // MAPCREATOR_H
