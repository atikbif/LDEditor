#ifndef AINSENSOR_H
#define AINSENSOR_H

#include <QString>

class AINSensor
{
    QString name;
    QString unit;
    QString lowLimit;
    QString highLimit;
    int sensType = 0;
    // 0 - 0.4-2 В
    // 1 - 0-20 мА
    // 2 - 4-20 мА
    // 3 - 2-10 мА
    // 4 - 0-2.5 В

public:
    AINSensor();
    void setName(const QString &value);
    void setUnit(const QString &value);
    void setLowLimit(const QString &value);
    void setHighLimit(const QString &value);
    void setSensorType(int value);
    void setSensorTypeText(const QString &type);
    QString getName() const {return name;}
    QString getTranscriptedName();
    QString getUnit() const {return unit;}
    QString getLowLimit() const {return lowLimit;}
    QString getHighLimit() const {return highLimit;}
    int getSensorType() const {return sensType;}
    QString getSensorTypeText() const;
};

#endif // AINSENSOR_H
