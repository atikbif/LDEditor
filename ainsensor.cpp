#include "ainsensor.h"
#include "transcripter.h"

AINSensor::AINSensor()
{

}

void AINSensor::setName(const QString &value)
{
    name = value;
}

void AINSensor::setUnit(const QString &value)
{
    unit = value;
}

void AINSensor::setLowLimit(const QString &value)
{
    lowLimit = value;
}

void AINSensor::setHighLimit(const QString &value)
{
    highLimit = value;
}

void AINSensor::setSensorType(int value)
{
    const int maxValue = 4;
    if(value>maxValue) value = 0;
    sensType = value;
}

void AINSensor::setSensorTypeText(const QString &type)
{
    if(type=="0.4 - 2 В") sensType = 0;
    else if(type=="0 - 20 мА") sensType = 1;
    else if(type=="4 - 20 мА") sensType = 2;
    else if(type=="2 - 10 мА") sensType = 3;
    else if(type=="0 - 2.5 В") sensType = 4;
    else sensType = 0;
}

QString AINSensor::getTranscriptedName()
{
    QString res = Transcripter::getTranscriptedWord(name);
    return res;
}

QString AINSensor::getSensorTypeText() const
{
    switch(sensType) {
        case 0: return "0.4 - 2 В";
        case 1: return "0 - 20 мА";
        case 2: return "4 - 20 мА";
        case 3: return "2 - 10 мА";
        case 4: return "0 - 2.5 В";
    }
    return "";
}
