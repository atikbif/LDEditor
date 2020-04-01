#ifndef PLCCONFIG_H
#define PLCCONFIG_H

#include <QString>
#include <vector>
#include <QByteArray>
#include "ainsensor.h"

class PLCConfig
{
    std::vector<bool> input_type;
    std::vector<int> sensor_type;
    std::vector<AINSensor> adcSensors;
    QByteArray settings;
    QString name;
public:
    PLCConfig(const QString &name="", int input_cnt=64, int ai_cnt=64);
    void setName(const QString &name);
    QString getName() const {return name;}
    void setInputType(int num, bool value);
    void setSensorType(int num, int value);
    bool getInputType(int num) const;
    int getSensorType(int num) const;
    void clearADCSensors() {adcSensors.clear();}
    std::size_t getADCSensorsCount() const {return adcSensors.size();}
    AINSensor getADCSensor(int num) const;
    std::vector<AINSensor> getADCSensors() const {return adcSensors;}
    void addADCSensor(AINSensor sensor);
    QByteArray toBytes() const;
    QByteArray getSettings() const {return settings;}
    int getTDUCode(int adcNum) const;
    int getSensorTypeCode(int adcNum) const;
    void setSettings(const QByteArray &inp) {settings = inp;}
    void fromBytes(QByteArray &value);
};

#endif // PLCCONFIG_H
